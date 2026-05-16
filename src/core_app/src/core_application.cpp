#include <QScreen>
#include <QCommandLineParser>
#include <QCommandLineOption>

#include "core_app/core_application.h"

#include "core/utils.h"
#include "core_app/main_window.h"
#include "core_app/scheme_editor/processor_data_model.h"
#include "core_app/scheme_editor/sink_data_model.h"
#include "core_app/scheme_editor/source_data_model.h"

Q_LOGGING_CATEGORY(core_app_main, "core_app.main")


CoreApplication::CoreApplication(int32_t argc, char *argv[]) : QApplication(argc, argv) {
    setApplicationName("Data Processing System Core");
    setApplicationVersion("1.0.0");
    setOrganizationName("Data Processing System");

    _parse_command_line();

    if (not _config_manager.load(_ctx.file_configs)) {
        qCInfo(core_app_main, "Using default configuration");
    }
    _ctx.configs.enable_ui = true; // UI всегда включен для core_app

    _ctx.delegate_model_registry = std::make_shared<QtNodes::NodeDelegateModelRegistry>();
    _ctx.data_flow_graph = std::make_shared<QtNodes::DataFlowGraphModel>(_ctx.delegate_model_registry);
    _ctx.delegate_model_registry->registerModel<NumberSourceDataModel>();
    _ctx.delegate_model_registry->registerModel<NumberDisplayDataModel>();
    _ctx.delegate_model_registry->registerModel<AdditionModel>();

    if (not _ctx.file_configs.scene_graph.empty()) {
        _ctx.data_flow_graph->load(_ctx.file_configs.scene_graph);
    }

    if (_ctx.file_configs.core.run_analyser) {
        start_analyzer();
    }

    if (_ctx.file_configs.core.run_loopback) {
        start_loopback();
    }

    if (_ctx.file_configs.wled_ar_server.enabled) {
        start_wled_audio_reactive_server();
    }

    if (not _ctx.configs.enable_ui) return;
    const auto main_window = new MainWindow(*this);
    _main_window = main_window;
    connect(main_window, &MainWindow::on_config_updated, this, [this]() {
        _ctx.file_configs.scene_graph = _ctx.data_flow_graph->save();
        _config_manager.save(_ctx.file_configs);
    });

    // Подключаем сигналы управления ядром от MainWindow
    connect(main_window, &MainWindow::start_loopback_requested, this, &CoreApplication::start_loopback);
    connect(main_window, &MainWindow::stop_loopback_requested, this, &CoreApplication::stop_loopback);
    connect(main_window, &MainWindow::start_analyzer_requested, this, &CoreApplication::start_analyzer);
    connect(main_window, &MainWindow::stop_analyzer_requested, this, &CoreApplication::stop_analyzer);
    connect(main_window, &MainWindow::start_wled_server_requested, this,
            &CoreApplication::start_wled_audio_reactive_server);
    connect(main_window, &MainWindow::stop_wled_server_requested, this,
            &CoreApplication::stop_wled_audio_reactive_server);
    connect(main_window, &MainWindow::wled_server_config_changed, this,
            &CoreApplication::restart_wled_audio_reactive_server);
}

CoreApplication::~CoreApplication() {
    stop_loopback();
    stop_analyzer();
    stop_wled_audio_reactive_server();
}

void CoreApplication::_parse_command_line() {
    QCommandLineParser parser;
    parser.setApplicationDescription("Data Processing System Core Application");
    parser.addHelpOption();
    parser.addVersionOption();

    const QCommandLineOption config_option(QStringList() << "c" << "config",
                                           "Configuration file path (default: config.json)",
                                           "file", "config.json");
    parser.addOption(config_option);

    parser.process(*this);
    _config_manager.setConfigPath(parser.value(config_option));
}

int CoreApplication::exec() const {
    if (_main_window) {
        if (_ctx.file_configs.ui.move_to_primary_screen_on_start) {
            _main_window->move(primaryScreen()->availableGeometry().center() - _main_window->rect().center());
        }
        _main_window->showNormal();
    }
    return QApplication::exec();
}

void CoreApplication::start_analyzer() {
    if (_ctx.analyzer) {
        qDebug("Analyzer already started");
        return;
    }

    _ctx.analyzer = new Analyzer;
    if (not _ctx.spectrogram) {
        _ctx.spectrogram = new Spectrogram(400);
    }

    _ctx.analyzer->set_update_handler([this]() {
        // Усреднение амплитуд
        emit amplitudes_ready(_ctx.analyzer->amplitudes.left);

        // Усреднение тестовых амплитуд (Mel шкала)
        emit test_amplitudes_ready(_ctx.analyzer->amplitudes_test.left);

        // Спектрограмма (только левый канал)
        if (_ctx.spectrogram) {
            _ctx.spectrogram->push(_ctx.analyzer->amplitudes_test.left);
            emit spectrogram_updated(_ctx.spectrogram->get_vector_spectrogram());
        }

        if (_ctx.wled_ar_server) {
            _ctx.wled_ar_server->update(_ctx.analyzer->amplitudes_test.left);
        }
    });
    _ctx.analyzer->set_update_window_function_handler([this](const std::vector<float> &window) {
        emit window_function_ready(window);
    });

    _ctx.analyzer->set_samples_after_window_function_handler([this](const std::vector<float> &samples) {
        emit samples_after_window_function_ready(samples);
        if (_ctx.analyzer and not _ctx.analyzer->window.empty()) {
            emit window_function_ready(_ctx.analyzer->window);
        }
    });

    _ctx.analyzer->start();
    emit analyzer_started();
    qCInfo(core_app_main, "Analyzer started");
}

void CoreApplication::stop_analyzer() const {
    if (_ctx.analyzer) {
        _ctx.analyzer->stop();
        qCInfo(core_app_main, "Analyzer stopped");
    }
    delete _ctx.analyzer;
    delete _ctx.spectrogram;
}

void CoreApplication::start_loopback() {
    if (not _ctx.loopback) {
        _ctx.loopback = new audio::loopback::WASAPILoopback;
    }

    _ctx.loopback->set_audio_handler([this](const audio::loopback::loopback_audio &data) {
        static size_t sample_rate = 0;
        std::vector<float> left;
        std::vector<float> right;
        left.resize(data.samples);
        right.resize(data.samples);
        for (int i = 0; i < data.samples; ++i) {
            left[i] = data.data[i];
            right[i] = data.data[i + data.samples];
        }

        emit raw_samples_ready(Analyzer::average_channels(left, right));

        if (sample_rate not_eq data.sample_rate) {
            sample_rate = data.sample_rate;
            if (_ctx.analyzer) {
                _ctx.analyzer->update_sample_rate(sample_rate);
            }
            emit sample_rate_changed(sample_rate);
        }

        if (_ctx.analyzer) {
            _ctx.analyzer->add_samples(left, right);
            emit samples_ready(_ctx.analyzer->samples.left.get_window());
        }
    });

    _ctx.loopback->start();
    emit loopback_started();
    qCInfo(core_app_main, "Loopback started");
}

void CoreApplication::stop_loopback() const {
    if (_ctx.loopback) {
        _ctx.loopback->set_audio_handler(nullptr);
        _ctx.loopback->stop();
        qCInfo(core_app_main, "Loopback stopped");
    }
}

void CoreApplication::start_wled_audio_reactive_server() {
    if (_ctx.wled_ar_server) {
        qCInfo(core_app_main, "WLED audio server already started");
        return;
    }
    qCInfo(core_app_main, "WLED audio server start");
    const QHostAddress address = get_network_interface_address(_ctx.file_configs.wled_ar_server.network_interface_name);
    _ctx.wled_ar_server = new WLEDAudioReactiveServer(address, _ctx.file_configs.wled_ar_server.port);
    _ctx.wled_ar_server->start();
}

void CoreApplication::stop_wled_audio_reactive_server() const {
    if (not _ctx.wled_ar_server) {
        qCInfo(core_app_main, "WLED audio server already stopped");
        return;
    }
    _ctx.wled_ar_server->stop();
    delete _ctx.wled_ar_server;
    qCInfo(core_app_main, "WLED audio server stop");
}

void CoreApplication::restart_wled_audio_reactive_server() const {
    qCInfo(core_app_main, "WLED audio server restart");
    stop_wled_audio_reactive_server();
    stop_wled_audio_reactive_server();
}

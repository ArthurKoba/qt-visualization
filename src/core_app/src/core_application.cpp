#include <QScreen>
#include <QCommandLineParser>
#include <QCommandLineOption>

#include "core_app/core_application.h"

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
    ;
    if (not _ctx.file_configs.scene_graph.empty()) {
        _ctx.data_flow_graph->load(_ctx.file_configs.scene_graph);
    }

    if (not _ctx.configs.enable_ui) return;
    _main_window = new MainWindow(_ctx);
    connect(_main_window, &MainWindow::on_config_updated, this, [this]() {
        _ctx.file_configs.scene_graph = _ctx.data_flow_graph->save();
        _config_manager.save(_ctx.file_configs);
    });
}

CoreApplication::~CoreApplication() {
    delete _main_window;
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

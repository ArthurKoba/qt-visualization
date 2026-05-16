#include "core_app/main_window.h"

#include <QMenuBar>
#include <QVBoxLayout>
#include <QtWidgets/QStatusBar>
#include <QtCore/QLoggingCategory>
#include <QtGui/QShowEvent>
#include <QApplication>

#include "core_app/core_application.h"
#include "core_app/settings_widget.h"
#include "core_app/scheme_editor/editor_widget.h"
#include "visualization/charts/line.h"
#include "visualization/charts/bar.h"
#include "visualization/charts/freq.h"

Q_LOGGING_CATEGORY(core_app_window, "core_app.window")

MainWindow::MainWindow(CoreApplication &application) : _application(application) {
    setWindowTitle("Data Processing System Core");
    setMinimumSize(800, 600);
    resize(1000, 800);

    setup_ui();
    connect(&_application, &CoreApplication::sample_rate_changed, this, &MainWindow::on_sample_rate_changed);

    connect(&_application, &CoreApplication::raw_samples_ready, this, &MainWindow::on_raw_samples_ready);
    connect(&_application, &CoreApplication::samples_ready, this, &MainWindow::on_samples_ready);
    connect(&_application, &CoreApplication::samples_after_window_function_ready, this, &MainWindow::on_samples_after_window_function_ready);
    connect(&_application, &CoreApplication::window_function_ready, this, &MainWindow::on_window_function_ready);
    connect(&_application, &CoreApplication::amplitudes_ready, this, &MainWindow::on_amplitudes_ready);
    connect(&_application, &CoreApplication::test_amplitudes_ready, this, &MainWindow::on_test_amplitudes_ready);
    connect(&_application, &CoreApplication::spectrogram_updated, this, &MainWindow::on_spectrogram_updated);
}

MainWindow::~MainWindow() = default;

void MainWindow::setup_ui() {
    auto *menu = menuBar()->addMenu("File");
    const auto saveAction = menu->addAction("Save Scene");
    saveAction->setShortcut(QKeySequence::Save);

    connect(saveAction, &QAction::triggered, this, [this]() { emit on_config_updated(); });

    _tab_widget = new QTabWidget(this);

    // Создаем все графики один раз

    // Raw Samples
    _raw_samples_chart = new LineChartView();
    _raw_samples_chart->chart()->setTitle("Raw Samples");
    _raw_samples_chart->set_range(-1, 1);
    _raw_samples_chart->set_auto_resizing(true);
    _raw_samples_chart->set_auto_gain(true, 1, -1);
    _raw_samples_tab_index = _tab_widget->addTab(_raw_samples_chart, "Raw Samples");

    // Samples
    _samples_chart = new LineChartView();
    _samples_chart->chart()->setTitle("Samples");
    _samples_chart->set_range(-1, 1);
    _samples_chart->set_auto_resizing(true);
    _samples_chart->set_auto_gain(true, 1, -1);
    _samples_tab_index = _tab_widget->addTab(_samples_chart, "Samples");

    // Samples after window function
    _samples_after_window_function_chart = new LineChartView();
    _samples_after_window_function_chart->chart()->setTitle("Samples after window function");
    _samples_after_window_function_chart->set_range(-1, 1);
    _samples_after_window_function_chart->set_auto_resizing(true);
    _samples_after_window_function_chart->set_auto_gain(true, 1, -1);
    _samples_after_window_function_tab_index = _tab_widget->addTab(_samples_after_window_function_chart, "Samples after window function");

    //  window function
    _window_function_chart = new LineChartView();
    _window_function_chart->chart()->setTitle("Window function values");
    _window_function_chart->set_range(-0.2, 1.2);
    if (_application._ctx.analyzer and _application._ctx.analyzer->window.size()) {
        _amplitudes_chart->update( _application._ctx.analyzer->window );
    }
    _window_function_tab_index = _tab_widget->addTab(_window_function_chart, "Window function");

    // Amplitudes (FFT)
    _amplitudes_chart = new FreqChartView();
    _amplitudes_chart->chart()->setTitle("Amplitudes FFT");
    _amplitudes_chart->set_range(0, 1);
    _amplitudes_chart->set_auto_resizing(true);
    _amplitudes_chart->set_auto_gain(true, 1, 0);

    _amplitudes_tab_index = _tab_widget->addTab(_amplitudes_chart, "Amplitudes (FFT)");

    // Test Amplitudes (Mel)
    _test_amplitudes_chart = new LineChartView();
    _test_amplitudes_chart->chart()->setTitle("Test Amplitudes (Mel)");
    _test_amplitudes_chart->set_range(0, 1);
    _test_amplitudes_chart->set_auto_resizing(true);
    _test_amplitudes_chart->set_auto_gain(true, 1, 0);
    _test_amplitudes_tab_index = _tab_widget->addTab(_test_amplitudes_chart, "Test Amplitudes (Mel)");

    // 3D Surface
    _surface_view = new SurfaceGraph(this);
    const QSize screenSize = screen()->size();
    const QSize minimumGraphSize{
        screenSize.width() / 2,
        qRound(screenSize.height() / 1.75)
    };
    if (!_surface_view->initialize(minimumGraphSize, screenSize)) {
        qWarning("Couldn't initialize the OpenGL context.");
    }
    _surface_tab_index = _tab_widget->addTab(_surface_view, "3D Surface");

    // Settings
    const auto settings_widget = new SettingsWidget(_application._ctx, this);
    connect(settings_widget, &SettingsWidget::on_config_changed, this, &MainWindow::on_config_updated);
    connect(settings_widget, &SettingsWidget::on_rebuild_charts, this, &MainWindow::on_update_chart_visibility);
    connect(settings_widget, &SettingsWidget::start_loopback_requested, this, &MainWindow::start_loopback_requested);
    connect(settings_widget, &SettingsWidget::stop_loopback_requested, this, &MainWindow::stop_loopback_requested);
    connect(settings_widget, &SettingsWidget::start_analyzer_requested, this, &MainWindow::start_analyzer_requested);
    connect(settings_widget, &SettingsWidget::stop_analyzer_requested, this, &MainWindow::stop_analyzer_requested);
    connect(settings_widget, &SettingsWidget::start_wled_server_requested, this, &MainWindow::start_wled_server_requested);
    connect(settings_widget, &SettingsWidget::stop_wled_server_requested, this, &MainWindow::stop_wled_server_requested);
    connect(settings_widget, &SettingsWidget::wled_server_config_changed, this, &MainWindow::wled_server_config_changed);
    _tab_widget->addTab(settings_widget, "Settings");

    setCentralWidget(_tab_widget);

    // Инициализируем видимость графиков на основе текущего конфига
    update_chart_visibility(_application._ctx.file_configs.ui);

    qCInfo(core_app_window, "UI setup completed");
}

void MainWindow::update_chart_visibility(const UIApplicationConfig &config) {
    _tab_widget->setTabVisible(_raw_samples_tab_index, config.show_raw_samples);
    _tab_widget->setTabVisible(_samples_tab_index, config.show_samples);
    _tab_widget->setTabVisible(_samples_after_window_function_tab_index, config.show_samples_after_window_function);
    _tab_widget->setTabVisible(_window_function_tab_index, config.show_window_function);
    _tab_widget->setTabVisible(_amplitudes_tab_index, config.show_amplitudes);
    _tab_widget->setTabVisible(_test_amplitudes_tab_index, config.show_test_amplitudes);
    _tab_widget->setTabVisible(_surface_tab_index, config.show_surface);

    qCDebug(core_app_window, "Chart visibility updated");
}

void MainWindow::on_update_chart_visibility() {
    update_chart_visibility(_application._ctx.file_configs.ui);
}

void MainWindow::on_raw_samples_ready(const std::vector<float> &data) {
    if (_raw_samples_chart) {
        _raw_samples_chart->update(data);
    }
}

void MainWindow::on_samples_ready(const std::vector<float> &data) {
    if (_samples_chart) {
        _samples_chart->update(data);
    }
}

void MainWindow::on_samples_after_window_function_ready(const std::vector<float> &data) {
    if (_samples_after_window_function_chart) {
        _samples_after_window_function_chart->update(data);
    }
}

void MainWindow::on_window_function_ready(const std::vector<float> &data) {
    if (_window_function_chart) {
        _window_function_chart->update(data);
    }
}

void MainWindow::on_amplitudes_ready(const std::vector<float> &data) {
    if (_amplitudes_chart) {
        reinterpret_cast<FreqChartView *>(_amplitudes_chart)->update_freq_step( _application._ctx.analyzer->get_freq_step() );
        _amplitudes_chart->update(data);
    }
}

void MainWindow::on_test_amplitudes_ready(const std::vector<float> &data) {
    if (_test_amplitudes_chart) {
        _test_amplitudes_chart->update(data);
    }
}

void MainWindow::on_spectrogram_updated(const std::vector<std::vector<float> > &data) const {
    if (_surface_view) {
        _surface_view->update(data);
    }
}

void MainWindow::on_sample_rate_changed(uint32_t sample_rate) {
    if (_amplitudes_chart && _application._ctx.analyzer) {
        auto *freq_chart = reinterpret_cast<FreqChartView *>(_amplitudes_chart);
        freq_chart->update_freq_step(_application._ctx.analyzer->get_freq_step());
    }
    qCDebug(core_app_window, "Sample rate changed: %u Hz", sample_rate);
}

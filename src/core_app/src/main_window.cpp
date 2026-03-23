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
    connect(&_application, &CoreApplication::processed_samples_ready, this, &MainWindow::on_processed_samples_ready);
    connect(&_application, &CoreApplication::amplitudes_ready, this, &MainWindow::on_amplitudes_ready);
    connect(&_application, &CoreApplication::test_amplitudes_ready, this, &MainWindow::on_test_amplitudes_ready);
    connect(&_application, &CoreApplication::spectrogram_updated, this, &MainWindow::on_spectrogram_updated);
}

MainWindow::~MainWindow() {
    for (auto *chart: _chart_views) {
        delete chart;
    }
    _chart_views.clear();
    if (_surface_view) {
        delete _surface_view;
    }
}

void MainWindow::setup_ui() {
    auto *menu = menuBar()->addMenu("File");
    const auto saveAction = menu->addAction("Save Scene");
    saveAction->setShortcut(QKeySequence::Save);

    connect(saveAction, &QAction::triggered, this, [this]() { emit on_config_updated(); });

    _tab_widget = new QTabWidget(this);

    // Вкладка редактора схем
    // _tab_widget->addTab(new SchemeEditorWidget(_application._ctx, this), "Scheme Editor");

    // Вкладка с графиками (контейнер для динамических графиков)
    _charts_splitter = new QSplitter(Qt::Vertical);
    _tab_widget->addTab(_charts_splitter, "Charts");

    // Вкладка настроек
    const auto settings_widget = new SettingsWidget(_application._ctx, this);
    connect(settings_widget, &SettingsWidget::on_config_changed, this, &MainWindow::on_config_updated);
    connect(settings_widget, &SettingsWidget::on_rebuild_charts, this, &MainWindow::on_rebuild_charts);
    connect(settings_widget, &SettingsWidget::start_loopback_requested, this, &MainWindow::start_loopback_requested);
    connect(settings_widget, &SettingsWidget::stop_loopback_requested, this, &MainWindow::stop_loopback_requested);
    connect(settings_widget, &SettingsWidget::start_analyzer_requested, this, &MainWindow::start_analyzer_requested);
    connect(settings_widget, &SettingsWidget::stop_analyzer_requested, this, &MainWindow::stop_analyzer_requested);
    _tab_widget->addTab(settings_widget, "Settings");

    setCentralWidget(_tab_widget);

    // Инициализируем графики на основе текущего конфига
    rebuild_charts_from_config(_application._ctx.file_configs.ui);
}

void MainWindow::rebuild_charts_from_config(const UIApplicationConfig &config) {
    // Очищаем старые графики
    for (auto *chart: _chart_views) {
        chart->setParent(nullptr);
        delete chart;
    }
    _chart_views.clear();

    if (_surface_view) {
        _surface_view->setParent(nullptr);
        delete _surface_view;
        _surface_view = nullptr;
    }

    // Создаем новые графики в зависимости от конфига
    if (config.show_raw_samples) {
        create_chart_tab("raw_samples", "Raw Samples", ChartType::Line);
    }

    if (config.show_samples) {
        create_chart_tab("processed_samples", "Processed Samples", ChartType::Line);
    }

    if (config.show_amplitudes) {
        create_chart_tab("amplitudes", "Amplitudes FFT", ChartType::Frequency);
    }

    if (config.show_test_amplitudes) {
        create_chart_tab("test_amplitudes", "Test Amplitudes (Mel)", ChartType::Line);
    }

    if (config.show_surface) {
        _surface_view = new SurfaceGraph(this);
        const QSize screenSize = screen()->size();
        const QSize minimumGraphSize{
            screenSize.width() / 2,
            qRound(screenSize.height() / 1.75)
        };
        if (!_surface_view->initialize(minimumGraphSize, screenSize)) {
            qWarning("Couldn't initialize the OpenGL context.");
        }
        _charts_splitter->addWidget(_surface_view);
    }

    qCInfo(core_app_window, "Charts rebuilt from config");
}

void MainWindow::create_chart_tab(const QString &chart_name,
                                  const QString &title,
                                  ChartType type) {
    AbstractChartView *chart = nullptr;

    switch (type) {
        case ChartType::Line:
            chart = new LineChartView();
            chart->set_range(-1, 1);
            chart->set_auto_resizing(true);
            chart->set_auto_gain(true, 1, -1);
            break;

        case ChartType::Bar:
            chart = new BarChartView();
            chart->set_range(0, 255);
            chart->set_auto_resizing(true);
            chart->set_auto_gain(true, 255, 0, 2);
            break;

        case ChartType::Frequency:
            chart = new FreqChartView();
            chart->set_range(0, 1);
            chart->set_auto_resizing(true);
            chart->set_auto_gain(true, 1, 0);
            // Обновляем частотный шаг, если analyzer готов
            if (_application._ctx.analyzer) {
                reinterpret_cast<FreqChartView *>(chart)->update_freq_step(
                    _application._ctx.analyzer->get_freq_step()
                );
            }
            break;
    }

    if (chart) {
        chart->chart()->setTitle(title);
        _chart_views[chart_name] = chart;
        _charts_splitter->addWidget(chart);
        qCDebug(core_app_window, "Created chart: %s", qPrintable(chart_name));
    }
}

void MainWindow::remove_chart_tab(const QString &chart_name) {
    auto it = _chart_views.find(chart_name);
    if (it != _chart_views.end()) {
        (*it)->setParent(nullptr);
        delete *it;
        _chart_views.erase(it);
        qCDebug(core_app_window, "Removed chart: %s", qPrintable(chart_name));
    }
}

void MainWindow::on_rebuild_charts() {
    rebuild_charts_from_config(_application._ctx.file_configs.ui);
}

void MainWindow::on_raw_samples_ready(const std::vector<float> &data) {
    auto it = _chart_views.find("raw_samples");
    if (it != _chart_views.end()) {
        it.value()->update(data);
    }
}

void MainWindow::on_processed_samples_ready(const std::vector<float> &data) {
    const auto it = _chart_views.find("processed_samples");
    if (it != _chart_views.end()) {
        std::vector<float> std_data(data.begin(), data.end());
        it.value()->update(std_data);
    }
}

void MainWindow::on_amplitudes_ready(const std::vector<float> &data) {
    const auto it = _chart_views.find("amplitudes");
    if (it != _chart_views.end()) {
        std::vector<float> std_data(data.begin(), data.end());
        it.value()->update(std_data);
    }
}

void MainWindow::on_test_amplitudes_ready(const std::vector<float> &data) {
    const auto it = _chart_views.find("test_amplitudes");
    if (it != _chart_views.end()) {
        std::vector<float> std_data(data.begin(), data.end());
        it.value()->update(std_data);
    }
}

void MainWindow::on_spectrogram_updated(const std::vector<std::vector<float> > &data) const {
    if (_surface_view) {
        _surface_view->update(data);
    }
}

void MainWindow::on_sample_rate_changed(uint32_t sample_rate) {
    auto it = _chart_views.find("amplitudes");
    if (it != _chart_views.end()) {
        auto *freq_chart = reinterpret_cast<FreqChartView *>(it.value());
        if (_application._ctx.analyzer) {
            freq_chart->update_freq_step(_application._ctx.analyzer->get_freq_step());
        }
    }
    qCDebug(core_app_window, "Sample rate changed: %u Hz", sample_rate);
}

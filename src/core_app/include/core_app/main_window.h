#ifndef CORE_APP_MAIN_WINDOW_H
#define CORE_APP_MAIN_WINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QSplitter>
#include <QMap>

#include "core_app/core_application.h"
#include "visualization/abstract/abs.h"
#include "visualization/surfaces/surfacegraph.h"

enum class ChartType {
    Line,
    Bar,
    Frequency
};

class MainWindow final : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(CoreApplication &application);
    ~MainWindow() override;

signals:
    void start_loopback_requested();
    void stop_loopback_requested();
    void start_analyzer_requested();
    void stop_analyzer_requested();
    void on_config_updated();

private slots:
    void on_rebuild_charts();
    void on_raw_samples_ready(const std::vector<float>& data);
    void on_processed_samples_ready(const std::vector<float>& data);
    void on_amplitudes_ready(const std::vector<float>& data);
    void on_test_amplitudes_ready(const std::vector<float>& data);
    void on_spectrogram_updated(const std::vector<std::vector<float>>& data) const;
    void on_sample_rate_changed(uint32_t sample_rate);

private:
    CoreApplication& _application;
    QTabWidget* _tab_widget = nullptr;
    QSplitter* _charts_splitter = nullptr;
    
    QMap<QString, AbstractChartView*> _chart_views;
    SurfaceGraph* _surface_view = nullptr;
    
    void setup_ui();
    void rebuild_charts_from_config(const UIApplicationConfig& config);
    void create_chart_tab(const QString& chart_name, const QString& title, 
                         ChartType type);
    void remove_chart_tab(const QString& chart_name);
};

#endif // CORE_APP_MAIN_WINDOW_H
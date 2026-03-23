#ifndef CORE_APP_SETTINGS_WIDGET_H
#define CORE_APP_SETTINGS_WIDGET_H

#include <QGroupBox>
#include <QVBoxLayout>
#include <QtWidgets/QCheckBox>

#include "core_app/types.h"

class SettingsWidget : public QWidget {
    Q_OBJECT
public:
    explicit SettingsWidget(CoreApplicationContext& context, QWidget* parent = nullptr);

signals:
    void on_config_changed();
    void on_rebuild_charts();
    void start_loopback_requested();
    void stop_loopback_requested();
    void start_analyzer_requested();
    void stop_analyzer_requested();

private:
    CoreApplicationContext& _context;

    QVBoxLayout* _main_layout = nullptr;
    QGroupBox* _core_group = nullptr;
    QVBoxLayout* _core_layout = nullptr;
    QGroupBox* _ui_group = nullptr;
    QVBoxLayout* _ui_layout = nullptr;


    QCheckBox* _enable_loopback_cb = nullptr;
    QCheckBox* _enable_analyzer_cb = nullptr;

    QCheckBox* _enable_autosave_cb = nullptr;
    QCheckBox* _move_to_primary_screen_on_start_cb = nullptr;
    QCheckBox* _show_raw_samples_cb = nullptr;
    QCheckBox* _show_samples_cb = nullptr;
    QCheckBox* _show_samples_after_window_function_cb = nullptr;
    QCheckBox* _show_window_function_cb = nullptr;
    QCheckBox* _show_amplitudes_cb = nullptr;
    QCheckBox* _show_test_amplitudes_cb = nullptr;
    QCheckBox* _show_surface_cb = nullptr;

    void setup_ui();
    void connect_signals();
    void load_current_config() const;
};

#endif // CORE_APP_SETTINGS_WIDGET_H

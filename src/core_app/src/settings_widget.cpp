#include "core_app/settings_widget.h"

#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QtCore/QLoggingCategory>

Q_LOGGING_CATEGORY(core_app_settings, "core_app.settings")

SettingsWidget::SettingsWidget(CoreApplicationContext &context, QWidget *parent)
    : QWidget(parent), _context(context) {
    setup_ui();
    load_current_config();
    connect_signals();
}

void SettingsWidget::setup_ui() {
    _main_layout = new QVBoxLayout(this);
    // Группа управления ядром
    _core_group = new QGroupBox("Core Settings", this);
    _core_layout = new QVBoxLayout(_core_group);

    _enable_loopback_cb = new QCheckBox("Enable Loopback", this);
    _enable_analyzer_cb = new QCheckBox("Enable Analyzer", this);

    _core_layout->addWidget(_enable_loopback_cb);
    _core_layout->addWidget(_enable_analyzer_cb);

    _main_layout->addWidget(_core_group);
    _main_layout->addStretch();

    // Группа управления отображением
    _ui_group = new QGroupBox("UI Settings", this);
    _ui_layout = new QVBoxLayout(_ui_group);

    _move_to_primary_screen_on_start_cb = new QCheckBox("Move windows after start to second screen", this);
    _enable_autosave_cb = new QCheckBox("Autosave enabled", this);
    _show_raw_samples_cb = new QCheckBox("Show Raw Samples", this);
    _show_samples_after_window_function_cb = new QCheckBox("Show Processed Samples", this);
    _show_window_function_cb = new QCheckBox("Show Windows Function", this);
    _show_amplitudes_cb = new QCheckBox("Show Amplitudes (FFT)", this);
    _show_test_amplitudes_cb = new QCheckBox("Show Test Amplitudes (Mel)", this);
    _show_surface_cb = new QCheckBox("Show 3D Surface", this);

    _ui_layout->addWidget(_move_to_primary_screen_on_start_cb);
    _ui_layout->addWidget(_enable_autosave_cb);
    _ui_layout->addWidget(_show_raw_samples_cb);
    _ui_layout->addWidget(_show_samples_after_window_function_cb);
    _ui_layout->addWidget(_show_window_function_cb);
    _ui_layout->addWidget(_show_amplitudes_cb);
    _ui_layout->addWidget(_show_test_amplitudes_cb);
    _ui_layout->addWidget(_show_surface_cb);
    _ui_layout->addStretch();

    _main_layout->addWidget(_ui_group);
    _main_layout->addStretch();
}

void SettingsWidget::load_current_config() const {
    const auto &_current_ui_config = _context.file_configs.ui;
    _enable_loopback_cb->setChecked(_context.file_configs.core.run_loopback);
    _enable_analyzer_cb->setChecked(_context.file_configs.core.run_analyser);

    _enable_autosave_cb->setChecked(_current_ui_config.autosave_enabled);
    _move_to_primary_screen_on_start_cb->setChecked(_current_ui_config.move_to_primary_screen_on_start);

    _show_raw_samples_cb->setChecked(_current_ui_config.show_raw_samples);
    _show_samples_after_window_function_cb->setChecked(_current_ui_config.show_samples_after_window_function);
    _show_window_function_cb->setChecked(_current_ui_config.show_window_function);
    _show_amplitudes_cb->setChecked(_current_ui_config.show_amplitudes);
    _show_test_amplitudes_cb->setChecked(_current_ui_config.show_test_amplitudes);
    _show_surface_cb->setChecked(_current_ui_config.show_surface);
}

void SettingsWidget::connect_signals() {
    // Сигналы управления ядром
    connect(_enable_loopback_cb, &QCheckBox::toggled, this, [this](const bool checked) {
        if (checked and checked not_eq _context.file_configs.core.run_loopback) {
            emit start_loopback_requested();
        } else {
            emit stop_loopback_requested();
        }
        _context.file_configs.core.run_loopback = checked;
        emit on_config_changed();
    });

    connect(_enable_analyzer_cb, &QCheckBox::toggled, this, [this](bool checked) {
        if (checked and checked not_eq _context.file_configs.core.run_analyser) {
            emit start_analyzer_requested();
        } else {
            emit stop_analyzer_requested();
        }
        _context.file_configs.core.run_analyser = checked;
        emit on_config_changed();
    });

    // Сигналы управления отображением

    connect(_move_to_primary_screen_on_start_cb, &QCheckBox::toggled, this, [this](bool checked) {
        _context.file_configs.ui.move_to_primary_screen_on_start = checked;
        emit on_config_changed();
    });

    connect(_enable_autosave_cb, &QCheckBox::toggled, this, [this](bool checked) {
        _context.file_configs.ui.autosave_enabled = checked;
        emit on_config_changed();
    });

    connect(_show_raw_samples_cb, &QCheckBox::toggled, this, [this](bool checked) {
        _context.file_configs.ui.show_raw_samples = checked;
        emit on_config_changed();
        emit on_rebuild_charts();
    });

    connect(_show_samples_cb, &QCheckBox::toggled, this, [this](bool checked) {
        _context.file_configs.ui.show_samples = checked;
        emit on_config_changed();
        emit on_rebuild_charts();
    });

    connect(_show_samples_after_window_function_cb, &QCheckBox::toggled, this, [this](bool checked) {
        _context.file_configs.ui.show_samples_after_window_function = checked;
        emit on_config_changed();
        emit on_rebuild_charts();
    });

    connect(_show_window_function_cb, &QCheckBox::toggled, this, [this](bool checked) {
        _context.file_configs.ui.show_window_function = checked;
        emit on_config_changed();
        emit on_rebuild_charts();
    });

    connect(_show_amplitudes_cb, &QCheckBox::toggled, this, [this](bool checked) {
        _context.file_configs.ui.show_amplitudes = checked;
        emit on_config_changed();
        emit on_rebuild_charts();
    });

    connect(_show_test_amplitudes_cb, &QCheckBox::toggled, this, [this](bool checked) {
        _context.file_configs.ui.show_test_amplitudes = checked;
        emit on_config_changed();
        emit on_rebuild_charts();
    });

    connect(_show_surface_cb, &QCheckBox::toggled, this, [this](bool checked) {
        _context.file_configs.ui.show_surface = checked;
        emit on_config_changed();
        emit on_rebuild_charts();
    });
}

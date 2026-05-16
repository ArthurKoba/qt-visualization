#include "core_app/settings_widget.h"

#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QNetworkInterface>
#include <QUdpSocket>
#include <QtCore/QLoggingCategory>
#include <QSpinBox>
#include <QHBoxLayout>

#include "core/utils.h"

Q_LOGGING_CATEGORY(core_app_settings, "core_app.settings")

SettingsWidget::SettingsWidget(CoreApplicationContext &context, QWidget *parent)
    : QWidget(parent), _context(context) {
    _setup_ui();
    _load_current_config();
    _connect_signals();
}

void SettingsWidget::_setup_ui() {
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
    _show_samples_cb = new QCheckBox("Show Samples", this);
    _show_samples_after_window_function_cb = new QCheckBox("Show Samples after window function", this);
    _show_window_function_cb = new QCheckBox("Show Windows Function", this);
    _show_amplitudes_cb = new QCheckBox("Show Amplitudes (FFT)", this);
    _show_test_amplitudes_cb = new QCheckBox("Show Test Amplitudes (Mel)", this);
    _show_surface_cb = new QCheckBox("Show 3D Surface", this);

    _ui_layout->addWidget(_move_to_primary_screen_on_start_cb);
    _ui_layout->addWidget(_enable_autosave_cb);
    _ui_layout->addWidget(_show_raw_samples_cb);
    _ui_layout->addWidget(_show_samples_cb);
    _ui_layout->addWidget(_show_samples_after_window_function_cb);
    _ui_layout->addWidget(_show_window_function_cb);
    _ui_layout->addWidget(_show_amplitudes_cb);
    _ui_layout->addWidget(_show_test_amplitudes_cb);
    _ui_layout->addWidget(_show_surface_cb);
    _ui_layout->addStretch();

    _main_layout->addWidget(_ui_group);
    _main_layout->addStretch();

    // Группа WLED AR server
    _wled_group = new QGroupBox("WLED AR server", this);
    _wled_layout = new QVBoxLayout(_wled_group);

    _enable_wled_server_cb = new QCheckBox("Enable WLED AR server", this);
    _wled_layout->addWidget(_enable_wled_server_cb);

    // Выбор сетевого адаптера
    auto* adapter_label = new QLabel("Network Adapter:", this);
    _wled_layout->addWidget(adapter_label);

    _network_adapter_combo = new QComboBox(this);
    _wled_layout->addWidget(_network_adapter_combo);

    // Определение интерфейса по умолчанию через тестовое UDP соединение
    const auto defaultInterfaceAddress = get_default_interfaces();
    
    // Заполнение списка сетевых адаптеров
    int defaultIndex = -1;
    int savedIndex = -1;
    int currentIndex = 0;
    const QString saved_interface = _context.file_configs.wled_ar_server.network_interface_name;
    
    const auto networkEntries = get_available_network_interfaces();
    for (const auto&[interface_name, human_readable_name, ip_address] : networkEntries) {
        const QString displayText = QString("%1 (%2)") .arg(human_readable_name) .arg(ip_address.toString());
        _network_adapter_combo->addItem(displayText, interface_name);

        if (not saved_interface.isEmpty() and interface_name == saved_interface) {
            savedIndex = currentIndex;
        }

        if (not defaultInterfaceAddress.isNull() and ip_address == defaultInterfaceAddress) {
            defaultIndex = currentIndex;
        }
        currentIndex++;
    }

    // Установка интерфейса: приоритет у сохраненного, если он доступен
    if (savedIndex >= 0) {
        _network_adapter_combo->setCurrentIndex(savedIndex);
    } else if (defaultIndex >= 0) {
        // Если сохраненный интерфейс недоступен, устанавливаем системный по умолчанию
        _network_adapter_combo->setCurrentIndex(defaultIndex);
        // Обновляем конфигурацию с новым интерфейсом
        _context.file_configs.wled_ar_server.network_interface_name = _network_adapter_combo->itemData(defaultIndex).toString();
    }

    // Порт сервера
    auto* port_layout = new QHBoxLayout();
    auto* port_label = new QLabel("Server Port:", this);
    _port_spinbox = new QSpinBox(this);
    _port_spinbox->setMinimum(1024);
    _port_spinbox->setMaximum(65535);
    _port_spinbox->setValue(_context.file_configs.wled_ar_server.port);
    port_layout->addWidget(port_label);
    port_layout->addWidget(_port_spinbox);
    port_layout->addStretch();
    _wled_layout->addLayout(port_layout);

    _wled_layout->addStretch();

    _main_layout->addWidget(_wled_group);
    _main_layout->addStretch();
}

void SettingsWidget::_load_current_config() const {
    _enable_loopback_cb->setChecked(_context.file_configs.core.run_loopback);
    _enable_analyzer_cb->setChecked(_context.file_configs.core.run_analyser);

    _enable_autosave_cb->setChecked(_context.file_configs.ui.autosave_enabled);
    _move_to_primary_screen_on_start_cb->setChecked(_context.file_configs.ui.move_to_primary_screen_on_start);

    _show_raw_samples_cb->setChecked(_context.file_configs.ui.show_raw_samples);
    _show_samples_cb->setChecked(_context.file_configs.ui.show_samples);
    _show_samples_after_window_function_cb->setChecked(_context.file_configs.ui.show_samples_after_window_function);
    _show_window_function_cb->setChecked(_context.file_configs.ui.show_window_function);
    _show_amplitudes_cb->setChecked(_context.file_configs.ui.show_amplitudes);
    _show_test_amplitudes_cb->setChecked(_context.file_configs.ui.show_test_amplitudes);
    _show_surface_cb->setChecked(_context.file_configs.ui.show_surface);

    // Загрузка настроек WLED AR server
    _enable_wled_server_cb->setChecked(_context.file_configs.wled_ar_server.enabled);
    _port_spinbox->setValue(_context.file_configs.wled_ar_server.port);
}


void SettingsWidget::_connect_signals() {
    // Сигналы управления ядром
    connect(_enable_loopback_cb, &QCheckBox::toggled, this, [this](const bool checked) {
        if (checked and checked not_eq _context.file_configs.core.run_loopback) {
            emit start_loopback_requested();
        } else {
            emit stop_loopback_requested();
        }
        _context.file_configs.core.run_loopback = checked;
        _on_config_change();
    });

    connect(_enable_analyzer_cb, &QCheckBox::toggled, this, [this](const bool checked) {
        if (checked and checked not_eq _context.file_configs.core.run_analyser) {
            emit start_analyzer_requested();
        } else {
            emit stop_analyzer_requested();
        }
        _context.file_configs.core.run_analyser = checked;
        _on_config_change();
    });

    // Сигналы управления отображением

    connect(_move_to_primary_screen_on_start_cb, &QCheckBox::toggled, this, [this](const bool checked) {
        _context.file_configs.ui.move_to_primary_screen_on_start = checked;
        _on_config_change();
    });

    connect(_enable_autosave_cb, &QCheckBox::toggled, this, [this](const bool checked) {
        _context.file_configs.ui.autosave_enabled = checked;
        _on_config_change(true);
    });

    connect(_show_raw_samples_cb, &QCheckBox::toggled, this, [this](const bool checked) {
        _context.file_configs.ui.show_raw_samples = checked;
        _on_config_change();
        emit on_rebuild_charts();
    });

    connect(_show_samples_cb, &QCheckBox::toggled, this, [this](const bool checked) {
        _context.file_configs.ui.show_samples = checked;
        _on_config_change();
        emit on_rebuild_charts();
    });

    connect(_show_samples_after_window_function_cb, &QCheckBox::toggled, this, [this](const bool checked) {
        _context.file_configs.ui.show_samples_after_window_function = checked;
        _on_config_change();
        emit on_rebuild_charts();
    });

    connect(_show_window_function_cb, &QCheckBox::toggled, this, [this](const bool checked) {
        _context.file_configs.ui.show_window_function = checked;
        _on_config_change();
        emit on_rebuild_charts();
    });

    connect(_show_amplitudes_cb, &QCheckBox::toggled, this, [this](const bool checked) {
        _context.file_configs.ui.show_amplitudes = checked;
        _on_config_change();
        emit on_rebuild_charts();
    });

    connect(_show_test_amplitudes_cb, &QCheckBox::toggled, this, [this](const bool checked) {
        _context.file_configs.ui.show_test_amplitudes = checked;
        _on_config_change();
        emit on_rebuild_charts();
    });

    connect(_show_surface_cb, &QCheckBox::toggled, this, [this](const bool checked) {
        _context.file_configs.ui.show_surface = checked;
        _on_config_change();
        emit on_rebuild_charts();
    });

    // Сигналы WLED AR server
    connect(_enable_wled_server_cb, &QCheckBox::toggled, this, [this](const bool checked) {
        _context.file_configs.wled_ar_server.enabled = checked;
        if (checked) {
            emit start_wled_server_requested();
        } else {
            emit stop_wled_server_requested();
        }
        _on_config_change();
    });

    connect(_network_adapter_combo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](const int index) {
        if (index >= 0) {
            _context.file_configs.wled_ar_server.network_interface_name = _network_adapter_combo->itemData(index).toString();
            emit wled_server_config_changed();
            _on_config_change();
        }
    });

    connect(_port_spinbox, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](const int value) {
        _context.file_configs.wled_ar_server.port = static_cast<quint16>(value);
        emit wled_server_config_changed();
        _on_config_change();
    });
}

void SettingsWidget::_on_config_change(const bool force_save) {
    if (_context.file_configs.ui.autosave_enabled or force_save) {
        emit on_config_changed();
    }
}

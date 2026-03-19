#include "core_app/settings_widget.h"
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QFormLayout>

SettingsWidget::SettingsWidget(CoreApplicationContext &ctx, QWidget *parent) : QWidget(parent) {
    auto root_layout = new QVBoxLayout(this);
    root_layout->setContentsMargins(20, 20, 20, 20);
    root_layout->setSpacing(16);

    auto *ui_group = new QGroupBox("UI", this);
    auto *ui_layout = new QFormLayout(ui_group);

    auto primary_screen_checkbox = new QCheckBox("Move window to primary screen on start", this);
    if (ctx.file_configs.ui.move_to_primary_screen_on_start) primary_screen_checkbox->toggle();
    ui_layout->addWidget(primary_screen_checkbox);
    connect(primary_screen_checkbox, &QCheckBox::toggled, this, [this, primary_screen_checkbox, &ctx]() {
        ctx.file_configs.ui.move_to_primary_screen_on_start = primary_screen_checkbox->isChecked();
        emit on_settings_changed();
    });

    auto autosave_checkbox = new QCheckBox("Enable autosave", this);
    if (ctx.file_configs.ui.autosave_enabled) autosave_checkbox->toggle();
    ui_layout->addWidget(autosave_checkbox);

    root_layout->addWidget(ui_group);
    root_layout->addStretch();

    connect(autosave_checkbox, &QCheckBox::toggled, this, [this, autosave_checkbox, &ctx]() {
        ctx.file_configs.ui.autosave_enabled = autosave_checkbox->isChecked();
        emit on_settings_changed();
    });
}

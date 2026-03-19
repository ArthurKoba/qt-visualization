#ifndef CORE_APP_SETTINGS_WIDGET_H
#define CORE_APP_SETTINGS_WIDGET_H

#include <QtWidgets/QCheckBox>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QLabel>

#include "core_app/types.h"

class SettingsWidget final : public QWidget {
    Q_OBJECT

public:
    explicit SettingsWidget(CoreApplicationContext &ctx, QWidget* parent = nullptr);
signals:
    void on_settings_changed();
};

#endif // CORE_APP_SETTINGS_WIDGET_H

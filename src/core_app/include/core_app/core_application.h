#ifndef CORE_APP_CORE_APPLICATION_H
#define CORE_APP_CORE_APPLICATION_H

#include <QtWidgets/QApplication>
#include <QLoggingCategory>
#include <QString>
#include <memory>

#include "core_app/main_window.h"
#include "core_app/config_manager.h"

Q_DECLARE_LOGGING_CATEGORY(core_app_main)

class CoreApplication final : public QApplication {
    Q_OBJECT

public:
    explicit CoreApplication(int32_t argc, char *argv[]);

    ~CoreApplication() override;

    [[nodiscard]] int exec() const;

Q_SIGNALS:
    void on_config_updated();

private:
    CoreApplicationContext _ctx{};
    MainWindow *_main_window = nullptr;
    ConfigManager _config_manager;

    void _parse_command_line();

};

#endif // CORE_APP_CORE_APPLICATION_H

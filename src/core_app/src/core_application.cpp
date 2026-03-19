#include <QScreen>

#include "core_app/core_application.h"

Q_LOGGING_CATEGORY(core_app_main, "core_app.main")


CoreApplication::CoreApplication(int32_t argc, char *argv[]) : QApplication(argc, argv) {
    setApplicationName("Data Processing System Core");
    setApplicationVersion("1.0.0");
    setOrganizationName("Data Processing System");
    qCInfo(core_app_main, "Core application created");

    if (_ctx.configs.enable_ui) {
        _main_window = new MainWindow(_ctx);
    }
}

CoreApplication::~CoreApplication() {
    delete _main_window;
}

int CoreApplication::exec() const {
    // QTimer::singleShot(2000, this, []() {
    //     exit(1);
    // });

    if (_main_window) {
        if (_ctx.configs.ui.move_to_primary_screen_on_start) {
            _main_window->move(primaryScreen()->availableGeometry().center() - _main_window->rect().center());
        }
        _main_window->showNormal();
    };
    return QApplication::exec();
}

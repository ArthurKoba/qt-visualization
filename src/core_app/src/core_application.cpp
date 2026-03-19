#include <QScreen>
#include <QCommandLineParser>
#include <QCommandLineOption>

#include "core_app/core_application.h"

Q_LOGGING_CATEGORY(core_app_main, "core_app.main")


CoreApplication::CoreApplication(int32_t argc, char *argv[]) : QApplication(argc, argv) {
    setApplicationName("Data Processing System Core");
    setApplicationVersion("1.0.0");
    setOrganizationName("Data Processing System");

    _parse_command_line();

    if (not _config_manager.load(_ctx._file_configs)) {
        qCInfo(core_app_main, "Using default configuration");
    }
    _ctx.configs.enable_ui = true; // UI всегда включен для core_app

    if (not _ctx.configs.enable_ui) return;
    _main_window = new MainWindow(_ctx);
    connect(_main_window, &MainWindow::on_config_updated, this, [this]() {
        _config_manager.save(_ctx._file_configs);
    });
}

CoreApplication::~CoreApplication() {
    delete _main_window;
}

void CoreApplication::_parse_command_line() {
    QCommandLineParser parser;
    parser.setApplicationDescription("Data Processing System Core Application");
    parser.addHelpOption();
    parser.addVersionOption();

    const QCommandLineOption config_option(QStringList() << "c" << "config",
                                           "Configuration file path (default: config.json)",
                                           "file", "config.json");
    parser.addOption(config_option);

    parser.process(*this);
    _config_manager.setConfigPath(parser.value(config_option));
}

int CoreApplication::exec() const {
    if (_main_window) {
        if (_ctx._file_configs.ui.move_to_primary_screen_on_start) {
            _main_window->move(primaryScreen()->availableGeometry().center() - _main_window->rect().center());
        }
        _main_window->showNormal();
    }
    return QApplication::exec();
}

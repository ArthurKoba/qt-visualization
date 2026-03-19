#ifndef CORE_APP_MAIN_WINDOW_H
#define CORE_APP_MAIN_WINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QTabWidget>

#include "core_app/types.h"

class MainWindow final : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(CoreApplicationContext &context);
    ~MainWindow() override = default;

signals:
    void on_config_updated();

private:
    CoreApplicationContext& _context;

    QTabWidget* _tab_widget{};
};

#endif // CORE_APP_MAIN_WINDOW_H
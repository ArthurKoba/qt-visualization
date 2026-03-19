#ifndef CORE_APP_CORE_APPLICATION_H
#define CORE_APP_CORE_APPLICATION_H

#include <QtWidgets/QApplication>
#include <qloggingcategory.h>

// #include "aggregator/aggregator_client.h"
#include "core_app/main_window.h"

Q_DECLARE_LOGGING_CATEGORY(core_app_main)



class CoreApplication final : public QApplication {
    Q_OBJECT
public:
    explicit CoreApplication(int32_t argc, char* argv[]);
    ~CoreApplication() override;
    [[nodiscard]] int exec() const;
private:
    CoreApplicationContext _ctx{};
    MainWindow* _main_window = nullptr;
};

#endif // CORE_APP_CORE_APPLICATION_H
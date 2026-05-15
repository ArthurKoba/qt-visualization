#ifndef CORE_APP_CORE_APPLICATION_H
#define CORE_APP_CORE_APPLICATION_H

#include <QtWidgets/QApplication>
#include <QLoggingCategory>
#include <QMainWindow>
#include <QString>

#include "core_app/config_manager.h"

Q_DECLARE_LOGGING_CATEGORY(core_app_main)

class CoreApplication final : public QApplication {
    Q_OBJECT

public:
    CoreApplicationContext _ctx{};

    explicit CoreApplication(int32_t argc, char *argv[]);

    ~CoreApplication() override;

    [[nodiscard]] int exec() const;

public Q_SLOTS:
    void start_analyzer();

    void stop_analyzer() const;

    void start_loopback();

    void stop_loopback() const;

Q_SIGNALS:
    void on_config_updated();

    void raw_samples_ready(const std::vector<float> &samples);

    void samples_ready(const std::vector<float> &samples);

    void samples_after_window_function_ready(const std::vector<float> &samples);

    void window_function_ready(const std::vector<float> &window);

    void amplitudes_ready(const std::vector<float> &amplitudes);

    void test_amplitudes_ready(const std::vector<float> &test_amplitudes);

    void spectrogram_updated(const std::vector<std::vector<float> > &spectrogram);

    void sample_rate_changed(uint32_t sample_rate);

    void analyzer_started();

    void analyzer_stopped();

    void loopback_started();

    void loopback_stopped();

private:
    QMainWindow *_main_window = nullptr;
    ConfigManager _config_manager;

    void _parse_command_line();
};

#endif // CORE_APP_CORE_APPLICATION_H

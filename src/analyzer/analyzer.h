#ifndef QT_VISUALIZATION_ANALYZER_H
#define QT_VISUALIZATION_ANALYZER_H

#include <vector>
#include <functional>
#include <QElapsedTimer>

#include "core/abs_task.h"
#include "fft.h"
#include "audio_window.h"


class Samples {
public:
    AudioWindow left;
    AudioWindow right;

    explicit Samples(size_t size = 0);
};

class Amplitudes {
public:
    std::vector<float> left;
    std::vector<float> right;

    explicit Amplitudes(size_t size = 0);
};

class Analyzer final : public AbstractTask {
public:
    typedef std::function<void(void)> update_handler_t;
protected:
    QElapsedTimer timer;
    FFT fft;
    FFT fft2;
    update_handler_t _handler = nullptr;
    bool _need_update = false;
    size_t _samples_size{};
    size_t _amplitudes_size{};

    uint64_t _task() override;

public:
    Samples samples;
    Amplitudes amplitudes;


    Analyzer();

    void add_samples(const std::vector<float>& left, const std::vector<float> &right);

    void set_update_handler(update_handler_t handler);
};


#endif //QT_VISUALIZATION_ANALYZER_H

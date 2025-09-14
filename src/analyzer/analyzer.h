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
    float _smoothing_percent = 0;
    bool _need_update = false;
    size_t _sample_rate{};
    size_t _samples_size{};
    size_t _amplitudes_size{};
    std::vector<float> _bark_scale;

    uint64_t _task() override;

public:
    Samples samples;
    Amplitudes amplitudes;

    Analyzer();

    void add_samples(const std::vector<float>& left, const std::vector<float> &right);

    void set_update_handler(update_handler_t handler);

    void update_sample_rate(size_t sample_rate);

    float get_freq_step();

    static void generate_bark_scale(float *scale_ptr, size_t scale_size, float frequency_step);
};


#endif //QT_VISUALIZATION_ANALYZER_H

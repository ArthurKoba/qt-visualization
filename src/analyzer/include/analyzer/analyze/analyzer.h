#ifndef QT_VISUALIZATION_ANALYZER_H
#define QT_VISUALIZATION_ANALYZER_H

#include <vector>
#include <functional>
#include <QElapsedTimer>

#include "core/abstract/abs_task.h"
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

class SpectralWhitening
{
public:
    SpectralWhitening(size_t bins = 0)
    {
        prev_max.assign(bins, 0.0f);
    }

    void process(const std::vector<float>& input,
                 std::vector<float>& output)
    {
        const float gamma = 0.9f;

        output.resize(input.size());

        for (size_t k = 0; k < input.size(); ++k)
        {
            float reference = prev_max[k] * gamma;

            float value = input[k] - reference;

            if (value < 0.0f)
                value = 0.0f;

            output[k] = value;

            prev_max[k] = std::max(prev_max[k] * gamma, input[k]);
        }
    }

private:
    std::vector<float> prev_max;
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
    std::vector<float> _volume_scale;

    uint64_t _task() override;

public:
    Samples samples;
    Amplitudes amplitudes;
    Amplitudes amplitudes_test;
    SpectralWhitening whitening;

    Analyzer();

    void add_samples(const std::vector<float>& left, const std::vector<float> &right);

    void set_update_handler(update_handler_t handler);

    void update_sample_rate(size_t sample_rate);

    float get_freq_step();

    static void generate_bark_scale(float *scale_ptr, size_t scale_size, float frequency_step);
    static void generate_volume_scale(float *scale_ptr, size_t scale_size, float frequency_step);
};


#endif //QT_VISUALIZATION_ANALYZER_H

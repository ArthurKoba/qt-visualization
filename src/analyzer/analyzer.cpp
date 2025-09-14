#include "analyzer.h"

#include <QDebug>

Samples::Samples(size_t size) : left(size), right(size) {}

Amplitudes::Amplitudes(size_t size) {
    left.resize(size, 0);
    right.resize(size, 0);
}

uint64_t Analyzer::_task() {
    const float fps_ms_delay = 1000.0 / 45;
    while (_thread_handle) {
        timer.start();
        if (_need_update) {
            std::vector<float> temp_amplitudes;
            temp_amplitudes.resize(amplitudes.left.size(), 0.0f);
            auto status = fft.run(samples.left.get_window(), temp_amplitudes);
            if (_sample_rate and temp_amplitudes.size() == _bark_scale.size()) {
                for (int i = 0; i < temp_amplitudes.size(); ++i) {
                    temp_amplitudes[i] *= _bark_scale[i];
                }
            }
            if (_smoothing_percent > 0 and _smoothing_percent < 100) {
                float new_mul = 100 - _smoothing_percent;
                for (int i = 0; i < amplitudes.left.size(); ++i) {
                    amplitudes.left[i] = (amplitudes.left[i] * _smoothing_percent + temp_amplitudes[i] * new_mul) / 100;
                }
            } else {
                for (int i = 0; i < amplitudes.left.size(); ++i) {
                amplitudes.left[i] = temp_amplitudes[i];
                }
            }
            for (int i = 0; i < amplitudes.left.size(); ++i) {
                if (amplitudes.left[i] < 0 or std::isinf(amplitudes.left[i]) or std::isnan(amplitudes.left[i])) {
                    amplitudes.left[i] = 0;
                }
            }



            if (status not_eq FFT::SUCCESS) {
                qCritical(std::format("Failed run fft. code: {}", int(status)).c_str());
                break;
            }
            if (_handler) {
                _handler();
            }
            _need_update = false;
        }
        auto fft_calc_time_ms = float(timer.elapsed()) / 1000000;
        auto delta = abs(fps_ms_delay - fft_calc_time_ms);
        if (delta > 0) _sleep(delta);
    }
    return 0;
}

Analyzer::Analyzer() {
    _samples_size = 4096;
    _sample_rate = 0;
    _amplitudes_size = _samples_size / 2;
//    _smoothing_percent = 60;

    samples = Samples(_samples_size);
    amplitudes = Amplitudes(_amplitudes_size);
    _bark_scale.resize(_amplitudes_size, 1.0f);

    auto status = fft.init(int(_samples_size), fft.FFT4R);
    if (status not_eq FFT::SUCCESS) {
        qCritical("Failed init fft");
    }

}

void Analyzer::add_samples(const std::vector<float> &left, const std::vector<float> &right) {
    samples.left.append_data(left);
    samples.right.append_data(right);
    _need_update = true;
    if (not _thread_handle) {
        qDebug("Analyzer task not started");
    }
}

void Analyzer::set_update_handler(update_handler_t handler) {
    _handler = std::move(handler);
}

void Analyzer::generate_bark_scale(float *scale_ptr, size_t scale_size, float frequency_step) {
    float base;
    for (int i = 0; i < scale_size; i++) {
        base = static_cast<float>(i) * (frequency_step / 650);
        scale_ptr[i] = 7 * logf(base + sqrtf(1 + base * base));
    }
}

void Analyzer::update_sample_rate(size_t sample_rate) {
    if (sample_rate == _sample_rate) return;
    _sample_rate = sample_rate;
    generate_bark_scale(_bark_scale.data(), _bark_scale.size(), get_freq_step());
}

float Analyzer::get_freq_step() {
    return float(_sample_rate) / float(_samples_size);
}

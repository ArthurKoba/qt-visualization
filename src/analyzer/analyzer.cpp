#include "analyzer.h"

#include <QDebug>

Samples::Samples(size_t size) : left(size), right(size) {}

Amplitudes::Amplitudes(size_t size) {
    left.resize(size, 0);
    right.resize(size, 0);
}

static float hz_to_mel(float f)
{
    return 2595.0f * log10f(1.0f + f / 700.0f);
}

static float mel_to_hz(float m)
{
    return 700.0f * (powf(10.0f, m / 2595.0f) - 1.0f);
}

void fft_to_mel(
        const std::vector<float>& temp_amplitudes,
        size_t sample_rate,
        std::vector<float>& mel_output,
        size_t mel_bands = 40)
{
    mel_output.resize(mel_bands);
    const size_t fft_bins = temp_amplitudes.size();
    const size_t N = fft_bins * 2;

    float f_min = 0.0f;
    float f_max = sample_rate * 0.5f;

    float mel_min = hz_to_mel(f_min);
    float mel_max = hz_to_mel(f_max);

    std::vector<float> mel_points(mel_bands + 2);

    for (size_t i = 0; i < mel_bands + 2; ++i)
    {
        mel_points[i] = mel_min +
                        (mel_max - mel_min) * i / (mel_bands + 1);
    }

    std::vector<int> bin(mel_bands + 2);

    for (size_t i = 0; i < mel_bands + 2; ++i)
    {
        float f = mel_to_hz(mel_points[i]);
        bin[i] = static_cast<int>(floor((N + 1) * f / sample_rate));

    }

    mel_output.assign(mel_bands, 0.0f);

    for (size_t m = 1; m <= mel_bands; ++m)
    {
        int left = bin[m - 1];
        int center = bin[m];
        int right = bin[m + 1];

        float energy = 0.0f;

        for (int k = left; k < center; ++k)
        {
            float w = float(k - left) / (center - left);
            energy += temp_amplitudes[k] * w;
        }

        for (int k = center; k < right; ++k)
        {
            float w = float(right - k) / (right - center);
            energy += temp_amplitudes[k] * w;
        }

        mel_output[m - 1] = energy;

    }
//    float max_val = *std::max_element(mel_output.begin(), mel_output.end());
//
//    for (auto& v : mel_output)
//    {
//        v = 20.0f * log10f((v + 1e-9f) / max_val);
//    }

}



uint64_t Analyzer::_task() {
    const float fps_ms_delay = 1000.0 / 45;
    while (_thread_handle) {
        timer.start();
        if (_need_update) {
            std::vector<float> temp_amplitudes;
            std::vector<float> mel_amplitudes;
            temp_amplitudes.resize(amplitudes.left.size(), 0.0f);
            mel_amplitudes.resize(80, 0.0f);
            auto status = fft.run(samples.left.get_window(), temp_amplitudes);

            if (_sample_rate and temp_amplitudes.size() == _bark_scale.size()) {
                for (int i = 0; i < temp_amplitudes.size(); ++i) {
                    temp_amplitudes[i] *= _bark_scale[i];
                }
            }

            for (int i = 0; i < amplitudes.left.size(); ++i) {
                amplitudes.left[i] = temp_amplitudes[i];
            }


            fft_to_mel(temp_amplitudes,
                       _sample_rate,
                       amplitudes_test.left,
                       150);

            for (int i = 0; i < amplitudes.left.size(); ++i) {
                amplitudes.left[i] *= float(i);
            }

//            whitening.process(mel_amplitudes,
//                              amplitudes_test.left);
//            for (int i = 0; i < amplitudes_test.left.size(); ++i) {
//                amplitudes_test.left[i] = temp_amplitudes[i];
//            }

//            if (_sample_rate and temp_amplitudes.size() == _bark_scale.size()) {
//                for (int i = 0; i < temp_amplitudes.size(); ++i) {
//                    temp_amplitudes[i] *= _bark_scale[i];
//                }
//            }
//            if (_smoothing_percent > 0 and _smoothing_percent < 100) {
//                float new_mul = 100 - _smoothing_percent;
//                for (int i = 0; i < amplitudes.left.size(); ++i) {
//                    amplitudes.left[i] = (amplitudes.left[i] * _smoothing_percent + temp_amplitudes[i] * new_mul) / 100;
//                }
//            } else {
//                for (int i = 0; i < amplitudes.left.size(); ++i) {
//                amplitudes.left[i] = temp_amplitudes[i];
//                }
//            }
//            for (int i = 0; i < amplitudes.left.size(); ++i) {
//                if (amplitudes.left[i] < 0 or std::isinf(amplitudes.left[i]) or std::isnan(amplitudes.left[i])) {
//                    amplitudes.left[i] = 0;
//                }
//            }



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
    whitening = SpectralWhitening(80);
//    _smoothing_percent = 60;

    samples = Samples(_samples_size);
    amplitudes = Amplitudes(_amplitudes_size);
    amplitudes_test = Amplitudes(50);
    _bark_scale.resize(_amplitudes_size, 1.0f);
    _volume_scale.resize(_amplitudes_size, 1.0f);

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
    generate_volume_scale(_volume_scale.data(), _volume_scale.size(), get_freq_step());
}

float Analyzer::get_freq_step() {
    return float(_sample_rate) / float(_samples_size);
}

void Analyzer::generate_volume_scale(float *scale_ptr, size_t scale_size, float frequency_step) {

    for (size_t i = 0; i < scale_size; ++i)
    {
        float f = i * frequency_step;

        if (f <= 0.0f)
        {
            scale_ptr[i] = 0.0f;
            continue;
        }

        float f2 = f * f;

        float num = powf(12200.0f, 2.0f) * f2 * f2;

        float den =
                (f2 + powf(20.6f, 2.0f)) *
                sqrtf((f2 + powf(107.7f, 2.0f)) *
                      (f2 + powf(737.9f, 2.0f))) *
                (f2 + powf(12200.0f, 2.0f));

        float A = 20.0f * log10f(num / den) + 2.0f;

        // перевод dB -> линейный коэффициент
        scale_ptr[i] = powf(10.0f, A / 20.0f);
    }

}

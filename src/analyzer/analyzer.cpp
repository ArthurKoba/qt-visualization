#include "analyzer.h"

#include <QDebug>

Samples::Samples(size_t size) : left(size), right(size) {}

Amplitudes::Amplitudes(size_t size) {
    left.resize(size, 0);
    right.resize(size, 0);
}

uint64_t Analyzer::_task() {
    const float fps_ms_delay = 1000 / 30;
    while (_thread_handle) {
        timer.start();
        if (_need_update) {
            auto status = fft.run(samples.left.get_window(), amplitudes.left);
            if (status not_eq FFT::SUCCESS) {
                qCritical(std::format("Failed run fft. code: {}", int(status)).c_str());
                break;
            }
            if (_handler) {
                _handler();
            }
            _need_update = false;
        }
        auto fft_calc_time_ms = float(timer.nsecsElapsed()) / 1000;
//        qDebug("fft calc time ms: %.2f", fft_calc_time_ms);
        auto delta = fps_ms_delay - fft_calc_time_ms;
        if (delta > 0) _sleep(delta);
    }
    return 0;
}

Analyzer::Analyzer() {
    _samples_size = 4096;
    _amplitudes_size = _samples_size / 2;

    samples = Samples(_samples_size);
    amplitudes = Amplitudes(_amplitudes_size);

    auto status = fft.init(int(_samples_size), fft.FFT4R);
    if (status not_eq FFT::SUCCESS) {
        qCritical("Failed init fft");
    }

}

void Analyzer::add_samples(const std::vector<float>& left, const std::vector<float> &right) {
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
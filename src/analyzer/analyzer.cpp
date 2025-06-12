#include "analyzer.h"

#include <QDebug>

void ValuesContainer::move_and_join(std::vector<float> &values, std::vector<float> &target) {
    if (values.size() < target.size()) {
        size_t count_of_out_of_bounds = target.size() - values.size();
        for (size_t i = 0; i < count_of_out_of_bounds; i++) {
            target[i] = target[i + values.size()];
        }
        for (int i = 0; i < values.size(); ++i) {
            target[count_of_out_of_bounds + i] = values[i];
        }
    } else if (values.size() == target.size()) {
        for (int i = 0; i < target.size(); ++i) {
            target[i] = values[i];
        }
    } else {
        size_t count_of_out_of_bounds = values.size() - target.size();
        for (int i = 0; i < target.size(); ++i) {
            target[i] = values[count_of_out_of_bounds + i];
        }
    }
}

Samples::Samples(size_t size) {
    left.resize(size, 0);
    right.resize(size, 0);
}

Amplitudes::Amplitudes(size_t size) {
    left.resize(size, 0);
    right.resize(size, 0);
}

uint64_t Analyzer::_task() {
    while (_thread_handle) {
        if (not _need_update or not _handler) {
            _sleep(1);
            continue;
        }
        _need_update = false;
        auto status = fft.run(samples.left, amplitudes.left);
        if (status not_eq FFT::SUCCESS) {
            qCritical(std::format("Failed run fft. code: {}", int(status)).c_str());
            break;
        }
        status = fft2.run(samples.left, amplitudes.right);
        if (status not_eq FFT::SUCCESS) {
            qCritical(std::format("Failed run fft2. code: {}", int(status)).c_str());
            break;
        }
        if (_handler) {
            _handler();
        }
    }
    return 0;
}

Analyzer::Analyzer() {
    _samples_size = 4096;
    _amplitudes_size = _samples_size / 2;

    samples = Samples(_samples_size);
    amplitudes = Amplitudes(_amplitudes_size);

    auto status = fft.init(int(_samples_size), fft.FFT2R);
    if (status not_eq FFT::SUCCESS) {
        qCritical("Failed init fft");
    }
    auto status2 = fft2.init(int(_samples_size), fft.FFT4R);
    if (status2 not_eq FFT::SUCCESS) {
        qCritical("Failed init fft");
    }
}

void Analyzer::add_samples(Samples &samples_) {
    Samples::move_and_join(samples_.left, samples.left);
    Samples::move_and_join(samples_.right, samples.right);
    _need_update = true;
    if (not _thread_handle) {
        qDebug("Analyzer task not started");
    }
}

void Analyzer::set_update_handler(update_handler_t handler) {
    _handler = std::move(handler);
}
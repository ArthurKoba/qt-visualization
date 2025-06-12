#ifndef QT_VISUALIZATION_ANALYZER_H
#define QT_VISUALIZATION_ANALYZER_H

#include <vector>
#include <functional>
#include "fft.h"
#include "../core/abs_task.h"


class ValuesContainer {
public:
    static void move_and_join(std::vector<float> &values, std::vector<float> &target);
};

class Samples : public ValuesContainer {
public:
    std::vector<float> left;
    std::vector<float> right;

    explicit Samples(size_t size = 0);
};

class Amplitudes : public ValuesContainer {
public:
    std::vector<float> left;
    std::vector<float> right;

    explicit Amplitudes(size_t size = 0);
};

class Analyzer final : public AbstractTask {
public:
    typedef std::function<void(void)> update_handler_t;
protected:
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

    void add_samples(Samples &samples_);

    void set_update_handler(update_handler_t handler);
};


#endif //QT_VISUALIZATION_ANALYZER_H

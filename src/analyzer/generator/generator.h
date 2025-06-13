#ifndef QT_VISUALIZATION_GENERATOR_H
#define QT_VISUALIZATION_GENERATOR_H

#include <functional>

#include "core/abs_task.h"

class Generator final : public AbstractTask {
    typedef std::function<void(std::vector<float>)> audio_handler_t;
    audio_handler_t _handler;
    size_t _data_size = 512;

    uint64_t _task() override;

public:
    void set_handler(audio_handler_t handler);
};

#endif //QT_VISUALIZATION_GENERATOR_H

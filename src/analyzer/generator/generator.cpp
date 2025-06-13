#include <cmath>

#include "generator.h"

#define _Generator_PI 3.14159265358979323846

uint64_t Generator::_task() {
    size_t index = 0;
    while (_thread_handle) {
        this->_sleep(15);
        index++;
        if (_handler) {
            std::vector<float> data;
            data.reserve(_data_size);
            float value;
            for (int i = 0; i < _data_size; ++i) {
                value = std::sin(float(i + index) * _Generator_PI / 20 );
                data.push_back(value);
            }
//            index += 1;
            index += _data_size;
            _handler(data);
        }
    }
    return 0;
}

void Generator::set_handler(audio_handler_t handler) {
    _handler = handler;
}

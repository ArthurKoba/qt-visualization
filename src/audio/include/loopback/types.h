#ifndef QT_VISUALIZATION_AUDIO_LOOPBACK_INTERFACE_H
#define QT_VISUALIZATION_AUDIO_LOOPBACK_INTERFACE_H

#include <cstdint>
#include <functional>

namespace audio::loopback {
    typedef struct {
        float *data;
        size_t samples;
        size_t channels;
    } loopback_audio;

    typedef std::function<void(loopback_audio)> audio_handler_t;

    class IAudioLoopback {
    public:
        virtual void set_audio_handler(audio_handler_t handler) = 0;
        virtual void start() = 0;
        virtual void stop() = 0;
        ~IAudioLoopback() = default;
    };
}

#endif //QT_VISUALIZATION_AUDIO_LOOPBACK_INTERFACE_H

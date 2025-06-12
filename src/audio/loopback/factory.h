#ifndef QT_VISUALIZATION_AUDIO_LOOPBACK_FACTORY_H
#define QT_VISUALIZATION_AUDIO_LOOPBACK_FACTORY_H

#include "types.h"
#include "windows_loopback.h"

namespace audio::loopback {
    class LoopbackFactory {
    public:
        static IAudioLoopback* get_loopback();
    };
}


#endif //QT_VISUALIZATION_AUDIO_LOOPBACK_FACTORY_H

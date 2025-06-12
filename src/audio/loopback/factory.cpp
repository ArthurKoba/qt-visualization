#include "factory.h"

#ifdef WINDOWS_AUDIO_LOOPBACK
#include "windows_loopback.h"
#endif

using namespace audio::loopback;

IAudioLoopback *LoopbackFactory::get_loopback() {
    return new WASAPILoopback();
}

#ifndef QT_VISUALIZATION_WINDOWS_LOOPBACK_H
#define QT_VISUALIZATION_WINDOWS_LOOPBACK_H

#include <audioclient.h>
#include <mmdeviceapi.h>
#include <string>

#include "core/abs_task.h"
#include "loopback/types.h"

namespace audio::loopback {

class WASAPILoopback final : public virtual IAudioLoopback, public AbstractTask {
    private:
        bool need_stop = false;

        IAudioClient *pAudioClient = nullptr;
        IAudioCaptureClient *pCaptureClient = nullptr;
        IMMDevice *pDevice = nullptr;
        IMMDeviceEnumerator *pEnumerator = nullptr;
        WAVEFORMATEX *pwfx = nullptr;

        audio_handler_t _handler = nullptr;

        HRESULT _show_error_and_return(std::string msg, HRESULT result);
    protected:
        uint64_t _before_run_task() override;

        uint64_t _task() override;

        void _after_close_task() override;

        void _handle_audio(float *samples, size_t count);

        int _init();

        void _reset();

    public:
        void set_audio_handler(audio_handler_t handler) override;
        void start() override;
        void stop() override;
        ~WASAPILoopback();
    };
}
#endif //QT_VISUALIZATION_WINDOWS_LOOPBACK_H

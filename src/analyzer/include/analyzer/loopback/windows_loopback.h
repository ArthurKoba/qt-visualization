#ifndef QT_VISUALIZATION_WINDOWS_LOOPBACK_H
#define QT_VISUALIZATION_WINDOWS_LOOPBACK_H

#include <functional>
#include <audioclient.h>
#include <mmdeviceapi.h>
#include <string>

#include <QLoggingCategory>

#include "core/abstract/abs_task.h"




namespace audio::loopback {
    typedef struct {
        size_t sample_rate;
        float *data;
        size_t samples;
        size_t channels;
    } loopback_audio;

    typedef std::function<void(loopback_audio)> audio_handler_t;


    class WASAPILoopback final : public AbstractTask {
    public:
        void set_audio_handler(const audio_handler_t &handler);

        void start() override;

        void stop() override;

        ~WASAPILoopback() override;

    private:
        bool need_stop = false;

        IAudioClient *pAudioClient = nullptr;
        IAudioCaptureClient *pCaptureClient = nullptr;
        IMMDevice *pDevice = nullptr;
        IMMDeviceEnumerator *pEnumerator = nullptr;
        WAVEFORMATEX *pwfx = nullptr;

        audio_handler_t _handler = nullptr;

        static HRESULT _show_error_and_return(const std::string& msg, HRESULT result);

    protected:
        uint64_t _before_run_task() override;

        uint64_t _task() override;

        void _after_close_task() override;

        void _handle_audio(const float *samples, size_t count, size_t sample_rate) const;

        int _init();

        void _reset();
    };
}
#endif //QT_VISUALIZATION_WINDOWS_LOOPBACK_H

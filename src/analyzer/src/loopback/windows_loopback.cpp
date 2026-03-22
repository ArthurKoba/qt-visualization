#include "analyzer/loopback/windows_loopback.h"

#include <thread>
#include <iostream>


#include <initguid.h>  // Для DEFINE_GUID
#include <valarray>

#ifndef KSDATAFORMAT_SUBTYPE_IEEE_FLOAT_
DEFINE_GUID(KSDATAFORMAT_SUBTYPE_IEEE_FLOAT_,
            0x00000003, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71);
#endif

#define REFTIMES_PER_SEC  10000000


Q_LOGGING_CATEGORY(wasapi, "wasapi_loopback")

using namespace audio::loopback;


void WASAPILoopback::set_audio_handler(const audio_handler_t &handler) {
    _handler = handler;
}

void WASAPILoopback::start() {
    need_stop = false;
    AbstractTask::start();
}

void WASAPILoopback::stop() {
    need_stop = true;
}

void WASAPILoopback::_after_close_task() {
    if (not pAudioClient) return;
    const HRESULT hr = pAudioClient->Stop();
    if (FAILED(hr)) {
        qCCritical(wasapi, "IAudioClient::Stop failed: hr = 0x%08lx\n", hr);
    }
    _reset();
}

uint64_t WASAPILoopback::_before_run_task() {
    const auto result = _init();
    if (result not_eq 0) {
        _reset();
    }
    return result;
}

void WASAPILoopback::_handle_audio(const float *samples, const size_t count, const size_t sample_rate) const {
    if (not _handler) return;
    loopback_audio audio_data;
    audio_data.sample_rate = sample_rate;
    audio_data.channels = pwfx->nChannels;
    audio_data.data = new float[count * pwfx->nChannels];
    audio_data.samples = count;
    if (samples) {
        for (size_t ch = 0; ch < pwfx->nChannels; ++ch) {
            size_t offset = ch * count;
            for (int i = 0; i < count; ++i) {
                audio_data.data[offset + i] = samples[i * pwfx->nChannels + ch];
            }
        }
    } else {
        for (int i = 0; i < count * pwfx->nChannels; ++i) {
            audio_data.data[i] = 0.0f;
        }
    }
    _handler(audio_data);
    delete[] audio_data.data;
}

uint64_t WASAPILoopback::_task() {
    UINT32 packet_size;
    uint8_t *data;
    uint32_t frames;
    DWORD flags;

    while (_thread_handle) {
        if (need_stop) {
            need_stop = false;
            return 0;
        }
        HRESULT hr = pCaptureClient->GetNextPacketSize(&packet_size);
        if (FAILED(hr)) {
            return _show_error_and_return("IAudioCaptureClient::GetNextPacketSize failed: hr = 0x%08lx\n", hr);
        }
        while (packet_size) {
            hr = pCaptureClient->GetBuffer(&data, &frames, &flags, nullptr, nullptr);

            if (FAILED(hr)) {
                return _show_error_and_return("IAudioCaptureClient::GetBuffer failed: hr = 0x%08lx\n", hr);
            }

            if (flags & AUDCLNT_BUFFERFLAGS_SILENT) {
                data = nullptr; // Буфер молчания
            }
            if (frames) {
                _handle_audio(reinterpret_cast<float *>(data), frames, size_t(pwfx->nSamplesPerSec));
            }

            hr = pCaptureClient->ReleaseBuffer(frames);
            if (FAILED(hr)) {
                return _show_error_and_return("IAudioCaptureClient::ReleaseBuffer failed: hr = 0x%08lx\n", hr);
            }
        }
    }
    return 1;
}


int WASAPILoopback::_init() {
    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    if (FAILED(hr)) {
        return _show_error_and_return("CoInitializeEx failed: hr = 0x%08lx\n", hr);
    }

    hr = CoCreateInstance(
        ::__uuidof(MMDeviceEnumerator), nullptr,
        CLSCTX_ALL, ::__uuidof(IMMDeviceEnumerator),
        reinterpret_cast<void **>(&pEnumerator));
    if (FAILED(hr)) {
        return _show_error_and_return("CoCreateInstance failed: hr = 0x%08lx\n", hr);
    }
    hr = CoCreateInstance(
        __uuidof(MMDeviceEnumerator), nullptr,
        CLSCTX_ALL, ::__uuidof(IMMDeviceEnumerator),
        reinterpret_cast<void **>(&pEnumerator));
    if (FAILED(hr)) {
        return _show_error_and_return("CoCreateInstance failed: hr = 0x%08lx\n", hr);
    }

    hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);
    if (FAILED(hr)) {
        return _show_error_and_return("GetDefaultAudioEndpoint failed: hr = 0x%08lx\n", hr);
    }

    hr = pDevice->Activate(::__uuidof(IAudioClient), CLSCTX_ALL, nullptr, reinterpret_cast<void **>(&pAudioClient));
    if (FAILED(hr)) {
        return _show_error_and_return("IMMDevice::Activate failed: hr = 0x%08lx\n", hr);
    }

    hr = pAudioClient->GetMixFormat(&pwfx);
    if (FAILED(hr)) {
        return _show_error_and_return("IAudioClient::GetMixFormat failed: hr = 0x%08lx\n", hr);
    }
    if (pwfx->wFormatTag not_eq WAVE_FORMAT_EXTENSIBLE) {
        return _show_error_and_return("wave format is not WAVE_FORMAT_EXTENSIBLE. format id: %d\n", pwfx->wFormatTag);
    }
    auto *pwfex = reinterpret_cast<WAVEFORMATEXTENSIBLE *>(pwfx);
    if (not IsEqualGUID(pwfex->SubFormat, KSDATAFORMAT_SUBTYPE_IEEE_FLOAT_)) {
        return _show_error_and_return("WAVEFORMATEXTENSIBLE used not IEEE_FLOAT. used: %d\n", pwfx->wFormatTag);
    }

    if (pwfx->wFormatTag not_eq WAVE_FORMAT_EXTENSIBLE) {
        return _show_error_and_return("unknown wave format: %d\n", pwfx->wFormatTag);
    }

    hr = pAudioClient->Initialize(
        AUDCLNT_SHAREMODE_SHARED,
        AUDCLNT_STREAMFLAGS_LOOPBACK,
        REFTIMES_PER_SEC,
        0,
        pwfx,
        nullptr);
    if (FAILED(hr)) {
        return _show_error_and_return("IAudioClient::Initialize failed: hr = 0x%08lx\n", hr);
    }

    hr = pAudioClient->GetService(::__uuidof(IAudioCaptureClient), reinterpret_cast<void **>(&pCaptureClient));
    if (FAILED(hr)) {
        return _show_error_and_return("IAudioClient::GetService failed: hr = 0x%08lx\n", hr);
    }
    hr = pAudioClient->Start();
    if (FAILED(hr)) {
        return _show_error_and_return("IAudioClient::Start failed: hr = 0x%08lx\n", hr);

    }
    return 0;
}

HRESULT WASAPILoopback::_show_error_and_return(const std::string &msg, const HRESULT result) {
    qCCritical(wasapi, msg.c_str(), result);
    return result;
}

void WASAPILoopback::_reset() {
    if (pCaptureClient) {
        pCaptureClient->ReleaseBuffer(0);
        pCaptureClient->Release();
        pCaptureClient = nullptr;
    }
    if (pwfx) {
        CoTaskMemFree(pwfx);
        pwfx = nullptr;
    }
    if (pAudioClient) {
        pAudioClient->Release();
        pAudioClient = nullptr;
    }

    if (pDevice) {
        pDevice->Release();
        pDevice = nullptr;
    }

    if (pEnumerator) {
        pEnumerator->Release();
        pEnumerator = nullptr;
    }
    CoUninitialize();
}

WASAPILoopback::~WASAPILoopback() {
    _reset();
}

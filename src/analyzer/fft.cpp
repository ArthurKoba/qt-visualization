#include "fft.h"
#include <QDebug>


FFT::fft_status_t FFT::init(int samples_size, fft_type_t type) {
    _type = type;

    esp_err_t ret = ESP_OK;
    if (_type == FFT2R) {
        ret = dsps_fft2r_init_fc32(nullptr, samples_size);
    } else if (_type == FFT4R) {
        ret = dsps_fft4r_init_fc32(nullptr, samples_size);
    }

    if (ret not_eq ESP_OK) {
        qCritical(std::format("Not possible to initialize FFT2R. Error = {}", ret).c_str());
        return FFT::NOT_INIT;
    }
//    dsps_wind_hann_f32(wind, samples_size);
    _samples_size = samples_size;

    _buffer.resize(_samples_size * 2, 0);

    return FFT::SUCCESS;
}

FFT::fft_status_t FFT::run(std::vector<float> &samples, std::vector<float> &amplitudes) {
//    qDebug("samples size: %i, amplitudes size: %i\n", samples.size(), amplitudes.size());
    if (_samples_size == 0) return FFT::NOT_INIT;
    if (samples.size() not_eq _samples_size or amplitudes.size() not_eq _samples_size >> 1) return FFT::INCORRECT_CONFIGS;
    for (int i = 0; i < _samples_size; i++) {
        _buffer[i * 2 + 0] = samples[i];
//        if (_cfg.window_type not_eq NO_WINDOW) {
//            _buffer[i * 2 + 0] *= _fft_window[i];
//        }
        _buffer[i * 2 + 1] = 0;
    }
    esp_err_t ret;

    if (_type == FFT2R) {
        ret = dsps_fft2r_fc32_ansi_(_buffer.data(), _samples_size, dsps_fft_w_table_fc32);
        if (ret not_eq ESP_OK) return FFT::INCORRECT_CONFIGS;
        ret = dsps_bit_rev_fc32_ansi(_buffer.data(), _samples_size);
        if (ret not_eq ESP_OK) return FFT::INCORRECT_CONFIGS;
        ret = dsps_cplx2reC_fc32_ansi(_buffer.data(), _samples_size);
        if (ret not_eq ESP_OK) return FFT::INCORRECT_CONFIGS;
    } else if (_type == FFT4R) {
        ret = dsps_fft4r_fc32_ansi_(_buffer.data(), _samples_size, dsps_fft4r_w_table_fc32, dsps_fft4r_w_table_size);
        if (ret not_eq ESP_OK) return FFT::INCORRECT_CONFIGS;
        ret = dsps_bit_rev4r_fc32(_buffer.data(), _samples_size);
        if (ret not_eq ESP_OK) return FFT::INCORRECT_CONFIGS;
        ret = dsps_cplx2real_fc32_ansi_(_buffer.data(), _samples_size, dsps_fft4r_w_table_fc32, dsps_fft4r_w_table_size);
        if (ret not_eq ESP_OK) return FFT::INCORRECT_CONFIGS;
    }



    float temp;
    for (int i = 0 ; i < amplitudes.size() ; i++) {
        temp = _buffer[i * 2 + 0] * _buffer[i * 2 + 0] + _buffer[i * 2 + 1] * _buffer[i * 2 + 1];
        amplitudes[i] = 2 * sqrtf(temp) / float(_samples_size);
//        switch(_cfg.amplitudes_type) {
//            case BARK: amplitudes[i] *= _bark_scale[i]; break;
//            case LOG : amplitudes[i] = 10 * log10f(temp / _samples_size); break;
//            default: break;
//        }
    }

    return FFT::SUCCESS;
}

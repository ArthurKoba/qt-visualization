#include "fft.h"
#include <QDebug>

#include "./dsp/fft/include/dsps_fft2r.h"
#include "./dsp/fft/include/dsps_fft4r.h"

FFT::fft_status_t FFT::init(int samples_size, fft_type_t type) {
    _type = type;

    esp_err_t ret = ESP_OK;
    if (_type == FFT2R) {
        ret = dsps_fft2r_init_fc32(nullptr, samples_size);
    } else if (_type == FFT4R) {
        ret = dsps_fft4r_init_fc32(nullptr, samples_size);
    } else if (_type == FHT2R) {
        ret = dsps_fft2r_init_fc32(nullptr, samples_size);
    } else if (_type == FHT4R) {
        ret = dsps_fft4r_init_fc32(nullptr, samples_size);
    }

    if (ret not_eq ESP_OK) {
        qCritical(std::format("Not possible to initialize FFT2R. Error = {}", ret).c_str());
        return FFT::NOT_INIT;
    }

    _samples_size = samples_size;
    _buffer.resize(_samples_size * 2, 0);
    return FFT::SUCCESS;
}

FFT::fft_status_t FFT::run(std::vector<float> &samples, std::vector<float> &amplitudes) {
    if (_samples_size == 0) {
        return FFT::NOT_INIT;
    }
    if (samples.size() not_eq _samples_size or amplitudes.size() not_eq _samples_size / 2) {
        return FFT::INCORRECT_CONFIGS;
    }

    if (_type == FFT2R or _type == FFT4R) {
        float sample;
        for (int i = 0; i < _samples_size; i++) {
            sample = samples[i];
            _buffer[i * 2 + 0] = sample;
            _buffer[i * 2 + 1] = 0;
        }
    } else if (_type == FHT2R or _type == FHT4R) {
        float sample;
        for (int i = 0; i < _samples_size; i++) {
            sample = samples[i];
            _buffer[i * 2 + 0] = sample;
            _buffer[i * 2 + 1] = 0;
        }
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
    } else if (_type == FHT2R) {
        ret = dsps_fft2r_fc32_ansi_(_buffer.data(), _samples_size, dsps_fft_w_table_fc32);
//        ret = dsps_fht2r_fc32_ansi_(_buffer.data(), _samples_size, dsps_fft_w_table_fc32);
        if (ret not_eq ESP_OK) return FFT::INCORRECT_CONFIGS;
        ret = dsps_bit_rev_fc32_ansi(_buffer.data(), _samples_size);
        if (ret not_eq ESP_OK) return FFT::INCORRECT_CONFIGS;
        ret = dsps_cplx2reC_fc32_ansi(_buffer.data(), _samples_size);
        if (ret not_eq ESP_OK) return FFT::INCORRECT_CONFIGS;
    } else if (_type == FHT4R) {
        ret = dsps_fft4r_fc32_ansi_(_buffer.data(), _samples_size, dsps_fft4r_w_table_fc32, dsps_fft4r_w_table_size);
//        ret = dsps_fht4r_fc32_ansi_(_buffer.data(), _samples_size, dsps_fft4r_w_table_fc32, dsps_fft4r_w_table_size);
        if (ret not_eq ESP_OK) return FFT::INCORRECT_CONFIGS;
        ret = dsps_bit_rev4r_fc32(_buffer.data(), _samples_size);
        if (ret not_eq ESP_OK) return FFT::INCORRECT_CONFIGS;
        ret = dsps_cplx2real_fc32_ansi_(_buffer.data(), _samples_size, dsps_fft4r_w_table_fc32, dsps_fft4r_w_table_size);
        if (ret not_eq ESP_OK) return FFT::INCORRECT_CONFIGS;
    }


    if (_type == FFT2R or _type == FFT4R) {
        float temp;
        for (int i = 0 ; i < amplitudes.size() ; i++) {
            temp = _buffer[i * 2 + 0] * _buffer[i * 2 + 0] + _buffer[i * 2 + 1] * _buffer[i * 2 + 1];
            amplitudes[i] = 2 * sqrtf(temp) / float(_samples_size);
        }
    }

    if (_type == FHT2R or _type == FHT4R) {
        float temp;
        for (int i = 0 ; i < amplitudes.size() ; i++) {
            temp = _buffer[i * 2 + 0] - _buffer[i * 2 + 1];
            amplitudes[i] = abs(temp);
        }
    }
    return FFT::SUCCESS;
}

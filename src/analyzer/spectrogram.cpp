#include "spectrogram.h"

#include <algorithm>

// ─────────────────────────────────────────────────────────────────────────────
// Construction
// ─────────────────────────────────────────────────────────────────────────────

Spectrogram::Spectrogram(std::size_t history_size)
        : history_size_(history_size > 0 ? history_size : 1),
          channels_(0),
          write_index_(0),
          filled_count_(0)
{}

// ─────────────────────────────────────────────────────────────────────────────
// Public interface
// ─────────────────────────────────────────────────────────────────────────────

bool Spectrogram::push(const std::vector<float>& spectrum) {
    if (spectrum.empty()) {
        return false;
    }

    // Если число каналов изменилось — полный сброс
    if (spectrum.size() != channels_) {
        reset(spectrum.size());
    }

    for (std::size_t ch = 0; ch < channels_; ++ch) {
        data_[ch][write_index_] = spectrum[ch];
    }

    write_index_ = (write_index_ + 1) % history_size_;
    if (filled_count_ < history_size_) {
        ++filled_count_;
    }

    return true;
}

bool Spectrogram::set_history_size(std::size_t new_size) {
    if (new_size == 0 || new_size == history_size_) {
        return new_size != 0;
    }

    // Сначала разворачиваем текущие данные в линейный порядок (старые→новые)
    const std::size_t old_filled = filled_count_;
    std::vector<std::vector<float>> linear(channels_, std::vector<float>(old_filled, 0.0f));

    for (std::size_t ch = 0; ch < channels_; ++ch) {
        unwrap_channel(ch, linear[ch]);
    }

    history_size_ = new_size;
    write_index_  = 0;
    filled_count_ = 0;

    // Пересоздаём буферы под новый размер истории, заполняем нулями
    data_.assign(channels_, std::vector<float>(history_size_, 0.0f));

    if (new_size >= old_filled) {
        // Увеличение: данные помещаются целиком, начало (старые слоты) — нули
        const std::size_t offset = new_size - old_filled;
        for (std::size_t ch = 0; ch < channels_; ++ch) {
            for (std::size_t t = 0; t < old_filled; ++t) {
                data_[ch][offset + t] = linear[ch][t];
            }
        }
        write_index_  = 0;            // следующая запись снова в начало (кольцо заполнено)
        filled_count_ = new_size;
    } else {
        // Уменьшение: берём только new_size самых новых срезов
        const std::size_t discard = old_filled - new_size;
        for (std::size_t ch = 0; ch < channels_; ++ch) {
            for (std::size_t t = 0; t < new_size; ++t) {
                data_[ch][t] = linear[ch][discard + t];
            }
        }
        write_index_  = 0;
        filled_count_ = new_size;
    }

    return true;
}

std::vector<float> Spectrogram::get_channel_history(std::size_t channel_index) const {
    if (channel_index >= channels_) {
        return {};
    }

    std::vector<float> result(history_size_, 0.0f);
    unwrap_channel(channel_index, result);
    return result;
}

std::vector<std::vector<float>> Spectrogram::get_spectrogram() const {
    std::vector<std::vector<float>> result(channels_);
    for (std::size_t ch = 0; ch < channels_; ++ch) {
        result[ch] = get_channel_history(ch);
    }
    return result;
}

std::vector<float> Spectrogram::get_channel_history_reversed(std::size_t channel_index) const {
    if (channel_index >= channels_) {
        return {};
    }

    std::vector<float> result = get_channel_history(channel_index);
    std::reverse(result.begin(), result.end());
    return result;
}

std::vector<std::vector<float>> Spectrogram::get_spectrogram_reversed() const {
    std::vector<std::vector<float>> result(channels_);
    for (std::size_t ch = 0; ch < channels_; ++ch) {
        result[ch] = get_channel_history_reversed(ch);
    }
    return result;
}

// ─────────────────────────────────────────────────────────────────────────────
// Accessors
// ─────────────────────────────────────────────────────────────────────────────

std::size_t Spectrogram::history_size() const { return history_size_; }
std::size_t Spectrogram::channels()     const { return channels_; }
std::size_t Spectrogram::filled_count() const { return filled_count_; }
bool        Spectrogram::is_full()      const { return filled_count_ == history_size_; }

// ─────────────────────────────────────────────────────────────────────────────
// Private helpers
// ─────────────────────────────────────────────────────────────────────────────

void Spectrogram::reset(std::size_t new_channels) {
    channels_     = new_channels;
    write_index_  = 0;
    filled_count_ = 0;
    data_.assign(channels_, std::vector<float>(history_size_, 0.0f));
}

void Spectrogram::unwrap_channel(std::size_t channel_index, std::vector<float>& output) const {
    // Буфер не полный: данные лежат с позиции 0 до filled_count_ - 1
    // в хронологическом порядке, остаток — нули.
    if (filled_count_ < history_size_) {
        const std::size_t zeros = history_size_ - filled_count_;
        for (std::size_t i = 0; i < zeros; ++i) {
            output[i] = 0.0f;
        }
        for (std::size_t i = 0; i < filled_count_; ++i) {
            output[zeros + i] = data_[channel_index][i];
        }
        return;
    }

    // Буфер полный: write_index_ указывает на самый старый элемент
    for (std::size_t i = 0; i < history_size_; ++i) {
        output[i] = data_[channel_index][(write_index_ + i) % history_size_];
    }
}
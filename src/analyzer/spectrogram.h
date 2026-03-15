#pragma once

#include <cstddef>
#include <vector>

class Spectrogram {
public:
    // history_size — максимальное число хранимых временных срезов
    explicit Spectrogram(std::size_t history_size = 100);

    // Добавить новый срез данных.
    // Если длина среза изменилась — производится resize всей спектрограммы.
    // Возвращает false, если spectrum пуст.
    bool push(const std::vector<float>& spectrum);

    // Изменить глубину истории без сброса данных:
    //   - увеличение: добавляет нули в начало (старые позиции)
    //   - уменьшение: отбрасывает наиболее старые срезы
    // Возвращает false, если new_size == 0.
    bool set_history_size(std::size_t new_size);

    // Получить временной ряд одного канала [0..channels-1], от старых к новым.
    // Если channel_index >= channels(), возвращает пустой вектор.
    std::vector<float> get_channel_history(std::size_t channel_index) const;

    // Получить всю спектрограмму: [channel][time], от старых к новым.
    std::vector<std::vector<float>> get_spectrogram() const;

    // Получить временной ряд одного канала [0..channels-1], от новых к старым.
    // Если channel_index >= channels(), возвращает пустой вектор.
    std::vector<float> get_channel_history_reversed(std::size_t channel_index) const;

    // Получить всю спектрограмму: [channel][time], от новых к старым.
    std::vector<std::vector<float>> get_spectrogram_reversed() const;

    std::size_t history_size()  const;  // максимальный размер истории
    std::size_t channels()      const;  // число каналов (длина последнего среза)
    std::size_t filled_count()  const;  // реально заполненных срезов
    bool        is_full()       const;

private:
    // Пересоздать data_ под новое число каналов, данные сбрасываются в ноль.
    void reset(std::size_t new_channels);

    // Скопировать данные из кольцевого буфера в линейный порядок (старые→новые)
    // для одного канала в output[0..history_size_-1].
    void unwrap_channel(std::size_t channel_index, std::vector<float>& output) const;

    std::size_t history_size_;    // глубина истории
    std::size_t channels_;        // число каналов
    std::size_t write_index_;     // следующая позиция записи в кольцевом буфере
    std::size_t filled_count_;    // сколько позиций реально заполнено

    // Транспонированное хранение: data_[channel][time_slot]
    std::vector<std::vector<float>> data_;
};
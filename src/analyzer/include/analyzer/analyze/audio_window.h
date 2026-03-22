#ifndef ARD_AUDIO_WINDOW_H
#define ARD_AUDIO_WINDOW_H

#include <vector>
#include <cstdint>

class AudioWindow {
private:
    std::vector<float> window;
public:
    explicit AudioWindow(size_t size);

    void append_data(const std::vector<float> &newData);
    void append_data(const float *data, size_t dataSize);
    void clear();
    std::vector<float> &get_window();
    size_t size() const;

};

#endif //ARD_AUDIO_WINDOW_H

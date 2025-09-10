#include "audio_window.h"

AudioWindow::AudioWindow(size_t size) : window(size, 0) {

}

void AudioWindow::append_data(const std::vector<float> &newData) {
    if (newData.empty()) {
        return;
    }
    if (newData.size() >= window.size()) {
        std::copy(newData.end() - window.size(), newData.end(), window.begin());
    } else {
        std::copy(window.begin() + newData.size(), window.end(), window.begin());
//            std::rotate(window.begin(), window.begin() + newData.size(), window.end()); // todo check and test perform
        std::copy(newData.begin(), newData.end(), window.end() - newData.size());
    }
}

void AudioWindow::append_data(const float *data, size_t dataSize) {
    if (dataSize == 0 || data == nullptr) {
        return;
    }

    if (dataSize >= window.size()) {
        std::copy(data + dataSize - window.size(), data + dataSize, window.begin());
    } else {
        std::copy(window.begin() + dataSize, window.end(), window.begin());
//            std::rotate(window.begin(), window.begin() + dataSize, window.end()); // todo check and test perform
        std::copy(data, data + dataSize, window.end() - dataSize);
    }
}

void AudioWindow::clear() {
    std::fill(window.begin(), window.end(), 0);
}

std::vector<float> &AudioWindow::get_window() {
    return window;
}

size_t AudioWindow::size() const {
    return window.size();
}



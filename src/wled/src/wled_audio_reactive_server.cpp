#include <QHostAddress>
#include <QUdpSocket>
#include <algorithm>
#include <cmath>
#include <winsock2.h>

#include "wled/wled_audio_reactive_server.h"


Q_LOGGING_CATEGORY(logger, "wled_audio_reactive_server")

WLEDAudioReactiveServer::WLEDAudioReactiveServer(const QHostAddress &broadcast_address, const quint16 broadcast_port) {
    // Store interface address for binding
    _interface_address = broadcast_address;
    
    // Always use limited broadcast address (255.255.255.255) for WLED
    _broadcast_address = QHostAddress("255.255.255.255");
    _broadcast_port = broadcast_port;
    _data.resize(16, 0.0);
    _frame_counter = 0;

    _udp_socket = new QUdpSocket();

    // Bind to specific interface address for broadcast to work on Windows
    // The source IP must be the interface IP for broadcast to be sent correctly
    const bool bind_ok = _udp_socket->bind(_interface_address, 0);

    if (not bind_ok) {
        qCWarning(logger, "Failed to bind socket to %s: %s",
                 _interface_address.toString().toStdString().c_str(),
                 _udp_socket->errorString().toStdString().c_str());
    } else {
        qCInfo(logger, "Socket bound to interface: %s", _interface_address.toString().toStdString().c_str());
    }

    // Enable broadcast using native socket
    int broadcast_enabled = 1;
    if (setsockopt(_udp_socket->socketDescriptor(), SOL_SOCKET, SO_BROADCAST,
                   reinterpret_cast<const char*>(&broadcast_enabled), sizeof(broadcast_enabled)) < 0) {
        qCWarning(logger, "Failed to enable broadcast socket option");
                   }

    qCInfo(logger, "UDP socket created and configured for broadcast");

    qCInfo(logger, "WLED broadcast address set to: %s:%u (interface: %s)", 
           _broadcast_address.toString().toStdString().c_str(), _broadcast_port,
           _interface_address.toString().toStdString().c_str());
}

WLEDAudioReactiveServer::~WLEDAudioReactiveServer() {
    delete _udp_socket;
}

void WLEDAudioReactiveServer::update(const std::vector<float> &spectrum) {
    if (spectrum.size() != 50) {
        qCInfo(logger, "incorrect size: expected 50, got %zu", spectrum.size());
        return;
    }
    
    // Map spectrum[4..46] (42 values) to 16 WLED bins
    // Data is already normalized to 0.0-1.0 range
    constexpr int start_idx = 4;
    constexpr int end_idx = 46;
    constexpr int useful_bins = end_idx - start_idx;  // 42 bins
    constexpr int target_bins = 16;
    
    // Calculate bins per target bin
    constexpr float bins_per_target = static_cast<float>(useful_bins) / target_bins;
    
    for (int i = 0; i < target_bins; ++i) {
        // Calculate source range for this target bin
        const float src_start = i * bins_per_target;
        const float src_end = (i + 1) * bins_per_target;
        
        // Average values in source range
        float sum = 0.0f;
        int count = 0;
        
        for (int j = static_cast<int>(src_start); j < static_cast<int>(src_end) && (start_idx + j) < end_idx; ++j) {
            sum += spectrum[start_idx + j];
            count++;
        }
        
        // Use average directly (already 0.0-1.0)
        _data[i] = count > 0 ? sum / count : 0.0f;
    }
    
    _need_update = true;
}

uint64_t WLEDAudioReactiveServer::_task() {
    constexpr float fps_ms_delay = 1000.0 / 30;
    while (_thread_handle) {
        timer.start();
        if (_need_update) {
            _handler();
            _need_update = false;
        }
        const auto fft_calc_time_ms = static_cast<float>(timer.elapsed()) / 1000000;
        const auto delta = abs(fps_ms_delay - fft_calc_time_ms);
        if (delta > 0) _sleep(delta);
    }
    return 0;
}

void WLEDAudioReactiveServer::_handler() {

    // Build WLED audio sync packet
    wled_audio_reactive::AudioSyncPacket packet = {};
    
    // Set header
    std::strcpy(packet.header, "00002");
    
    // Set frame counter
    packet.frameCounter = _frame_counter++;
    
    // Calculate average and peak values from spectrum
    float avg_value = 0.0f;
    float max_magnitude = 0.0f;
    float major_peak_freq = 0.0f;
    
    for (size_t i = 0; i < 16; ++i) {
        avg_value += _data[i];
        
        // Clamp to 0-255 range for FFT bins
        packet.fftResult[i] = static_cast<uint8_t>(std::clamp(_data[i] * 255.0f, 0.0f, 255.0f));
        
        if (_data[i] > max_magnitude) {
            max_magnitude = _data[i];
            major_peak_freq = static_cast<float>(i);
        }
    }
    
    avg_value /= 16.0f;
    
    // Set sample values (0-255 range, like in C# version)
    packet.sampleRaw = avg_value * 255.0f;
    packet.sampleSmth = avg_value * 255.0f;
    
    // Set peak detection (simple threshold)
    packet.samplePeak = (max_magnitude > 0.5f) ? 1 : 0;
    
    // Set FFT magnitude and peak frequency
    packet.FFT_Magnitude = max_magnitude * 4096.0f;
    packet.FFT_MajorPeak = major_peak_freq;
    
    // Send packet via UDP broadcast
    const auto packet_data = reinterpret_cast<const char*>(&packet);
    constexpr auto packet_size = sizeof(wled_audio_reactive::AudioSyncPacket);

    const qint64 bytes_sent = _udp_socket->writeDatagram(packet_data, packet_size, _broadcast_address, _broadcast_port);
    
    if (bytes_sent <= 0) {
        qCWarning(logger, "Failed to send datagram: %s", _udp_socket->errorString().toStdString().c_str());
    } else if (bytes_sent not_eq packet_size) {
        qCWarning(logger, "Partial send: sent %lld of %zu bytes", bytes_sent, packet_size);
    }
}

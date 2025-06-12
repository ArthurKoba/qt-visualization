#ifndef QT_VISUALIZATION_PACKET_H
#define QT_VISUALIZATION_PACKET_H

#include <cstdint>

struct Packet {
    uint8_t id;
    float *data_ptr;
    size_t size;
};

#endif //QT_VISUALIZATION_PACKET_H

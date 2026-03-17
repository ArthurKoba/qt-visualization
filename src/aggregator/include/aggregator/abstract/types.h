#ifndef AGGREGATOR_ABSTRACT_TYPES_H
#define AGGREGATOR_ABSTRACT_TYPES_H

#include <cstdint>
#include <string>
#include <array>
#include <QtNetwork>

namespace aggregator_component {
    static constexpr uint16_t DEFAULT_PORT = 8212;
    static constexpr uint16_t REGISTRATION_TIMEOUT_MS = 1000;
    static constexpr int32_t CLIENT_RECONNECT_TIMEOUT_MS = 200;
    static constexpr int32_t RECONNECT_TRIES = 3;
}


enum class ComponentType : uint8_t {
    unknown = 0,
    data_source = 1,
    data_processor = 2,
    data_sink = 3,
    core = 4
};


enum class ComponentState : uint8_t {
    unknown = 0,
    disconnected = 1,
    wait_registration = 2,
    registered = 3,
    inactive = 4,
    active = 5
};

using uuid_t = QUuid::Id128Bytes;

enum class PacketType : uint8_t {
    error = 0,
    component_registration = 1,
    component_info_update = 2
};

struct registration_data_t {
    uuid_t uuid;
    ComponentType component_type;
    uint16_t component_server_port;
} __attribute__((packed));

struct component_state_t {
    uuid_t uuid;
    ComponentState new_state;
} __attribute__((packed));


struct component_info_t {
    uuid_t uuid{};
    ComponentType type = ComponentType::unknown;
    QIPv6Address address{};
    uint16_t component_server_port{};
    ComponentState state = ComponentState::wait_registration;
} __attribute__((packed));

struct registered_component_t {
    QTcpSocket *socket;
    ComponentType type;
    uint16_t component_server_port;
    ComponentState state;
};


#endif // AGGREGATOR_ABSTRACT_TYPES_H

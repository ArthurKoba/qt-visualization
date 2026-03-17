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
    static constexpr uint32_t COMPONENT_OFFLINE_TIMEOUT_MS = 60000; // 1 минута
}


enum class ComponentType : uint8_t {
    unknown = 0,
    data_source = 1,
    data_processor = 2,
    data_sink = 3,
    core = 4
};


/// @brief Состояния компонентов в системе агрегации
/// 
/// Жизненный цикл состояний:
/// unknown -> registered -> inactive -> idle -> active
///                      \-> disconnected -> off (при длительном отключении)
enum class ComponentState : uint8_t {
    /// @brief Неизвестное состояние (заглушка на случай ошибок)
    unknown = 0,
    
    /// @brief Компонент отключился и длительное время не восстанавливает подключение (более минуты)
    off = 1,
    
    /// @brief Компонент отключен, если простой достигнет минуты - переходит в off
    disconnected = 2,
    
    /// @brief Клиент зарегистрирован, но пока ничего не делает и не сообщает
    registered = 3,
    
    /// @brief Клиент сообщил о себе данные и возможности, но пока не готов участвовать в цепочках данных
    inactive = 4,
    
    /// @brief Готов к работе, но активных задач нет, простаивает
    idle = 5,
    
    /// @brief Занят работой
    active = 6
};

using uuid_t = QUuid::Id128Bytes;

enum class PacketType : uint8_t {
    error = 0,
    component_registration = 1,
    component_info_update = 2,
    component_state_change = 3
};

using session_token_t = uint16_t; // CRC16 токен сессии

struct registration_data_t {
    uuid_t uuid;
    ComponentType component_type;
    uint16_t component_server_port;
    session_token_t session_token; // токен сессии, генерируемый клиентом
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
    ComponentState state = ComponentState::unknown;
} __attribute__((packed));

struct registered_component_t {
    QTcpSocket *socket;
    ComponentType type;
    uint16_t component_server_port;
    ComponentState state;
    session_token_t session_token;
};


#endif // AGGREGATOR_ABSTRACT_TYPES_H

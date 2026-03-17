#include <QHostAddress>
#include <QLoggingCategory>

#include "core/tcp_packet_socket.h"
#include "core/utils.h"
#include "aggregator/aggregator_server.h"
#include "aggregator/utils.h"


Q_LOGGING_CATEGORY(aggregator_server, "aggregator.server")

AggregatorServer::AggregatorServer(QObject *parent) : TcpBDSPServer(parent) {
    connect(this, &TcpBDSPServer::tcp_bdsp_socket_ready, this, &AggregatorServer::on_new_connection);
}

AggregatorServer::~AggregatorServer() {
    stop();
}

bool AggregatorServer::start(uint16_t port, uint32_t registration_timeout_ms) {
    if (this->isListening()) {
        return true;
    }

    _registration_timeout_ms = registration_timeout_ms;
    if (not this->listen(QHostAddress::Any, port)) {
        qCCritical(aggregator_server, "Error: failed to start server on port %d. Maybe already used...", port);
        return false;
    }

    qCInfo(aggregator_server, "Aggregator server started on port %d", port);
    if (_registration_timeout_ms == 0) {
        qCInfo(aggregator_server, "Registration timeout: disabled (clients can connect without time limit)");
    } else {
        qCInfo(aggregator_server, "Registration timeout: %d ms", _registration_timeout_ms);
    }

    return true;
}

void AggregatorServer::stop() {
    if (this->isListening()) {
        this->close();
        qCInfo(aggregator_server, "Aggregator server stopped");
    }

    for (auto it = _sockets_uuid_map.begin(); it != _sockets_uuid_map.end(); ++it) {
        QTcpSocket *socket = it.key();
        socket->disconnect();
        socket->disconnectFromHost();
        if (socket->state() != QAbstractSocket::UnconnectedState) {
            socket->waitForDisconnected(1000);
        }
        socket->deleteLater();
    }
    _sockets_uuid_map.clear();
}

bool AggregatorServer::_register_component(registration_data_t &registration_data, TcpBDSPSocket *socket) {
    auto uuid = QUuid::fromBytes(&registration_data.uuid);
    qCDebug(aggregator_server, "Registration received from %s",
            qPrintable(convert_qt_socket_to_string_ip_port(socket)));
    
    // Проверяем, есть ли уже зарегистрированный компонент с таким UUID
    if (_components_info_map.contains(uuid)) {
        // Проверяем токен сессии для восстановления
        if (_registered_components_map.contains(uuid)) {
            auto &existing_component = _registered_components_map[uuid];
            if (existing_component.session_token == registration_data.session_token) {
                qCInfo(aggregator_server, "Session restored for component uuid: %s, %s", 
                       qPrintable(uuid.toString()), qPrintable(convert_qt_socket_to_string_ip_port(socket)));
                
                // Обновляем сокет и состояние
                existing_component.socket = socket;
                existing_component.state = ComponentState::registered;
                
                // Обновляем маппинги
                _sockets_uuid_map[socket] = uuid;
                _components_info_map[uuid].state = ComponentState::registered;
                
                // Уведомляем клиента о успешной регистрации через изменение состояния
                _send_component_state_change_event(uuid, ComponentState::registered);
                
                return true;
            } else {
                qCWarning(aggregator_server, "Invalid session token for component uuid: %s from %s",
                          qPrintable(uuid.toString()), qPrintable(convert_qt_socket_to_string_ip_port(socket)));
                return false;
            }
        } else {
            qCWarning(aggregator_server, "Component with uuid: %s, already registered from %s",
                      qPrintable(uuid.toString()), qPrintable(convert_qt_socket_to_string_ip_port(socket)));
            return false;
        }
    }
    
    // Новая регистрация - используем токен сессии, который прислал клиент
    qCInfo(aggregator_server, "Registered new component. %s", 
           qPrintable(aggregator_utils::format_component_registration(uuid, registration_data.component_type, 
                     convert_qt_socket_to_string_ip_port(socket))));
    
    component_info_t info = {
        .uuid = uuid.toBytes(),
        .type = registration_data.component_type,
        .address = socket->peerAddress().toIPv6Address(),
        .component_server_port = registration_data.component_server_port,
        .state = ComponentState::registered
    };
    
    registered_component_t registered_component = {
        .socket = socket,
        .type = registration_data.component_type,
        .component_server_port = registration_data.component_server_port,
        .state = ComponentState::registered,
        .session_token = registration_data.session_token // сохраняем токен от клиента
    };
    
    _components_info_map[uuid] = info;
    _registered_components_map[uuid] = registered_component;
    _sockets_uuid_map[socket] = uuid;
    
    // Уведомляем всех остальных клиентов о новом компоненте (полная информация)
    _send_update_component_event(info);
    
    // Уведомляем самого клиента о успешной регистрации через изменение состояния
    _send_component_state_change_event(uuid, ComponentState::registered);
    
    return true;
}

bool AggregatorServer::_get_registration_status(TcpBDSPSocket *socket) {
    return not this->_sockets_uuid_map[socket].isNull();
}

void AggregatorServer::_disconnect_socket(TcpBDSPSocket *socket) {
    if (_sockets_uuid_map.contains(socket)) {
        auto uuid = _sockets_uuid_map.value(socket);

        if (not uuid.isNull()) {
            // Не удаляем из _components_info_map и _registered_components_map
            // для возможности восстановления сессии
            if (_registered_components_map.contains(uuid)) {
                _registered_components_map[uuid].socket = nullptr;
                _update_component_state(uuid, ComponentState::disconnected);
                _start_offline_timer(uuid);
            }
        }
        _sockets_uuid_map.remove(socket);
    }

    if (socket->isOpen()) {
        socket->close();
    }
}

void AggregatorServer::on_got_packet(BDSP::packet_context_t &packet_context, TcpBDSPSocket *socket) {
    auto packet_type = static_cast<PacketType>(packet_context.packet_id);

    if (not _get_registration_status(socket) and packet_type not_eq PacketType::component_registration) {
        qCWarning(aggregator_server, "An unregistered socket (%s) tries to work without registration",
                  qPrintable(convert_qt_socket_to_string_ip_port(socket)));
        return;
    }

    switch (packet_type) {
        case PacketType::component_registration: {
            if (packet_context.size not_eq sizeof(registration_data_t)) {
                qCWarning(aggregator_server, "Registration packet size incorrect from %s",
                          qPrintable(convert_qt_socket_to_string_ip_port(socket)));
                socket->close();
                // todo check
                return;
            }
            auto data = *reinterpret_cast<registration_data_t *>(packet_context.data_ptr);
            _register_component(data, socket);
            break;
        }
        case PacketType::component_state_change: {
            if (packet_context.size not_eq sizeof(component_state_t)) {
                qCWarning(aggregator_server, "Component state change packet size incorrect from %s",
                          qPrintable(convert_qt_socket_to_string_ip_port(socket)));
                return;
            }
            auto state_data = *reinterpret_cast<component_state_t *>(packet_context.data_ptr);
            auto uuid = QUuid::fromBytes(&state_data.uuid);
            
            // Проверяем, что клиент может изменять состояние только своего компонента
            if (_sockets_uuid_map.value(socket) == uuid) {
                _update_component_state(uuid, state_data.new_state);
                qCDebug(aggregator_server, "Component %s changed state to %s", 
                        qPrintable(uuid.toString()), qPrintable(aggregator_utils::component_state_to_string(state_data.new_state)));
            } else {
                qCWarning(aggregator_server, "Component tried to change state of another component");
            }
            break;
        }
        default:
            qCWarning(aggregator_server, "Unknown packet type %u from %s", packet_type,
                      qPrintable(convert_qt_socket_to_string_ip_port(socket)));
            break;
    }
}

void AggregatorServer::on_new_connection(TcpBDSPSocket *socket) {
    auto address = convert_qt_host_address_to_string(socket->peerAddress());;
    qCDebug(aggregator_server, "[%x] new connection, %s:%d", socket, qPrintable(address), socket->peerPort());
    _sockets_uuid_map[socket];
    connect(socket, &TcpBDSPSocket::disconnected, this, &AggregatorServer::on_client_disconnected);
    connect(socket, &TcpBDSPSocket::on_got_packet, this, &AggregatorServer::on_got_packet);
    
    // Устанавливаем таймер регистрации только если таймаут не равен 0
    if (_registration_timeout_ms > 0) {
        QTimer::singleShot(_registration_timeout_ms, this, [socket, this]() {
            if (not this->_get_registration_status(socket)) {
                qCWarning(aggregator_server, "Registration timeout for %s",
                          qPrintable(convert_qt_socket_to_string_ip_port(socket)));
                _disconnect_socket(socket);
            }
        });
    } else {
        qCDebug(aggregator_server, "Registration timeout disabled for %s", 
                qPrintable(convert_qt_socket_to_string_ip_port(socket)));
    }
}

void AggregatorServer::on_client_disconnected(TcpBDSPSocket *socket) {
    auto address = convert_qt_host_address_to_string(socket->peerAddress());
    qCDebug(aggregator_server, "[%x] disconnected, %s:%d", socket, qPrintable(address), socket->peerPort());
    
    if (_sockets_uuid_map.contains(socket)) {
        auto uuid = _sockets_uuid_map.value(socket);
        
        if (not uuid.isNull() and _registered_components_map.contains(uuid)) {
            // Обновляем состояние на disconnected
            _update_component_state(uuid, ComponentState::disconnected);
            
            // Убираем сокет из registered_component, но сохраняем информацию для восстановления сессии
            _registered_components_map[uuid].socket = nullptr;
            
            // Запускаем таймер для перехода в состояние off
            _start_offline_timer(uuid);
        }
        
        _sockets_uuid_map.remove(socket);
    }
}

void AggregatorServer::_send_update_component_event(component_info_t &info) {
    _broadcast(PacketType::component_info_update, reinterpret_cast<uint8_t *>(&info), sizeof(info));
}

void AggregatorServer::_broadcast(PacketType packet_type, uint8_t *data, size_t size) {
    for (auto it = _sockets_uuid_map.begin(); it != _sockets_uuid_map.end(); ++it) {
        if (it.value().isNull()) continue;
        if (it.key() == nullptr) continue;
        it.key()->send_data(static_cast<uint8_t>(packet_type), data, size);
    }
}

void AggregatorServer::_broadcast_except_socket(PacketType packet_type, uint8_t *data, size_t size, TcpBDSPSocket *except_socket) {
    for (auto it = _sockets_uuid_map.begin(); it != _sockets_uuid_map.end(); ++it) {
        if (it.value().isNull()) continue;
        if (it.key() == nullptr or it.key() == except_socket) continue;
        it.key()->send_data(static_cast<uint8_t>(packet_type), data, size);
    }
}

void AggregatorServer::_send_component_state_change_event(const QUuid &uuid, ComponentState new_state) {
    component_state_t state_data = {
        .uuid = uuid.toBytes(),
        .new_state = new_state
    };
    
    qCDebug(aggregator_server, "Broadcasting component state change for %s", 
            qPrintable(aggregator_utils::format_component_state_change(uuid, new_state)));
    _broadcast(PacketType::component_state_change, reinterpret_cast<uint8_t *>(&state_data), sizeof(state_data));
}

void AggregatorServer::_update_component_state(const QUuid &uuid, ComponentState new_state) {
    if (_components_info_map.contains(uuid)) {
        _components_info_map[uuid].state = new_state;
        
        if (_registered_components_map.contains(uuid)) {
            _registered_components_map[uuid].state = new_state;
        }
        
        qCInfo(aggregator_server, "Component state changed to %s for %s", 
               qPrintable(aggregator_utils::component_state_to_string(new_state)), qPrintable(uuid.toString()));
        
        // Уведомляем всех клиентов об изменении состояния через component_state_change
        _send_component_state_change_event(uuid, new_state);
    }
}

void AggregatorServer::_handle_component_offline_timeout(const QUuid &uuid) {
    qCInfo(aggregator_server, "Component %s went offline due to timeout", qPrintable(uuid.toString()));
    
    // Переводим в состояние off
    _update_component_state(uuid, ComponentState::off);
    
    // Удаляем из всех таблиц, чтобы освободить UUID для нового компонента
    _registered_components_map.remove(uuid);
    _components_info_map.remove(uuid);
    
    qCInfo(aggregator_server, "Component %s removed from registry", qPrintable(uuid.toString()));
}

void AggregatorServer::_start_offline_timer(const QUuid &uuid) {
    qCDebug(aggregator_server, "Starting offline timer for component %s (%d ms)", 
            qPrintable(uuid.toString()), aggregator_component::COMPONENT_OFFLINE_TIMEOUT_MS);
    
    QTimer::singleShot(aggregator_component::COMPONENT_OFFLINE_TIMEOUT_MS, this, [this, uuid]() {
        _handle_component_offline_timeout(uuid);
    });
}


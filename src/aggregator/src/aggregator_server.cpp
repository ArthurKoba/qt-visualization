#include <QHostAddress>

#include "core/network/tcp_packet_socket.h"
#include "core/utils.h"
#include "aggregator/aggregator_server.h"
#include "aggregator/utils.h"


Q_LOGGING_CATEGORY(aggregator_server, "aggregator.server")

AggregatorServer::AggregatorServer(QObject *parent) : TcpBDSPServer(parent) {
    connect(this, &TcpBDSPServer::tcp_bdsp_socket_ready, this, &AggregatorServer::_on_new_connection);
}

AggregatorServer::~AggregatorServer() {
    stop();
}

bool AggregatorServer::start(const uint16_t port, const uint32_t registration_timeout_ms) {
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

bool AggregatorServer::_register_component(TcpBDSPSocket *socket, const QUuid &uuid,
                                           const component_info_t &new_info, const uint16_t session_token) {
    qCDebug(aggregator_server, "Registration received from %s",
            qPrintable(convert_qt_socket_to_string_ip_port(socket)));

    if (_registered_components_map.contains(uuid)) {
        auto &[socket_before, last_token, info] = _registered_components_map[uuid];
        if (last_token not_eq session_token) {
            qCWarning(aggregator_server, "Invalid session token for component uuid: %s from %s",
                      qPrintable(uuid.toString()), qPrintable(convert_qt_socket_to_string_ip_port(socket)));
            return false;
        }

        if (socket_before->isOpen() and socket_before not_eq socket) {
            qCWarning(aggregator_server, "Socket %s tried to take control of a component with a uuid: %s",
                      qPrintable(convert_qt_socket_to_string_ip_port(socket)), qPrintable(uuid.toString()));
            return false;
        }

        info.state = ComponentState::registered;

        _registered_components_map[uuid].info.state = ComponentState::registered;

        qCInfo(aggregator_server, "Session restored for component uuid: %s, %s",
               qPrintable(uuid.toString()), qPrintable(convert_qt_socket_to_string_ip_port(socket)));
    } else {
        // Новая регистрация - используем токен сессии, который прислал клиент
        qCInfo(aggregator_server, "Registered new component %s",
               qPrintable(aggregator_utils::format_component_registration(uuid, new_info.type,
                   convert_qt_socket_to_string_ip_port(socket))));
        _registered_components_map[uuid] = {
            .socket = socket,
            .session_token = session_token,
            .info = {
                .type = new_info.type,
                .state = ComponentState::registered,
                .component_server_port = new_info.component_server_port,
            }
        };
    }
    _sockets_uuid_map[socket] = uuid;
    component_update_full_packet_t info = {
        .uuid = uuid.toBytes(),
        .type = new_info.type,
        .address = socket->peerAddress().toIPv6Address(),
        .component_server_port = new_info.component_server_port,
        .state = ComponentState::registered
    };
    _send_update_component_event(info);
    return true;
}

bool AggregatorServer::_get_registration_status(TcpBDSPSocket *socket) {
    return this->_sockets_uuid_map[socket].has_value();
}

void AggregatorServer::_disconnect_socket(TcpBDSPSocket *socket) {
    if (_sockets_uuid_map.contains(socket)) {
        const auto uuid = _sockets_uuid_map.value(socket);

        if (uuid.has_value() and _registered_components_map.contains(uuid.value())) {
            _registered_components_map[uuid.value()].socket = nullptr;
            // Обновляем состояние на disconnected
            _update_component_state_with_broadcast(uuid.value(), ComponentState::disconnected);
            // Запускаем таймер для перехода в состояние offline
            _start_offline_timer(uuid.value());
        }
        _sockets_uuid_map.remove(socket);
    }

    if (socket->isOpen()) socket->close();
}

void AggregatorServer::_on_got_packet(BDSP::packet_context_t &packet_context, TcpBDSPSocket *socket) {
    auto packet_type = static_cast<PacketType>(packet_context.packet_id);

    if (not _get_registration_status(socket) and packet_type not_eq PacketType::component_registration) {
        qCWarning(aggregator_server, "An unregistered socket (%s) tries to work without registration",
                  qPrintable(convert_qt_socket_to_string_ip_port(socket)));
        return;
    }

    switch (packet_type) {
        case PacketType::component_registration: {
            if (packet_context.size not_eq sizeof(registration_request_packet_t)) {
                qCWarning(aggregator_server, "Registration packet size incorrect from %s",
                          qPrintable(convert_qt_socket_to_string_ip_port(socket)));
                // todo send error message to socket
                return;
            }
            auto [uuid_raw, component_type, component_server_port, session_token] = *reinterpret_cast<
                registration_request_packet_t *>(packet_context.data_ptr);
            const QUuid uuid = QUuid::fromBytes(&uuid_raw);
            const component_info_t info = {
                .type = component_type,
                .state = ComponentState::unknown,
                .component_server_port = component_server_port,
                .address = socket->peerAddress()
            };
            if (not _register_component(socket, uuid, info, session_token)) {
                // todo send error message to socket
            };
            break;
        }
        case PacketType::component_state_change: {
            if (packet_context.size not_eq sizeof(component_update_state_packet_t)) {
                qCWarning(aggregator_server, "Component state change packet size incorrect from %s",
                          qPrintable(convert_qt_socket_to_string_ip_port(socket)));
                // todo send error message to socket
                return;
            }
            auto [uuid_raw, new_state] = *reinterpret_cast<component_update_state_packet_t *>(packet_context.data_ptr);
            const auto uuid = QUuid::fromBytes(&uuid_raw);

            // Проверяем, что клиент может изменять состояние только своего компонента
            if (_sockets_uuid_map.value(socket) not_eq uuid) {
                qCWarning(aggregator_server, "Component tried to change state of another component");
                // todo send error message to socket and disconnect
            }
            _update_component_state_with_broadcast(uuid, new_state);
            break;
        }
        default:
            qCWarning(aggregator_server, "Unknown packet type %u from %s", packet_type,
                      qPrintable(convert_qt_socket_to_string_ip_port(socket)));
            // todo send error message
            break;
    }
}

void AggregatorServer::_on_new_connection(TcpBDSPSocket *socket) {
    auto address = convert_qt_host_address_to_string(socket->peerAddress());;
    qCDebug(aggregator_server, "new connection, %s", qPrintable(convert_qt_socket_to_string_ip_port(socket)));
    _sockets_uuid_map[socket].reset();

    connect(socket, &TcpBDSPSocket::on_disconnected, this, [this](TcpBDSPSocket *disconnected_socket) {
        qCDebug(aggregator_server, "disconnected, %s",
                qPrintable(convert_qt_socket_to_string_ip_port(disconnected_socket)));
        _disconnect_socket(disconnected_socket);
    });
    connect(socket, &TcpBDSPSocket::on_got_packet, this, &AggregatorServer::_on_got_packet);

    if (not _registration_timeout_ms) {
        qCDebug(aggregator_server, "Registration timeout disabled for %s",
                qPrintable(convert_qt_socket_to_string_ip_port(socket)));
        return;
    }
    // Устанавливаем таймер регистрации только если timeout не равен 0
    QTimer::singleShot(_registration_timeout_ms, this, [socket, this]() {
        if (this->_get_registration_status(socket)) return;
        qCWarning(aggregator_server, "Registration timeout for %s",
                  qPrintable(convert_qt_socket_to_string_ip_port(socket)));
        _disconnect_socket(socket);
    });
}


void AggregatorServer::_send_update_component_event(component_update_full_packet_t &info) {
    _broadcast(PacketType::component_info_update, reinterpret_cast<uint8_t *>(&info), sizeof(info));
}

void AggregatorServer::_broadcast(const PacketType packet_type, uint8_t *data, const size_t size) {
    _broadcast_except_socket(packet_type, data, size, nullptr);
}

void AggregatorServer::_broadcast_except_socket(PacketType packet_type, uint8_t *data, const size_t size,
                                                const TcpBDSPSocket *except_socket) {
    for (auto it = _sockets_uuid_map.begin(); it != _sockets_uuid_map.end(); ++it) {
        if (not it.key() or it.key() == except_socket) continue;
        if (not it.value().has_value()) continue; // не отправляем незарегистрированным сокетам пакет
        it.key()->send_data(static_cast<uint8_t>(packet_type), data, size);
    }
}

void AggregatorServer::_send_component_state_change_event(const QUuid &uuid, ComponentState new_state) {
    component_update_state_packet_t state_data = {
        .uuid = uuid.toBytes(),
        .new_state = new_state
    };

    qCDebug(aggregator_server, "Broadcasting component state change for %s",
            qPrintable(aggregator_utils::format_component_state_change(uuid, new_state)));
    _broadcast(PacketType::component_state_change, reinterpret_cast<uint8_t *>(&state_data), sizeof(state_data));
}

void AggregatorServer::_update_component_state_with_broadcast(const QUuid &uuid, const ComponentState new_state) {
    if (_registered_components_map.contains(uuid)) {
        _registered_components_map[uuid].info.state = new_state;
    }
    // Уведомляем всех клиентов об изменении состояния через component_state_change
    _send_component_state_change_event(uuid, new_state);
    qCInfo(aggregator_server, "Component state changed to %s for %s",
           qPrintable(aggregator_utils::component_state_to_string(new_state)), qPrintable(uuid.toString()));
}


void AggregatorServer::_start_offline_timer(const QUuid &uuid) {
    qCDebug(aggregator_server, "Starting offline timer for component %s (%d ms)",
            qPrintable(uuid.toString()), aggregator_component::COMPONENT_OFFLINE_TIMEOUT_MS);

    QTimer::singleShot(aggregator_component::COMPONENT_OFFLINE_TIMEOUT_MS, this, [this, uuid]() {
        qCInfo(aggregator_server, "Component %s went offline due to timeout", qPrintable(uuid.toString()));
        _send_component_state_change_event(uuid, ComponentState::offline);
        _registered_components_map.remove(uuid);
        qCInfo(aggregator_server, "Component %s removed from registry", qPrintable(uuid.toString()));
    });
}

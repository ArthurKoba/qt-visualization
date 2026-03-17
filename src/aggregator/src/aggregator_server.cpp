#include <QHostAddress>
#include <QDataStream>
#include <QLoggingCategory>

#include "core/tcp_packet_socket.h"
#include "core/utils.h"
#include "aggregator/aggregator_server.h"


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
    if (_components_info_map.contains(uuid)) {
        qCWarning(aggregator_server, "Component with uuid: %s, already registered from %s",
                  qPrintable(uuid.toString()), qPrintable(convert_qt_socket_to_string_ip_port(socket)));
        return false;
    }
    qCInfo(aggregator_server, "Registered components. Type: %u, uuid: %s, %s", registration_data.component_type,
           qPrintable(uuid.toString()), qPrintable(convert_qt_socket_to_string_ip_port(socket)));
    component_info_t info = {
            .uuid=uuid.toBytes(),
            .type = registration_data.component_type,
            .address = socket->peerAddress().toIPv6Address(),
            .component_server_port = registration_data.component_server_port,
            .state = ComponentState::registered
    };
    _components_info_map[uuid] = info;
    _sockets_uuid_map[socket] = uuid;
    _send_update_component_event(info);
    return true;
}

bool AggregatorServer::_get_registration_status(TcpBDSPSocket *socket) {
    return not this->_sockets_uuid_map[socket].isNull();
}

void AggregatorServer::_disconnect_socket(TcpBDSPSocket *socket) {
    if (_sockets_uuid_map.contains(socket)) {
        auto uuid = _sockets_uuid_map.value(socket);

        if (not uuid.isNull()) {
            _components_info_map.remove(uuid);
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
        qCWarning(aggregator_server, "An unregistered _socket (%s) tries to work without registration",
                  qPrintable(convert_qt_socket_to_string_ip_port(socket)));
        return;
    }

    switch (packet_type) {
        case PacketType::component_registration: {
            if (packet_context.size not_eq sizeof(registration_data_t)) {
                qCWarning(aggregator_server, "Registration packet size incorrect from %s",
                          qPrintable(convert_qt_socket_to_string_ip_port(socket)));
                return;
            }
            auto data = *reinterpret_cast<registration_data_t *>(packet_context.data_ptr);
            _register_component(data, socket);
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
    auto address = convert_qt_host_address_to_string(socket->peerAddress());;
    qCDebug(aggregator_server, "[%x] disconnected, %s:%d", socket, qPrintable(address), socket->peerPort());
    // todo remove event
}

void AggregatorServer::_send_update_component_event(component_info_t &info) {
    _broadcast(PacketType::component_info_update, reinterpret_cast<uint8_t *>(&info), sizeof(info));
}

void AggregatorServer::_broadcast(PacketType packet_type, uint8_t *data, size_t size) {
    for (auto it = _sockets_uuid_map.begin(); it != _sockets_uuid_map.end(); ++it) {
        if (it.value().isNull()) return;
        it.key()->send_data(static_cast<uint8_t>(packet_type), data, size);
    }
}


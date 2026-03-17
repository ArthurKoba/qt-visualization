#include "aggregator/aggregator_client.h"
#include <QTimer>
#include <utility>

Q_LOGGING_CATEGORY(aggregator_client, "aggregator.client")


AggregatorClient::AggregatorClient(AggregatorClientConfig config, QObject *parent)
        : QObject(parent), _socket(this), _cfg(config), _reconnect_timer(this) {

    connect(&_socket, &QTcpSocket::connected, this, &AggregatorClient::on_socket_connected);
    connect(&_socket, &QTcpSocket::disconnected, this, &AggregatorClient::on_socket_disconnected);
    connect(&_socket, &TcpBDSPSocket::on_got_packet, this, &AggregatorClient::handle_packet);

    _reconnect_timer.setSingleShot(true);
    connect(&_reconnect_timer, &QTimer::timeout, this, &AggregatorClient::_on_reconnect_timeout);
    set_reconnect_mode(false, 10);
}

AggregatorClient::~AggregatorClient() {
    _reconnect_timer.stop();
    _socket.close();
    _socket.deleteLater();
}


bool AggregatorClient::connect_to_aggregator_server(QHostAddress address, uint16_t port, int32_t timeout_ms) {
    _aggregator_server_address = std::move(address);
    _aggregator_server_port = port;
    if (_socket.isOpen()) _socket.close();
    _socket.connectToHost(_aggregator_server_address, _aggregator_server_port);
    bool result = _socket.waitForConnected(timeout_ms);
    if (not result) _socket.close();
    return result;
}

bool AggregatorClient::is_connected() const {
    return _socket.isOpen();
}

bool AggregatorClient::is_registered() const {
    return _is_registered;
}

void AggregatorClient::on_socket_connected() {
    qCInfo(aggregator_client, "Connected to server, sending registration");
    _current_reconnect_attempt = 0;
    _reconnect_timer.stop();
    registration_data_t data{};
    data.component_type = _cfg.type;
    data.component_server_port = _cfg.component_server_port;
    data.uuid = _cfg.uuid.toBytes();
    _socket.send_data(static_cast<uint8_t>(PacketType::component_registration),
                      reinterpret_cast<uint8_t *>(&data), sizeof(data));
}

void AggregatorClient::on_socket_disconnected() {
    qCInfo(aggregator_client, "Disconnected from server");
    _is_registered = false;
    if (not _reconnect_tries.has_value()) {
        qCInfo(aggregator_client, "An endless process of restoring the connection has begun...");
    }
    _start_reconnect_timer();
}

void AggregatorClient::_start_reconnect_timer() {
    if (not _reconnect_tries.has_value()) {
        // Бесконечное переподключение
        _reconnect_timer.start(_reconnect_interval_ms);
    } else {
        // Ограниченное количество попыток
        qCInfo(aggregator_client, "(%d) Reconnect try...", _current_reconnect_attempt);

        if (_current_reconnect_attempt < _reconnect_tries.value()) {
            _reconnect_timer.start(_reconnect_interval_ms);
        } else {
            qCInfo(aggregator_client, "Max reconnection attempts reached (%d)", _current_reconnect_attempt);
            emit on_client_close();
        }
    }
}


void AggregatorClient::_on_reconnect_timeout() {
    _current_reconnect_attempt++;
    auto status = connect_to_aggregator_server(_aggregator_server_address, _aggregator_server_port);
    if (status) {
        qCInfo(aggregator_client, "Reconnected successfully");
    } else {
        _start_reconnect_timer();
    }
}

void AggregatorClient::handle_packet(BDSP::packet_context_t &packet_context) {
    auto packet_type = static_cast<PacketType>(packet_context.packet_id);

    switch (packet_type) {
        case PacketType::component_info_update: {
            if (packet_context.size not_eq sizeof(component_info_t)) {
                qCWarning(aggregator_client, "Component info packet size incorrect.");
                return;
            }

            auto info = *reinterpret_cast<component_info_t *>(packet_context.data_ptr);
            auto uuid = QUuid::fromBytes(&info.uuid);
            qCDebug(aggregator_client, "Got new component from aggregation server. UUID: %s, type: %d",
                    qPrintable(uuid.toString()), info.type);
            if (uuid == _cfg.uuid and info.state >= ComponentState::registered) {
                _is_registered = true;
                qCWarning(aggregator_client, "This component successfully registered!");
                emit registered();
            }
            break;
        }
        default:
            qCWarning(aggregator_client, "Got unknown packet type: %u", static_cast<uint32_t>(packet_type));
            break;
    }
}

void AggregatorClient::set_reconnect_mode(bool infinite_mode, int32_t tries, int32_t reconnect_interval_ms) {
    if (infinite_mode) {
        _reconnect_tries.reset();
    } else {
        _reconnect_tries = tries;
    }
    _reconnect_interval_ms = reconnect_interval_ms;
}

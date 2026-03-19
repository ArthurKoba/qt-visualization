#include <QTimer>

#include "core/utils.h"
#include "aggregator/aggregator_client.h"
#include "aggregator/utils.h"

Q_LOGGING_CATEGORY(aggregator_client, "aggregator.client")

AggregatorClient::AggregatorClient(const AggregatorClientConfig &config, QObject *parent)
    : QObject(parent), _cfg(config), _socket(this), _reconnect_timer(this) {
    _session_token = aggregator_utils::generate_session_token(_cfg.uuid, _cfg.type, _cfg.component_server_port);
    connect(&_socket, &TcpBDSPSocket::on_got_packet, this, &AggregatorClient::_handle_packet);

    connect(&_socket, &QTcpSocket::connected, this, [this]() {
        qCInfo(aggregator_client, "Connected to server, sending registration");
        _current_reconnect_attempt = 0;
        _reconnect_timer.stop();
        _send_registration_request();
    });

    connect(&_socket, &QTcpSocket::disconnected, this, [this]() {
        qCInfo(aggregator_client, "Socket disconnected");
        if (_is_registered) {
            _is_registered = false;
            emit on_client_disconnected();
        }
        if (not _reconnect_tries.has_value()) {
            qCInfo(aggregator_client, "An endless process of restoring the connection has begun...");
        }
        _start_reconnecting();
    });


    _reconnect_timer.setSingleShot(true);
    connect(&_reconnect_timer, &QTimer::timeout, this, [this]() {
        _current_reconnect_attempt++;
        if (connect_to_aggregator_server(_aggregator_server_address, _aggregator_server_port)) {
            qCInfo(aggregator_client, "Reconnected successfully");
        } else {
            _start_reconnecting();
        }
    });

    qCDebug(aggregator_client, "AggregatorClient created for component: %s",
            qPrintable(_cfg.uuid.toString()));
}

AggregatorClient::~AggregatorClient() {
    _reconnect_timer.stop();
    _socket.close();
}


bool AggregatorClient::connect_to_aggregator_server(QHostAddress address, uint16_t port, int32_t timeout_ms) {
    _aggregator_server_address = std::move(address);
    _aggregator_server_port = port;
    if (_socket.isOpen()) _socket.close();
    _socket.connectToHost(_aggregator_server_address, _aggregator_server_port);
    const bool result = _socket.waitForConnected(timeout_ms);
    if (not result) _socket.close();
    return result;
}

void AggregatorClient::connect_to_aggregator_server_with_waiting(const QHostAddress &address, const uint16_t port,
                                                                 const int32_t timeout_ms) {
    const auto last = _reconnect_tries;
    _reconnect_tries.reset();
    while (not connect_to_aggregator_server(address, port, timeout_ms)) {
    }
    _reconnect_tries = last;
}

bool AggregatorClient::is_connected() const {
    return _socket.isOpen();
}

bool AggregatorClient::is_registered() const {
    return _is_registered;
}


void AggregatorClient::_start_reconnecting() {
    if (not _reconnect_tries.has_value()) {
        // Бесконечное переподключение
        _reconnect_timer.start(_reconnect_interval_ms);
        return;
    }
    // Ограниченное количество попыток
    qCInfo(aggregator_client, "(%d) Reconnect try...", _current_reconnect_attempt);
    if (_current_reconnect_attempt < _reconnect_tries.value()) {
        _reconnect_timer.start(_reconnect_interval_ms);
    } else {
        qCInfo(aggregator_client, "Max reconnection attempts reached (%d)", _current_reconnect_attempt);
    }
}


void AggregatorClient::_handle_packet(BDSP::packet_context_t &packet_context) {
    const auto packet_type = static_cast<PacketType>(packet_context.packet_id);
    switch (packet_type) {
        case PacketType::component_state_change: {
            if (packet_context.size not_eq sizeof(component_update_state_packet_t)) {
                qCWarning(aggregator_client, "Component state change packet size incorrect.");
                return;
            }

            const auto [uuid_bytes, new_state] = *reinterpret_cast<component_update_state_packet_t *>(packet_context.
                data_ptr);
            const auto uuid = QUuid::fromBytes(&uuid_bytes);

            if (uuid == _cfg.uuid) return;
            qCInfo(aggregator_client, "Component state change: %s",
                   qPrintable(aggregator_utils::format_component_state_change(uuid, new_state)));
            emit on_update_component_state(uuid, new_state);
            break;
        }
        case PacketType::component_info_update: {
            if (packet_context.size not_eq sizeof(component_update_full_packet_t)) {
                qCWarning(aggregator_client, "Component info packet size incorrect.");
                return;
            }

            const auto [uuid_raw, type, address_raw, component_server_port, state] =
                    *reinterpret_cast<component_update_full_packet_t *>(packet_context.data_ptr);
            const QUuid uuid = QUuid::fromBytes(&uuid_raw);

            const component_info_t info{
                .type = type,
                .state = state,
                .component_server_port = component_server_port,
                .address = QHostAddress(address_raw),
            };

            if (uuid == _cfg.uuid) {
                if (state >= ComponentState::registered) {
                    _is_registered = true;
                    qCInfo(aggregator_client, "Registration AggregatorClient successful! External Ip: %s",
                           qPrintable(convert_qt_host_address_to_string(info.address)));
                    emit on_client_registered();
                }
                // todo update internal ip
                return;
            }


            qCInfo(aggregator_client, "New component: %s",
                   qPrintable(aggregator_utils::format_component_info(uuid, type, state)));

            // Генерируем событие обновления компонента
            emit on_update_component_info(uuid, info);
            break;
        }
        default:
            qCWarning(aggregator_client, "Got unknown packet type: %d", packet_type);
            break;
    }
}

void AggregatorClient::set_reconnect_mode(const bool infinite_mode, int32_t tries,
                                          const int32_t reconnect_interval_ms) {
    _reconnect_interval_ms = reconnect_interval_ms;
    if (infinite_mode) {
        _reconnect_tries.reset();
        return;
    }
    _reconnect_tries = tries;
}

void AggregatorClient::_send_registration_request() const {
    registration_request_packet_t data{};
    data.component_type = _cfg.type;
    data.component_server_port = _cfg.component_server_port;
    data.uuid = _cfg.uuid.toBytes();
    data.session_token = _session_token;
    _socket.send_data(static_cast<uint8_t>(PacketType::component_registration),
                      reinterpret_cast<uint8_t *>(&data), sizeof(data));
}

#ifndef AGGREGATOR_CLIENT_H
#define AGGREGATOR_CLIENT_H

#include "aggregator/abstract/types.h"
#include "core/tcp_packet_socket.h"
#include <BDSP/transceivers.h>
#include <QTcpSocket>
#include <QObject>
#include <QUuid>
#include <QHostAddress>
#include <optional>

Q_DECLARE_LOGGING_CATEGORY(aggregator_client)

struct AggregatorClientConfig {
    QUuid uuid{};
    ComponentType type = ComponentType::unknown;
    uint16_t component_server_port = 0;
};

class AggregatorClient final : public QObject {
Q_OBJECT
public:
    explicit AggregatorClient(AggregatorClientConfig config, QObject *parent = nullptr);

    ~AggregatorClient() final;

    bool connect_to_aggregator_server(QHostAddress address = QHostAddress::LocalHost,
                                      uint16_t port = aggregator_component::DEFAULT_PORT,
                                      int32_t timeout_ms = aggregator_component::CLIENT_RECONNECT_TIMEOUT_MS);

    bool is_connected() const;

    bool is_registered() const;

    void set_reconnect_mode(bool infinite_mode, int32_t tries = aggregator_component::RECONNECT_TRIES,
                            int32_t reconnect_interval_ms = aggregator_component::CLIENT_RECONNECT_TIMEOUT_MS);

Q_SIGNALS:

    void on_client_close();

    void registered();

protected:
    AggregatorClientConfig _cfg{};
    TcpBDSPSocket _socket;
    QHostAddress _aggregator_server_address = QHostAddress::LocalHost;
    uint16_t _aggregator_server_port = aggregator_component::DEFAULT_PORT;
    bool _is_registered = false;
    session_token_t _session_token = 0; // токен сессии, генерируемый клиентом
    std::optional<int32_t> _reconnect_tries;
    int32_t _current_reconnect_attempt = 0;
    QTimer _reconnect_timer;
    int32_t _reconnect_interval_ms = aggregator_component::CLIENT_RECONNECT_TIMEOUT_MS;

private:
    void _start_reconnect_timer();

    void _on_reconnect_timeout();

    void on_socket_connected();

    void on_socket_disconnected();

    void handle_packet(BDSP::packet_context_t &packet_ctx);
};

#endif // AGGREGATOR_CLIENT_H

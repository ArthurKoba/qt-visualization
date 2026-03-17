#ifndef AGGREGATOR_SERVER_H
#define AGGREGATOR_SERVER_H

#include "abstract/types.h"
#include "core/tcp_packet_socket.h"
#include "core/tcp_packet_server.h"

#include <QTimer>
#include <QObject>
#include <QHash>
#include <QUuid>


Q_DECLARE_LOGGING_CATEGORY(aggregator_server)

/**
 * @brief Сервер агрегации событий
 * 
 * Центральный компонент системы для отслеживания доступности
 * и состояния всех компонентов. Работает на статическом порту 8212.
 */

class AggregatorServer final : public TcpBDSPServer {
Q_OBJECT
public:
    explicit AggregatorServer(QObject *parent = nullptr);

    ~AggregatorServer() final;

    bool start(uint16_t port = aggregator_component::DEFAULT_PORT, 
               uint32_t registration_timeout_ms = aggregator_component::REGISTRATION_TIMEOUT_MS);

    void stop();

public slots:

    void on_got_packet(BDSP::packet_context_t &packet_context, TcpBDSPSocket *socket);

    void on_new_connection(TcpBDSPSocket *socket);

    void on_client_disconnected(TcpBDSPSocket *socket);

protected:
    QHash<TcpBDSPSocket *, QUuid> _sockets_uuid_map;
    QHash<QUuid, component_info_t> _components_info_map;
    uint32_t _registration_timeout_ms = aggregator_component::REGISTRATION_TIMEOUT_MS;

private:
    bool _register_component(registration_data_t &registration_data, TcpBDSPSocket *socket);

    bool _get_registration_status(TcpBDSPSocket *socket);

    void _disconnect_socket(TcpBDSPSocket *socket);

    void _send_update_component_event(component_info_t &info);

    void _broadcast(PacketType packet_type, uint8_t *data, size_t size);
};


#endif // AGGREGATOR_SERVER_H

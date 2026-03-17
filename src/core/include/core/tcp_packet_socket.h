#ifndef AGGREGATOR_SERVER_PROTOCOL_PACKET_HANDLER_H
#define AGGREGATOR_SERVER_PROTOCOL_PACKET_HANDLER_H

#include "BDSP/transceivers.h"
#include <QTcpSocket>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(tcp_bdsp_socket)

class TcpBDSPSocket final : public QTcpSocket {
    Q_OBJECT
public:
    explicit TcpBDSPSocket(QObject *parent = nullptr);

    ~TcpBDSPSocket() override;

    void send_data(uint8_t packet_id, uint8_t *data, size_t size);

Q_SIGNALS:

    void on_got_packet(BDSP::packet_context_t & packet_context, TcpBDSPSocket * socket);

    void on_bdsp_packet_parsing_error(BDSP::parse_packet_status_t status, TcpBDSPSocket *socket);

    void disconnected(TcpBDSPSocket *socket);

private:
    std::unique_ptr<BDSP::COBSZPETransceiver> _transceiver;
    using QTcpSocket::readyRead;
};


#endif // AGGREGATOR_SERVER_PROTOCOL_PACKET_HANDLER_H

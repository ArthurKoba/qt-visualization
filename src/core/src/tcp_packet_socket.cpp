#include "core/tcp_packet_socket.h"
#include "core/utils.h"

#include <QDataStream>

Q_LOGGING_CATEGORY(tcp_bdsp_socket, "tcp_socket.bdsp")

TcpBDSPSocket::TcpBDSPSocket(QObject *parent)
        : QTcpSocket(parent), _transceiver(std::make_unique<BDSP::COBSZPETransceiver>()) {
    _transceiver->get_write_stream().set_stream_writer([](const uint8_t byte, void *ctx) {
        auto *socket = static_cast<TcpBDSPSocket *>(ctx);
        if (not socket->isWritable()) {
            return qWarning("Socket %s alreay not writable",
                            qPrintable(convert_qt_socket_to_string_ip_port(socket)));
        }
        socket->write(reinterpret_cast<const char *>(&byte), 1);
    }, this);

    _transceiver->set_packet_handler([](BDSP::packet_context_t &packet_ctx, void *ctx) {
        const auto socket = static_cast<TcpBDSPSocket *>(ctx);
        qCDebug(tcp_bdsp_socket, "Received BDSP packet. ID: %d size: %d from %s", packet_ctx.packet_id,
                packet_ctx.size, qPrintable(convert_qt_socket_to_string_ip_port(socket)));
        emit socket->on_got_packet(packet_ctx, socket);
    }, this);

    _transceiver->set_error_handler([](const BDSP::parse_packet_status_t status, void *ctx) {
        const auto tcp_bdsp_socket = static_cast<TcpBDSPSocket *>(ctx);
        emit tcp_bdsp_socket->on_bdsp_packet_parsing_error(status, tcp_bdsp_socket);
    }, this);
    connect(this, &QTcpSocket::readyRead, this, [this]() {
        QByteArray data = this->readAll();
        if (data.isEmpty()) return;
        std::vector<uint8_t> buffer(data.begin(), data.end());
        _transceiver->get_read_stream().read(buffer.data(), buffer.size());
    });

    connect(this, &QTcpSocket::disconnected, this, [this]() {
        emit this->on_disconnected(this);
    });
}

TcpBDSPSocket::~TcpBDSPSocket() = default;

void TcpBDSPSocket::send_data(const uint8_t packet_id, uint8_t *data, const size_t size) const {
    _transceiver->send_data(packet_id, data, size);
}

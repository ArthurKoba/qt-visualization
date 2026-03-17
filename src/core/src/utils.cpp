#include "core/utils.h"

QString convert_qt_host_address_to_string(const QHostAddress &address) {
    bool is_ipv4;
    QHostAddress ip4Address(address.toIPv4Address(&is_ipv4));
    return is_ipv4 ? ip4Address.toString() : address.toString();;
}

QString convert_qt_socket_to_string_ip_port(const QAbstractSocket *socket) {
    auto address = convert_qt_host_address_to_string(socket->peerAddress());
    return QString("ip: %1, port: %2").arg(address).arg(socket->peerPort());
}

//network_address_t convert_qt_host_address_to_network_address(const QHostAddress &address) {
//    network_address_t network_address{};
//    bool is_ipv4;
//    network_address.ipv4 = address.toIPv4Address(&is_ipv4);
//    network_address.ip_version = is_ipv4 ? 4 : 6;
//    if (is_ipv4) return network_address;
//    auto ipv6 = address.toIPv6Address();
//    memcpy(network_address.bytes, &ipv6, 16);
//    return network_address;
//}

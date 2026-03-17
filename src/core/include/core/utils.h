#ifndef QT_VISUALIZATION_UTILS_H
#define QT_VISUALIZATION_UTILS_H

#include <QString>
#include <QHostAddress>
#include <QTcpSocket>

QString convert_qt_host_address_to_string(const QHostAddress &address);
QString convert_qt_socket_to_string_ip_port(const QAbstractSocket *socket);

#endif //QT_VISUALIZATION_UTILS_H

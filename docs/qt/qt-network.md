# Qt Network

## Описание

Qt Network предоставляет классы для сетевого программирования: TCP/IP сокеты, HTTP запросы, SSL/TLS, DNS и другие сетевые протоколы.

**Назначение:** Создание сетевых приложений, работа с интернетом, обмен данными по сети.

## Обзор

- [Qt Network Overview](https://doc.qt.io/qt-6/qtnetwork-index.html)
- [Qt Network Module](https://doc.qt.io/qt-6/qtnetwork-module.html)

## Ключевые концепции

### Сетевое программирование
- [Network Programming with Qt](https://doc.qt.io/qt-6/network-programming.html)

### Безопасность
- [Secure Sockets Layer (SSL) Classes](https://doc.qt.io/qt-6/ssl.html)

## Ключевые классы

| Класс | Описание | Ссылка |
|-------|---------|--------|
| `QTcpSocket` | TCP сокет для клиента | [QTcpSocket](https://doc.qt.io/qt-6/qtcpsocket.html) |
| `QTcpServer` | TCP сервер | [QTcpServer](https://doc.qt.io/qt-6/qtcpserver.html) |
| `QUdpSocket` | UDP сокет | [QUdpSocket](https://doc.qt.io/qt-6/qudpsocket.html) |
| `QNetworkAccessManager` | Менеджер сетевых запросов | [QNetworkAccessManager](https://doc.qt.io/qt-6/qnetworkaccessmanager.html) |
| `QNetworkRequest` | HTTP запрос | [QNetworkRequest](https://doc.qt.io/qt-6/qnetworkrequest.html) |
| `QNetworkReply` | Ответ на HTTP запрос | [QNetworkReply](https://doc.qt.io/qt-6/qnetworkreply.html) |
| `QNetworkCookie` | HTTP cookie | [QNetworkCookie](https://doc.qt.io/qt-6/qnetworkcookie.html) |
| `QNetworkCookieJar` | Хранилище cookies | [QNetworkCookieJar](https://doc.qt.io/qt-6/qnetworkcookiejar.html) |
| `QHostAddress` | IP адрес | [QHostAddress](https://doc.qt.io/qt-6/qhostaddress.html) |
| `QHostInfo` | Информация о хосте | [QHostInfo](https://doc.qt.io/qt-6/qhostinfo.html) |
| `QDnsLookup` | DNS запрос | [QDnsLookup](https://doc.qt.io/qt-6/qdnslookup.html) |
| `QSslSocket` | SSL/TLS сокет | [QSslSocket](https://doc.qt.io/qt-6/qsslsocket.html) |
| `QSslCertificate` | SSL сертификат | [QSslCertificate](https://doc.qt.io/qt-6/qsslcertificate.html) |
| `QAuthenticator` | Аутентификация | [QAuthenticator](https://doc.qt.io/qt-6/qauthenticator.html) |
| `QNetworkProxy` | Прокси сервер | [QNetworkProxy](https://doc.qt.io/qt-6/qnetworkproxy.html) |

## Примеры

- [Qt Network Examples](https://doc.qt.io/qt-6/qtnetwork-examples.html)

## CMake конфигурация

```cmake
find_package(Qt6 REQUIRED COMPONENTS Network)
target_link_libraries(my_target PRIVATE Qt6::Network)
```

## Связанные модули

- [Qt Core](./qt-core.md) — Основные классы
- [Qt Concurrent](./qt-concurrent.md) — Многопоточность для асинхронных операций

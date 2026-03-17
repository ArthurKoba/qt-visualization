# Интеграция BDSP в сервер агрегации

## Обзор

Сервер агрегации теперь использует протокол BDSP (Binary Data Stream Protocol) для надежной передачи пакетов между компонентами системы. Каждое TCP соединение обрабатывается отдельным экземпляром `TcpBDSPSocket`, который управляет BDSP трансивером.

## Архитектура

### Основные компоненты

1. **TcpBDSPSocket** - обработчик пакетов для одного TCP соединения
3. **AggregatorClient** - пример клиента, использующего BDSP
4. **Типы пакетов** - определены в `abstract/types.h`

### Типы пакетов

```cpp
enum class PacketType : uint8_t {
    error = 0,
    component_registration = 1,
    component_info_update = 2
};
```

### Сервер

```cpp
#include "aggregator_server/aggregator_server.h"

AggregatorServer server;
if (server.start(8212)) {
    qInfo() << "Server started on component_server_port 8212";
}
```

### Клиент

```cpp
#include "aggregator_server/client/bdsp_client.h"

QUuid client_uuid = QUuid::createUuid();
AggregatorClient client(client_uuid, ComponentType::data_source, "Test Client", 9001);

// Подключение к серверу
client.connect_to_server(QHostAddress::LocalHost, 8212);
```


## Протокол взаимодействия

### Регистрация компонента

1. Клиент подключается к серверу по TCP
2. Клиент отправляет пакет `component_registration` с бинарными данными компонента:
   - Тип компонента (1 байт)
   - Адрес (uuid 16 байт)
   - Порт сервера компонета для подключения цепочек данных (2 байта)
   ...
3. Сервер отвечает пакетом `component_registration` с результатом:
   - Флаг успеха (1 байт)
   - Сообщение об ошибке (строка с длиной, если неуспешно)
4. При успешной регистрации клиент начинает отправлять heartbeat пакеты

### События

- Сервер рассылает пакеты `event_notification` всем подключенным клиентам
- События включают подключение/отключение компонентов, изменения состояния
- Структура события:
  - Тип события (1 байт)
  - UUID компонента (16 байт)
  - Новое состояние (1 байт)

### События

- Сервер рассылает пакеты `event_notification` всем подключенным клиентам
- События включают подключение/отключение компонентов, изменения состояния

## Логирование

Включение отладочного логирования:
```cpp
;
```

## Расширение

Для добавления новых типов пакетов:

1. Добавьте новый тип в `PacketType`
2. Реализуйте сериализацию/десериализацию в `TcpBDSPSocket`
3. Добавьте обработку в `handle_packet()`
4. Обновите валидацию в `PacketValidator`
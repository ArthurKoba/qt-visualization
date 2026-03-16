# Интеграция BDSP в сервер агрегации

## Обзор

Сервер агрегации теперь использует протокол BDSP (Binary Data Stream Protocol) для надежной передачи пакетов между компонентами системы. Каждое TCP соединение обрабатывается отдельным экземпляром `TcpBDSPSocket`, который управляет BDSP трансивером.

## Архитектура

### Основные компоненты

1. **TcpBDSPSocket** - обработчик пакетов для одного TCP соединения
2. **PacketSerializer/PacketValidator** - утилиты для работы с пакетами
3. **AggregatorClient** - пример клиента, использующего BDSP
4. **Типы пакетов** - определены в `abstract/types.h`

### Типы пакетов

```cpp
enum class PacketType : uint8_t {
    heartbeat = 1,                  // Пакеты heartbeat
    component_registration = 2,     // Регистрация компонента
    component_status_update = 3,    // Обновление статуса
    event_notification = 4,         // Уведомления о событиях
    data_stream = 5,               // Потоки данных
    command_request = 6,           // Запросы команд
    command_response = 7           // Ответы на команды
};
```

### Структура пакета

Каждый пакет содержит:
- Тип пакета (1 байт)
- UUID источника (16 байт)
- UUID назначения (16 байт)
- Номер последовательности (4 байта)
- Временная метка (8 байт)
- Данные пакета (переменная длина)

## Использование

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

## Сборка

Убедитесь, что библиотека BDSP подключена в CMakeLists.txt:

```cmake
target_link_libraries(aggregator_server PUBLIC
    Qt6::Core
    Qt6::Network
    BDSP
)
```

## Тестирование

1. Запустите сервер:
```bash
./aggregator_server_app
```

2. Запустите тестовый клиент:
```bash
./aggregator_bdsp_test_client
```

## Протокол взаимодействия

### Регистрация компонента

1. Клиент подключается к серверу по TCP
2. Клиент отправляет пакет `component_registration` с бинарными данными компонента:
   - Тип компонента (1 байт)
   - Адрес (строка с длиной)
   - Порт (2 байта)
   - Имя компонента (строка с длиной)
3. Сервер отвечает пакетом `component_registration` с результатом:
   - Флаг успеха (1 байт)
   - Сообщение об ошибке (строка с длиной, если неуспешно)
4. При успешной регистрации клиент начинает отправлять heartbeat пакеты

### Heartbeat

- Клиент отправляет пакеты `heartbeat` каждые 100ms
- Сервер отвечает пакетами `heartbeat` для подтверждения
- При отсутствии heartbeat в течение 500ms компонент помечается как неактивный

### События

- Сервер рассылает пакеты `event_notification` всем подключенным клиентам
- События включают подключение/отключение компонентов, изменения состояния
- Структура события:
  - Тип события (1 байт)
  - UUID компонента (16 байт)
  - Новое состояние (1 байт)
  - Временная метка (8 байт)
- Сервер отвечает пакетами `heartbeat` для подтверждения
- При отсутствии heartbeat в течение 500ms компонент помечается как неактивный

### События

- Сервер рассылает пакеты `event_notification` всем подключенным клиентам
- События включают подключение/отключение компонентов, изменения состояния

## Логирование

Используются категории логирования:
- `aggregator.server.protocol` - протокол сервера
- `aggregator.client` - клиентская часть

Включение отладочного логирования:
```cpp
QLoggingCategory::setFilterRules("aggregator.*.debug=true");
```

## Расширение

Для добавления новых типов пакетов:

1. Добавьте новый тип в `PacketType`
2. Реализуйте сериализацию/десериализацию в `TcpBDSPSocket`
3. Добавьте обработку в `handle_packet()`
4. Обновите валидацию в `PacketValidator`
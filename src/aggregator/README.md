# Модуль Aggregator Server

## Описание

Модуль сервера агрегации для отслеживания доступности и состояния всех компонентов системы обработки данных.

## Компоненты

### 1. Библиотека aggregator_server

Статическая библиотека, содержащая:
- Типы и интерфейсы для работы с агрегатором
- Реализацию сервера агрегации
- Мониторинг heartbeat

### 2. Исполняемый файл aggregator_server_app

Сервер агрегации, который:
- Слушает на статическом порту 8212 (или пользовательском через -p)
- Регистрирует компоненты системы
- Отслеживает доступность через heartbeat (PING/PONG)
- Публикует события подключения/отключения компонентов
- Предоставляет список активных компонентов

**Использование:**
```bash
aggregator_server_app -s                    # Запуск на порту 8212
aggregator_server_app --start --component_server_port 9000   # Запуск на порту 9000
aggregator_server_app -h                    # Справка
```

### 3. Тестовый клиент aggregator_test_client

Тестовое приложение для проверки работы сервера:
- Генерирует уникальный UUID v4
- Подключается к серверу агрегации
- Регистрируется как компонент
- Отправляет PING каждые 100 мс
- Запрашивает список компонентов
- Автоматически переподключается при разрыве соединения

**Использование:**
```bash
aggregator_test_client                      # Подключение к localhost:8212
aggregator_test_client -p 9000              # Подключение к localhost:9000
aggregator_test_client -a 192.168.1.1 -p 8212  # Подключение к другому хосту
```

## Протокол взаимодействия

### Регистрация компонента

**Запрос:**
```
REGISTER:UUID:TYPE:PORT
```

**Ответ:**
```
REGISTER_OK
```
или
```
REGISTER_FAILED:REASON
```

### Heartbeat

**Запрос (каждые 100 мс):**
```
PING:UUID
```

**Ответ:**
```
PONG
```

### Запрос списка компонентов

**Запрос:**
```
GET_COMPONENTS
```

**Ответ (JSON):**
```json
{
  "type": "components_list",
  "components": [
    {
      "uuid": "...",
      "type": 1,
      "address": "127.0.0.1",
      "component_server_port": 9999,
      "state": 2
    }
  ]
}
```

## Типы компонентов

- `0` — unknown
- `1` — data_source (источник данных)
- `2` — data_processor (обработчик данных)
- `3` — data_sink (получатель данных)
- `4` — core (ядро приложения)

## Состояния компонентов

- `0` — unknown
- `1` — connecting (подключается)
- `2` — registered (зарегистрирован)
- `3` — active (активен)
- `4` — inactive (неактивен)
- `5` — disconnected (отключён)

## Конфигурация

Все параметры определены в `include/aggregator_server/abstract/types.h`:

```cpp
namespace config {
    constexpr uint16_t aggregator_port = 8212;
    constexpr uint32_t heartbeat_interval_ms = 100;
    constexpr uint32_t heartbeat_timeout_ms = 500;
    constexpr uint32_t registration_timeout_ms = 1000;
    constexpr uint32_t retry_interval_ms = 5000;
}
```

## Сборка

Модуль собирается автоматически при сборке проекта через CMake:

```bash
cmake -B build -S .
cmake --build build
```

## Запуск

### Сервер агрегации

```bash
./build/src/aggregator/aggregator_server_app -s
```

### Тестовый клиент

```bash
./build/src/aggregator/aggregator_test_client
```

Можно запустить несколько экземпляров тестового клиента для проверки работы с множественными подключениями.

## Примеры использования

### Запуск сервера

## Архитектура

Сервер агрегации реализует следующие интерфейсы:

- `IComponentRegistry` — регистрация и управление компонентами
- `IEventPublisher` — публикация событий
- `IHeartbeatMonitor` — мониторинг heartbeat

Все компоненты взаимодействуют через TCP сокеты с использованием простого текстового протокола для команд и JSON для структурированных данных.

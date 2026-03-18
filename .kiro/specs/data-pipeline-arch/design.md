# Дизайн: Архитектура конвейера обработки данных

## Обзор

Документ описывает технический дизайн архитектуры с разделением на источники, обработчики и получатели данных, а также ядро приложения для управления всеми компонентами.

## Архитектура взаимодействия

**Модель клиент-сервер:**
- **Ядро (Core)** — клиент сокета, управляет соединениями и потоком данных
- **Сервер агрегации (EventAggregatorServer)** — центральный компонент для отслеживания доступности (порт 8212)
- **Источники данных (DataSourceServer)** — серверы, к которым подключается ядро
- **Обработчики данных (DataProcessorServer)** — серверы, к которым подключается ядро
- **Получатели данных (DataSinkServer)** — серверы, к которым подключается ядро

## Структура модулей

```
src/
├── core/                           # Ядро приложения (клиент сокета)
│   ├── CMakeLists.txt
│   ├── include/
│   │   └── core/
│   │       ├── abstract/                     # Типы, интерфейсы, абстракции
│   │       │   ├── types.h                   # Типы данных ядра
│   │       │   └── interfaces.h              # Интерфейсы ядра
│   │       ├── core.h                        # Главный класс ядра
│   │       ├── socket_client.h               # TCP клиент для соединений
│   │       └── logging.h                     # Логирование
│   └── src/
│       ├── core.cpp
│       ├── socket_client.cpp
│       └── logging.cpp
│
├── event_aggregator/               # Сервер агрегации событий
│   ├── CMakeLists.txt
│   ├── include/
│   │   └── event_aggregator/
│   │       ├── abstract/                     # Типы, интерфейсы, абстракции
│   │       │   ├── types.h                   # Типы данных агрегатора
│   │       │   └── interfaces.h              # Интерфейсы агрегатора
│   │       ├── event_aggregator_server.h     # Главный сервер агрегации
│   │       ├── event_aggregator_client.h     # Клиент агрегации
│   │       └── utils.h                       # Утилиты форматирования и токенов
│   └── src/
│       ├── event_aggregator_server.cpp
│       ├── event_aggregator_client.cpp
│       └── utils.cpp
│
├── data_sources/                   # Источники данных (серверы)
│   ├── CMakeLists.txt
│   ├── include/
│   │   └── data_sources/
│   │       ├── abstract/                     # Типы, интерфейсы, абстракции
│   │       │   ├── types.h                   # Типы данных источников
│   │       │   └── interfaces.h              # Интерфейсы источников
│   │       ├── data_source_server.h          # Базовый сервер источника
│   │       ├── signal_generator.h            # Генератор сигналов
│   │       ├── audio_loopback_source.h       # WASAPI loopback
│   │       └── serial_data_source.h          # Источник данных по Serial Port
│   └── src/
│       ├── data_source_server.cpp
│       ├── signal_generator.cpp
│       ├── audio_loopback_source.cpp
│       └── serial_data_source.cpp
│
├── data_processors/                # Обработчики данных (серверы)
│   ├── CMakeLists.txt
│   ├── include/
│   │   └── data_processors/
│   │       ├── abstract/                     # Типы, интерфейсы, абстракции
│   │       │   ├── types.h                   # Типы данных обработчиков
│   │       │   ├── interfaces.h              # Интерфейсы обработчиков
│   │       │   └── mixins.h                  # Миксины для обработчиков
│   │       ├── data_processor_server.h       # Базовый сервер обработчика
│   │       ├── signal_processing/            # Обработчики сигналов и спектра
│   │       │   ├── window_signal_collector.h
│   │       │   ├── window_functions_applier.h
│   │       │   ├── fft_processor.h
│   │       │   ├── amplitude_spectrum_conversion.h
│   │       │   ├── energy_spectrum_conversion.h
│   │       │   ├── volume_normalization.h
│   │       │   ├── spectral_whitening.h
│   │       │   ├── bark_scale_mapper.h
│   │       │   └── mel_scale_mapper.h
│   │       └── spectrogram_builder.h         # Построитель спектрограммы
│   └── src/
│       ├── data_processor_server.cpp
│       ├── signal_processing/
│       │   ├── window_signal_collector.cpp
│       │   ├── window_functions_applier.cpp
│       │   ├── fft_processor.cpp
│       │   ├── amplitude_spectrum_conversion.cpp
│       │   ├── energy_spectrum_conversion.cpp
│       │   ├── volume_normalization.cpp
│       │   ├── spectral_whitening.cpp
│       │   ├── bark_scale_mapper.cpp
│       │   └── mel_scale_mapper.cpp
│       └── spectrogram_builder.cpp
│
├── data_sinks/                     # Получатели данных (серверы)
│   ├── CMakeLists.txt
│   ├── include/
│   │   └── data_sinks/
│   │       ├── abstract/                     # Типы, интерфейсы, абстракции
│   │       │   ├── types.h                   # Типы данных получателей
│   │       │   └── interfaces.h              # Интерфейсы получателей
│   │       ├── data_sink_server.h            # Базовый сервер получателя
│   │       └── visualization/                # Визуализаторы данных
│   │           ├── renderers_2d/             # 2D визуализация
│   │           │   ├── chart_renderer.h      # 2D график
│   │           │   └── line_chart_renderer.h # Линейный график
│   │           └── renderers_3d/             # 3D визуализация
│   │               └── spectrogram_renderer.h# 3D спектрограмма
│   └── src/
│       ├── data_sink_server.cpp
│       └── visualization/
│           ├── renderers_2d/
│           │   ├── chart_renderer.cpp
│           │   └── line_chart_renderer.cpp
│           └── renderers_3d/
│               └── spectrogram_renderer.cpp
│
└── ui/                             # Графический интерфейс
    ├── CMakeLists.txt
    ├── include/
    │   └── ui/
    │       ├── abstract/                     # Типы, интерфейсы, абстракции
    │       │   ├── types.h                   # Типы данных UI
    │       │   └── interfaces.h              # Интерфейсы UI
    │       ├── visual_editor.h               # Визуальный редактор
    │       └── component_inspector.h         # Инспектор компонентов
    └── src/
        ├── visual_editor.cpp
        └── component_inspector.cpp
```

## Организация типов и интерфейсов
### Абстрактные базовые классы

**DataSourceServer** — абстрактный базовый класс для всех источников
- Реализует регистрацию в агрегаторе
- Предоставляет методы для отправки данных
- Требует реализации методов захвата/генерации данных

**DataProcessorServer** — абстрактный базовый класс для всех обработчиков
- Реализует регистрацию в агрегаторе
- Реализует получение данных от источников
- Реализует отправку данных получателям
- Требует реализации метода обработки данных

**DataSinkServer** — абстрактный базовый класс для всех получателей
- Реализует регистрацию в агрегаторе
- Реализует приём данных от обработчиков
- Требует реализации метода потребления данных

## Порты

**Статический порт:**
- **EventAggregatorServer** — порт `8212` (единственный статический порт)

**Динамические порты:**
- Все остальные серверы получают порты динамически
- Порты передаются через агрегатор при регистрации и запросах списка компонентов

## Протокол взаимодействия

**Передача данных:**
- Только бинарная передача данных и сообщений
- Максимальная производительность
- Чётко прописанные размеры и типы данных

**Состояния компонентов:**
Все компоненты системы используют единую систему состояний, определённую в `ComponentState`:

```cpp
/// @brief Состояния компонентов в системе агрегации
/// 
/// Жизненный цикл состояний:
/// unknown -> registered -> inactive -> idle -> active
///                      \-> disconnected -> offline (при длительном отключении)
enum class ComponentState : uint8_t {
    /// @brief Неизвестное состояние (заглушка на случай ошибок)
    unknown = 0,
    
    /// @brief Компонент отключился и длительное время не восстанавливает подключение (более минуты)
    offline = 1,
    
    /// @brief Компонент отключен, если простой достигнет минуты - переходит в offline
    disconnected = 2,
    
    /// @brief Клиент зарегистрирован, но пока ничего не делает и не сообщает
    registered = 3,
    
    /// @brief Клиент сообщил о себе данные и возможности, но пока не готов участвовать в цепочках данных
    inactive = 4,
    
    /// @brief Готов к работе, но активных задач нет, простаивает
    idle = 5,
    
    /// @brief Занят работой
    active = 6
};
```

**Управление состояниями:**
- Сервер агрегации отслеживает состояние всех компонентов
- Компоненты сообщают об изменении своего состояния через пакеты `component_info_update`
- Ядро получает уведомления об изменении состояний и может принимать решения на их основе

**Типы компонентов:**
Система поддерживает следующие типы компонентов:

```cpp
enum class ComponentType : uint8_t {
    unknown = 0,        // Неизвестный тип компонента
    data_source = 1,    // Источник данных (генераторы, захватчики)
    data_processor = 2, // Обработчик данных (FFT, фильтры, преобразователи)
    data_sink = 3,      // Получатель данных (визуализаторы, рекордеры)
    core = 4           // Ядро приложения (управляющий компонент)
};
```

**Константы системы:**
```cpp
namespace aggregator_component {
    static constexpr uint16_t DEFAULT_PORT = 8212;                    // Порт сервера агрегации
    static constexpr uint16_t REGISTRATION_TIMEOUT_MS = 1000;         // Таймаут регистрации
    static constexpr int32_t CLIENT_RECONNECT_TIMEOUT_MS = 200;       // Задержка переподключения
    static constexpr int32_t RECONNECT_TRIES = 3;                     // Количество попыток переподключения
    static constexpr int32_t COMPONENT_OFFLINE_TIMEOUT_MS = 60000;    // Таймаут перехода в состояние offline (60 сек)
}
```

**Структуры данных протокола:**
```cpp
// UUID компонента (128-битный идентификатор)
using uuid_t = QUuid::Id128Bytes;

// Токен сессии для восстановления соединений (CRC16)
using session_token_t = uint16_t;

// Типы пакетов протокола
enum class PacketType : uint8_t {
    error = 0,                    // Пакет ошибки
    component_registration = 1,   // Регистрация компонента
    component_info_update = 2,    // Обновление информации о компоненте (новый компонент)
    component_state_change = 4    // Изменение состояния компонента
};

// Данные регистрации компонента
struct registration_data_t {
    uuid_t uuid;                     // Уникальный идентификатор компонента
    ComponentType component_type;    // Тип компонента
    uint16_t component_server_port;  // Порт сервера компонента
    session_token_t session_token;   // Токен сессии для восстановления
} __attribute__((packed));

// Изменение состояния компонента
struct component_state_t {
    uuid_t uuid;                     // Идентификатор компонента
    ComponentState new_state;        // Новое состояние
} __attribute__((packed));

// Полная информация о компоненте
struct component_info_t {
    uuid_t uuid{};                              // Идентификатор компонента
    ComponentType type = ComponentType::unknown; // Тип компонента
    QIPv6Address address{};                     // IP-адрес компонента
    uint16_t component_server_port{};           // Порт сервера компонента
    ComponentState state = ComponentState::unknown; // Текущее состояние
} __attribute__((packed));

// Зарегистрированный компонент (внутренняя структура агрегатора)
struct registered_component_t {
    TcpBDSPSocket *socket;           // TCP-соединение с компонентом
    ComponentType type;              // Тип компонента
    uint16_t component_server_port;  // Порт сервера компонента
    ComponentState state;            // Текущее состояние
    session_token_t session_token;   // Токен сессии для восстановления
};
```

## Алгоритмы поведения компонентов

### 1. Регистрация компонента в агрегаторе

**Алгоритм:**
```
1. Компонент запускается и слушает на динамическом порту
   - Состояние: unknown
   - Генерирует токен сессии на основе CRC16 от UUID, типа, порта и случайного числа
2. Устанавливает TCP-соединение с сервером агрегатора (порт 8212)
3. Отправляет сообщение регистрации с информацией о компоненте
   - Пакет: component_registration
   - Данные: registration_data_t (uuid, component_type, component_server_port, session_token)
4. Агрегатор проверяет регистрацию:
   - Если UUID новый: регистрирует компонент
   - Если UUID существует и токен совпадает: восстанавливает сессию
   - Если UUID существует и токен не совпадает: отклоняет регистрацию
5. При успешной регистрации/восстановлении:
   - Состояние: registered
   - Агрегатор отправляет подтверждение через component_state_change
   - Агрегатор рассылает уведомление о новом компоненте через component_info_update (только для новых)
6. Если таймаут или ошибка:
   - Состояние: disconnected
   - Соединение разрывается
   - Логируется ошибка
   - Повторная попытка через CLIENT_RECONNECT_TIMEOUT_MS с тем же токеном сессии
```

### 2. Переходы состояний компонента

**Алгоритм изменения состояния:**
```
1. Компонент определяет необходимость изменения состояния
2. Обновляет внутреннее состояние
3. Отправляет пакет component_state_change агрегатору
   - Данные: component_state_t (uuid, new_state)
4. Агрегатор обновляет информацию о компоненте
5. Агрегатор рассылает уведомление об изменении состояния всем подписчикам через component_state_change
```

**Автоматические переходы:**
- `registered` → `inactive`: при отправке информации о возможностях
- `inactive` → `idle`: при готовности к приёму данных
- `idle` → `active`: при начале обработки данных
- `active` → `idle`: при завершении обработки данных
- любое → `disconnected`: при разрыве TCP-соединения
- `disconnected` → `offline`: через 60 секунд отсутствия соединения (с удалением из реестра)

**Восстановление сессий:**
- При переподключении с тем же UUID и токеном сессии: `disconnected` → `registered`
- Агрегатор сохраняет токены сессии для проверки подлинности
- Неверный токен приводит к отклонению восстановления сессии

### 3. Публикация состояний агрегатором

**Функции агрегатора:**
- Хранение состояния всех зарегистрированных компонентов
- Рассылка уведомлений при изменении состояния
- Выдача списка активных компонентов по запросу
- Подписка на события конкретных или всех компонентов

**Типы событий:**
- Компонент подключился (новый компонент)
- Компонент восстановил сессию (переподключение)
- Компонент отключился
- Изменилась доступность компонента
- Изменился статус компонента
- Поток данных открыт
- Поток данных закрыт

**Безопасность и логирование:**
- Все сообщения используют текстовые представления состояний и типов компонентов
- Токены сессии не логируются для обеспечения безопасности
- Унифицированный формат сообщений с UUID для всех компонентов
- Функции форматирования: `format_component_info()`, `format_component_state_change()`, `format_component_registration()`

## Конфигурация таймаутов

- Порт агрегатора: 8212 (`DEFAULT_PORT`)
- Таймаут регистрации: 1000 мс (`REGISTRATION_TIMEOUT_MS`)
- Автопереподключение: включено
- Количество попыток подключения: 3 (`RECONNECT_TRIES`)
- Задержка переподключения в случае разрыва соединения: 200 мс (`CLIENT_RECONNECT_TIMEOUT_MS`)
- Переход в состояние `offline`: 60 секунд отсутствия соединения (`COMPONENT_OFFLINE_TIMEOUT_MS`)
- Удаление из реестра при переходе в `offline` для освобождения UUID
- Поддержка восстановления сессий через CRC16 токены

## Режимы запуска

**Графический режим:**
- UI для визуальной настройки
- Drag-and-drop соединения
- Кнопки фокусировки на компонентах

**Консольный режим:**
- Быстрая сборка
- Подключение через командную строку
- Логирование в консоль

## Обработка ошибок

**Ядро:**
- Проверка доступности серверов
- Автоматическое переподключение
- Таймауты соединений

**Уровни логирования:**
- `INFO` — соединения, подписки
- `WARNING` — таймауты, переподключения
- `ERROR` — потеря соединения, ошибки серверов

## Применение архитектурных паттернов

### Паттерны в структуре модулей

**Factory Pattern:**
- Каждый модуль может содержать фабрику для создания компонентов
- Пример: `DataSourceFactory` для создания различных источников данных

**Strategy Pattern:**
- Различные обработчики реализуют разные стратегии обработки
- Пример: `FFTProcessor`, `BarkScaleMapper` — разные стратегии анализа спектра

**Adapter Pattern:**
- Адаптеры преобразуют разные API к единому интерфейсу
- Пример: `WASAPIAdapter` для адаптации WASAPI к `IDataSource`

**Decorator Pattern:**
- Миксины добавляют функциональность без изменения основного класса
- Пример: `LoggingMixin` добавляет логирование к любому компоненту

**Observer Pattern:**
- Компоненты подписываются на события от агрегатора
- Пример: компонент получает уведомление об отключении другого компонента

**Composite Pattern:**
- Цепочки обработчиков представляются как единый обработчик
- Пример: `ProcessorChain` содержит несколько обработчиков

**Facade Pattern:**
- Ядро (Core) предоставляет простой интерфейс для управления всеми компонентами
- Скрывает сложность взаимодействия между компонентами

### Применение SOLID принципов

**Single Responsibility:**
- Каждый класс отвечает за одну задачу
- Пример: `WindowSignalCollector` только собирает сигналы в окно

**Open/Closed:**
- Новые компоненты добавляются без изменения существующего кода
- Пример: новый обработчик наследует `IDataProcessor` и добавляется в систему

**Liskov Substitution:**
- Все реализации интерфейса взаимозаменяемы
- Пример: любой источник может быть заменён на другой

**Interface Segregation:**
- Интерфейсы разделены по функциональности
- Пример: `IDataSource`, `IDataProcessor`, `IDataSink` — разные интерфейсы

**Dependency Inversion:**
- Зависимости от абстракций, а не от конкретных реализаций
- Пример: ядро зависит от `IDataSource`, а не от конкретного источника


## Потоки данных

### Сценарий 1: Генератор → Обработчики → Визуализация

```
SignalGenerator → WindowSignalCollector → FFT → AmplitudeSpectrumConversion → ChartRenderer
```

**Применяемые паттерны:**
- **Factory:** SignalGenerator создаётся через SignalGeneratorFactory
- **Strategy:** Каждый обработчик реализует свою стратегию обработки
- **Chain of Responsibility:** Данные проходят через цепочку обработчиков
- **Observer:** ChartRenderer подписан на события от FFT

### Сценарий 2: Loopback → Обработчики → Визуализация

```
AudioLoopbackSource → WindowSignalCollector → WindowFunctionsApplier → FFT → EnergySpectrumConversion → MelScaleMapper → SpectrogramBuilder → SpectrogramRenderer
```

**Применяемые паттерны:**
- **Adapter:** AudioLoopbackSource адаптирует WASAPI API к IDataSource
- **Decorator:** Логирование и метрики добавляются через миксины
- **Strategy:** Каждый обработчик использует свой алгоритм
- **Composite:** Цепочка обработчиков представляется как единый обработчик

### Сценарий 3: ESP32 → Визуализация

```
SerialDataSource (BDSP) → ChartRenderer
```

**Применяемые паттерны:**
- **Adapter:** SerialDataSource адаптирует Serial Port API к IDataSource
- **Factory:** SerialDataSource создаётся через DataSourceFactory

### Сценарий 4: Множественные параллельные обработчики

```
SignalGenerator → WindowSignalCollector → WindowFunctionsApplier → FFT → AmplitudeSpectrumConversion → ChartRenderer
                                                                      ↓
                                                    EnergySpectrumConversion → MelScaleMapper → SpectrogramBuilder → SpectrogramRenderer
```

**Применяемые паттерны:**
- **Composite:** FFT имеет несколько выходов (AmplitudeSpectrumConversion и EnergySpectrumConversion)
- **Observer:** Оба получателя подписаны на события от FFT
- **Mediator:** Ядро управляет маршрутизацией данных между компонентами

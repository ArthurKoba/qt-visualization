---
inclusion: force
fileMatchPattern: '**/*.cpp,**/*.h,**/*.hpp'
---

# C++ Стандарты кодирования

## Обзор
Этот документ определяет стандарты кодирования специфично для C++ кода в проекте.

## Основные принципы

- Язык только C++, вставки чистого C кода нежелательны.
- Целевая платформа: Windows (win32)
- Версия Qt: 6.9.2 или Qt 6.10
- Не используй реализации на основе QT QML, лучше все делать нативно в стиле С++.

## Типы данных

### Размерность типов

**Принцип:** Все типы данных должны иметь явно определенную размерность.

**Правило:**
- **НИКОГДА не используй** `int`, `unsigned int` и подобные неявные, архитектурнозависимые типы
- **ВСЕГДА используй** типы с понятной размерностью: `uint8_t`, `int32_t`, `uint64_t` и т.д.

**Пример:**

```cpp
// ПРАВИЛЬНО
uint8_t byte_value = 255;
int32_t counter = 0;
uint64_t timestamp = 0;

// НЕПРАВИЛЬНО
unsigned char byte_value = 255;
int counter = 0;
unsigned long timestamp = 0;
```

### Специальные типы

**Для IP адресов:**
- **ВСЕГДА используй `QHostAddress`** вместо строк

```cpp
QHostAddress server_address("192.168.1.1");
QHostAddress ipv6_address("::1");
```

**Для UUID:**
- **ВСЕГДА используй `QUuid`** вместо строк

```cpp
QUuid unique_id = QUuid::createUuid();
QUuid parsed_id = QUuid::fromString("{12345678-1234-5678-1234-567812345678}");
```

## Соглашения об именовании

### Переменные и функции

- Использовать **snake_case** для переменных, функций, методов
- Название должно в полной мере отображать суть данных, функции или метода, не рекомендуется использовать общие и неконкретные названия. Нельзя делать именование общим и неконкретны. ВНИМАНИЕ, если именование получается слишком длинным, то это сигнал о необходимости произвести декомпозицию сущности и разбить данные на структуры, а процедуру на подметоды. 
- Если переменная хранит длительность и периоды, связанные со временем, обязательно должен быть префикс "_ms" для миллисекунд, "_us" для микросекунд , "_sec" для секунд и т.д.

**Пример:**

```cpp
int32_t tcp_socketconnection_timeout_ms = 5000;
QString user_email_address;
void process_incoming_protocol_data();
bool validate_user_registration_input_parameters();
```

### Классы и структуры

- **Классы:** PascalCase
- **Структуры:** snake_case + `_t` суффикс

**Пример:**

```cpp
class DataProcessor { };
class NetworkServer { };

struct user_information_t { };
struct connection_config_t { };
```

### Файлы

- Файлы исходного кода (.h, .cpp) используют **snake_case**
- Классы внутри файлов используют **PascalCase**

**Пример:**

```
aggregator_server.h    // файл
class AggregatorServer { };  // класс внутри файла

data_processor.cpp     // файл
class DataProcessor { };     // класс внутри файла
```

## Члены класса по уровню доступа

### Правило именования

**Принцип:** Члены класса должны иметь префиксы, которые явно указывают на уровень доступа.

**Правило:**
- **Приватные и защищенные поля и методы** — начинать с префикса `_` (одиночное  подчеркивание)
- **Публичные члены** — без префиксов, используется обычный snake_case

### Организация членов класса

**Порядок в определении класса:**
1. Публичные поля (если есть)
2. Публичные методы
3. Защищенные поля
4. Приватные поля
5. Приватные методы
6. Защищенные методы


### Хороший пример реализации (порядки определения, именования, инкапсуляция)

```cpp
class DataProcessor final {
public: // Публичные поля (редко)
    static const int32_t MAX_BUFFER_SIZE = 1024;
public: // Публичные методы
    void process_data(const QByteArray& data);
    QString get_status() const;
    bool is_connected() const;

protected: // Защищенные поля
    QVector<uint8_t> _cache;
    bool _is_initialized;
    int32_t _retry_count;
private: // Приватные поля
    
    int32_t _buffer_size;
    QString _internal_state;
    QVector<uint8_t> _cache;
protected:// Приватные методы
    void _notify_socket_observers();
    void _on_server_state_changed() override;
    void _reset_clietn_state(Client *client);
private:  // Приватные методы
    void _validate_input();
    bool _process_internal_data() override;
    void _cleanup_resources();
};
```

## Модификатор final

**Принцип:** Все конечные реализации классов должны быть помечены модификатором `final` для применения верных оптимизаций компилятором (devirtualization).

**Правило:**
- Все конкретные реализации (классы, которые используются напрямую) **ДОЛЖНЫ** иметь модификатор `final`
- Классы БЕЗ модификатора `final` можно использовать ТОЛЬКО если:
  - От них происходит наследование (промежуточные абстрактные классы)
  - Они не используются напрямую, а служат только базой для других классов
- Единственное исключение: классы без `final` можно создавать при написании тестов

**Пример:**

```cpp
// Абстрактный базовый класс - БЕЗ final
class EventHandler {
public:
    virtual ~EventHandler() = default;
    virtual void handle_event(const event_t& event) = 0;
};

// Конкретная реализация - С final
class NetworkEventHandler final : public EventHandler {
public:
    void handle_event(const event_t& event) override;
};

// Еще одна конкретная реализация - С final
class FileEventHandler final : public EventHandler {
public:
    void handle_event(const event_t& event) override;
};
```

## Типы данных и касты

### Явные касты

**Принцип:** Использовать явные и читаемые конструкции для кастов.

**Правило:**
- Используй `static_cast<>()` для безопасных преобразований типов
- Используй `reinterpret_cast<>()` для преобразований между несвязанными типами
- Используй `const_cast<>()` для удаления const квалификатора (редко)
- Используй `dynamic_cast<>()` для полиморфных типов

**Пример:**

```cpp
// ПРАВИЛЬНО
int32_t value = static_cast<int32_t>(floating_point_value);
void* ptr = reinterpret_cast<void*>(address);
uint8_t* data = reinterpret_cast<uint8_t*>(buffer);

// НЕПРАВИЛЬНО
int32_t value = (int32_t)floating_point_value;
int32_t value2 = int32_t(floating_point2_value);
void* ptr = (void*)address;
```

## Операторы

### ISO646 альтернативные операторы

**Принцип:** Для логических операций и побитовых операций используй ISO646 альтернативные операторы.

**Правило:**
- Используй `and` вместо `&&`
- Используй `or` вместо `||`
- Используй `not` вместо `!`
- Используй `bitand` вместо `&`
- Используй `bitor` вместо `|`
- Используй `xor` вместо `^`
- Используй `compl` вместо `~`

**Пример:**

```cpp
// ПРАВИЛЬНО - используй ISO646
if (is_valid and not is_empty) {
    process_data();
}

if (flag_a or flag_b) {
    handle_event();
}

uint32_t result = value1 bitand value2;
uint32_t combined = value1 bitor value2;
uint32_t toggled = value1 xor value2;
uint32_t inverted = compl value;

// НЕПРАВИЛЬНО - не используй символьные операторы
// if (is_valid && !is_empty) { ... }
// if (flag_a || flag_b) { ... }
```



## Комментарии и документация

### Стиль комментариев

- Добавляй комментарии для сложной логики
- Оформление комментариев должно быть **СТРОГО в стиле Doxygen**
- Не пиши бесполезные комментарии (например, "добавляю модуль X" перед `add_subdirectory(X)`)

**Пример:**

```cpp
/// @brief Обрабатывает входящие данные из сокета
/// @param data Буфер с данными для обработки
/// @param size Размер буфера в байтах
/// @return true если обработка успешна, false в противном случае
bool process_socket_data(const uint8_t* data, int32_t size);

/// @class DataProcessor
/// @brief Основной класс для обработки сигналов
/// 
/// Этот класс отвечает за:
/// - Получение данных из источника
/// - Применение фильтров
/// - Вычисление спектра
class DataProcessor final {
    // ...
};
```

## Система логирования

**Принцип:** Все логирование и вывод должны использовать Qt логгеры. Все сообщения должны быть на английском языке.

**Правило:**
- **ВСЕГДА используй Qt логгеры** для всех операций: `qDebug()`, `qInfo()`, `qWarning()`, `qCritical()`, `qFatal()`
- **НИКОГДА не используй** `std::cout`, `std::cerr` или `printf()`
- **ВСЕГДА используй форматируемое логирование** вместо оператора сдвига (`<<`)
- Для категоризированного логирования используй `qCDebug()`, `qCInfo()`, `qCWarning()`, `qCCritical()`, `qCFatal()` с `QLoggingCategory`
- Определяй категории логирования в заголовочных файлах модулей с помощью `Q_DECLARE_LOGGING_CATEGORY()` и реализуй их в `.cpp` файлах с помощью `Q_LOGGING_CATEGORY()`
- Названия категорий следуют соглашению: `module.submodule.component` (например: `aggregator.server.event_handler`)
- Для QString используй `qPrintable()` для преобразования в const char*
- Для QUuid используй `qPrintable(uuid.toString())`

**Разделение логирования по этапам запуска:**
1. **Этап инициализации и парсинга параметров (до успешного запуска) - БЕЗ категорий:**
   - Примеры: справка (help), ошибки парсинга аргументов, информация о конфигурации
2. **После успешного запуска приложения (runtime) - C категорий:**
   - Примеры: операции сервера, обработка событий, ошибки во время работы

**Примеры:**

```cpp
// В заголовочном файле (include/module/server.h)
#include <QLoggingCategory>
Q_DECLARE_LOGGING_CATEGORY(module_server)

// В файле реализации (src/aggregator.cpp)
#include <QLoggingCategory>
Q_LOGGING_CATEGORY(module_server, "module.server")

// ЭТАП ИНИЦИАЛИЗАЦИИ - БЕЗ категорий, белый цвет
qInfo("Usage: app [options]");
qInfo("Options:");
qInfo("  --help     Show this help message");
qInfo("Error: invalid argument");

// ПОСЛЕ УСПЕШНОГО ЗАПУСКА - С категориями
qCDebug(module_server, "Server started on component_server_port: %d", component_server_port);
qCWarning(module_server, "Client connection error: %s", qPrintable(error_message));
qCCritical(module_server, "Critical error: %s", qPrintable(critical_error));
qCInfo(module_server, "Event received: %d for component: %s", 
       static_cast<int32_t>(event.type), qPrintable(event.uuid.toString()));

// НЕПРАВИЛЬНО - не используй оператор сдвига
// qCDebug(aggregator) << "Server started on component_server_port:" << component_server_port;

// НЕПРАВИЛЬНО - не используй std::cout/std::cerr
// std::cout << "Usage information...\n";
// std::cerr << "Error: invalid argument\n";
```

## Качество кода

### Проверки перед коммитом

- Убедись, что в изменённых файлах нет синтаксических ошибок и приложение собирается
- Проверь возможные утечки памяти
- Убедись, что все логи используют систему логирования Qt, а не прямой вывод в консоль
- Проверь, что все классы-реализации имеют модификатор `final`
- Убедись, что все члены класса имеют правильные префиксы доступа
- Убедитесь, что все логи используют систему логирования Qt, а не прямой вывод в консоль


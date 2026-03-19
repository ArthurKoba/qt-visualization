---
inclusion: fileMatch
fileMatchPattern: '**/*.cpp,**/*.h'
description: "Стандарты кодирования C++: типы данных с явной размерностью, соглашения об именовании, члены класса с префиксами доступа, модификатор final, явные касты, ISO646 операторы, комментарии Doxygen, система логирования Qt. Подключать при написании и редактировании C++ кода для соблюдения стиля проекта."
---

# C++ Стандарты кодирования

## Обзор
Этот документ определяет стандарты кодирования специфично для C++ кода в проекте.

## Основные принципы

- Язык только C++, вставки чистого C кода нежелательны.
- Целевая платформа: Windows (win32)
- Версия Qt 6.10.2
- Не используй реализации на основе QT QML, лучше все делать нативно в стиле С++.

## Системные импорты

### **Порядок**
1. В начале списка должны следовать системные импорты из предустановленных библиотек, в частности речь о std.
2. Второй блок списка должен содержать импорты модулей Qt, основного фремворка данного проекта.
3. Третий блок должен включать все внешние библиотеки, не являющиеся файлами проекта.
4. В последнем блоке списка должны располагаться импорты файлов проекта в кавычках `#include "module.h"`
### **Примечания**
- Каждый блок импортов должен быть разделен от других блоков пробелом.
- Внутри каждого блока импортов строки должны располагаться без дополнительных пустых строк.
- Внешние импорты должны использовать фигурные символы <>, а внутренние (файлы проекта) кавычки.

```cpp
// внешние зависимости из библиотеки std
#include <cinttypes>

// внешние зависимости - фреймворк Qt
#include <QObject>
#include <QUuid>
#include <QHostAddress>

// внешние зависимости - библиотека скачанная через Fetch content
#include <BDSP/transceivers.h>

// внутренние зависимости - файлы проекта
#include "aggregator/abstract/types.h"
#include "core/tcp_packet_socket.h"
```

## Типы данных

### Очистка данных

Не используй бесполезные конструкции, delete работает безопасно даже с nullptr.
```cpp

// плохо и не имеет смысла
if (_reconnect_tries != nullptr) {
    delete _reconnect_tries;
}

// отлично
delete _reconnect_tries;
```

### Размерность типов

**Принцип:** Все типы данных должны иметь явно определенную размерность.

**Правило:**
- **НИКОГДА не используй** `int`, `unsigned int` и подобные неявные, архитектурно-зависимые типы
- **ВСЕГДА используй** типы с понятной размерностью: `uint8_t`, `int32_t`, `uint64_t` и т.д.
ИСКЛЮЧЕНИЕ!!! результат выполнения функции входа (main) приложения нужно оставить в формате int.

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
- Если переменная хранит длительность и периоды, связанные со временем, обязательно должен быть префикс "_ms" для миллисекунд, "_us" для микросекунд , "_sec" для секунд и т.д. Для констант можно использовать префиксы с большой буквы, например "_MS", "_US" и т.д.
- Константы именуются только большими буквами и разделяются всегда нижним подчеркиванием. Например "REQUEST_TIMEOUT_MS"

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
- **Приватные и защищенные поля и методы** — начинать с префикса `_` (одиночное подчеркивание)
- **Публичные члены** — без префиксов, используется обычный snake_case

### Организация членов класса

**Порядок в определении класса:**
1. Публичные поля и методы
2. Защищенные поля и методы
3. Приватные поля и методы
4. Сначала лучше размещать общие для класса поля, а потом методы.
5. Если поле используется только в одном или нескольких методах, его лучше разместить рядом с ними для читаемости, но самое главное чтобы он был определен выше самого первого метода который его использует.


### Хороший пример реализации (порядки определения, именования, инкапсуляция)

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
#include <QObject>
#include <QNetworkReply>
#include <QTimer>
#include <QString>
#include <QMap>

// Интерфейс — БЕЗ final
class INetworkEventHandler {
public:
    virtual ~INetworkEventHandler() = default;

    virtual void handle_event(const QNetworkReply* reply) = 0;
    virtual void handle_error(const QNetworkReply::NetworkError& error) = 0;
};


// Абстрактный класс — БЕЗ final
class AbstractNetworkEventHandler : public QObject, public INetworkEventHandler {
    Q_OBJECT

public:
    explicit AbstractNetworkEventHandler(QObject* parent = nullptr);
    ~AbstractNetworkEventHandler() override = default;

    // Общее для класса поле — максимальное число попыток
    int max_retry_count;

    void handle_event(const QNetworkReply* reply) override;
    void handle_error(const QNetworkReply::NetworkError& error) final;

protected:
    // Общие защищённые поля
    QString _base_url;
    int     _retry_count;

    // Метод и поле, используемое только в retry-логике — рядом друг с другом
    QTimer* _retry_timer;
    virtual void _schedule_retry();

    virtual void _process_reply(const QNetworkReply* reply) = 0;

private:
    // Поле используется только в _log_error()
    QString _last_error_message;
    void    _log_error(const QString& message);
};


// Конкретная реализация — С final
class BDSPNetworkEventHandler final : public AbstractNetworkEventHandler {
    Q_OBJECT

public:
    explicit BDSPNetworkEventHandler(const QString& endpoint, QObject* parent = nullptr);
    ~BDSPNetworkEventHandler() override = default;

    // Публичное поле + метод управления endpoint-ом
    QString active_endpoint;

    void handle_event(const QNetworkReply* reply) override;

    void        set_timeout(int milliseconds);
    int         get_timeout() const;
    bool        is_connected() const;

signals:
    void event_processed(const QString& response_data);
    void connection_lost();

protected:
    void _schedule_retry() override;
    void _process_reply(const QNetworkReply* reply) override;

private:
    // Общие приватные поля
    bool _is_connected;

    // Поле и методы timeout-логики — сгруппированы рядом
    int     _timeout_ms;
    QTimer* _timeout_timer;
    void    _start_timeout_timer();
    void    _stop_timeout_timer();

    // Поле и метод парсинга — сгруппированы рядом
    QMap<QString, QString> _response_cache;
    QString                _parse_response(const QByteArray& raw_data);
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

- Добавляй комментарии к коду для очень сложной и неочевидной логики.
- Не пиши комментарии к коду равные по смыслу последующему действию программы. 
  Например, "добавляю модуль X" перед `add_subdirectory(X)`, это очевидно из действия программы. 
  Другим плохим примером будет комментарий `// запускаем сервер`, а далее следует `aggregate_server.start()`
- Оформление комментариев должно быть **СТРОГО в стиле Doxygen**


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
- **Для настройки и управления логированием** используй документацию: `#[[file:.kiro/steering/qt/qt-logging.md]]`

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
- ДЛЯ БИЛДА КАК ПРАВИЛО ИСПОЛЬЗУЕТСЯ КОМАНДА cmake --build cmake-build-debug --target ...
- Проверь возможные утечки памяти
- Убедись, что все логи используют систему логирования Qt, а не прямой вывод в консоль
- Проверь, что все классы-реализации имеют модификатор `final`
- Убедись, что все члены класса имеют правильные префиксы доступа
- Убедитесь, что все логи используют систему логирования Qt, а не прямой вывод в консоль

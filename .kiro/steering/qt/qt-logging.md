---
inclusion: manual
---

# Qt Логирование - Управление и Настройка

## Обзор

Qt предоставляет мощную систему логирования через `QLoggingCategory`, которая позволяет контролировать вывод отладочных сообщений на уровне категорий и типов сообщений.

## Основные концепции

### Типы сообщений Qt

- `qDebug()` - отладочные сообщения (debug level)
- `qInfo()` - информационные сообщения (info level)  
- `qWarning()` - предупреждения (warning level)
- `qCritical()` - критические ошибки (critical level)
- `qFatal()` - фатальные ошибки (fatal level)

### Категоризированное логирование

```cpp
// Объявление категории в заголовочном файле
Q_DECLARE_LOGGING_CATEGORY(category_name)

// Определение категории в .cpp файле
Q_LOGGING_CATEGORY(category_name, "category.string.name")

// Использование
qCDebug(category_name) << "Debug message";
qCInfo(category_name) << "Info message";
qCWarning(category_name) << "Warning message";
qCCritical(category_name) << "Critical message";
```

## Управление логированием

### Через QLoggingCategory::setFilterRules()

**Синтаксис правил:**
```cpp
QLoggingCategory::setFilterRules(
    "category.name.debug=true;"
    "category.name.info=false;"
    "*.warning=true;"
    "qt.*.debug=false"
);
```

**Правила форматирования:**
- Разделитель правил: `;` (точка с запятой)
- Формат: `category.level=true/false`
- Поддержка wildcards: `*` и `?`
- Порядок правил важен - последнее правило имеет приоритет

**Синтаксис правил:**
```cpp
QLoggingCategory::setFilterRules(
    "category.name.debug=true\n"
    "category.name.info=false\n"
    "*.warning=true\n"
    "qt.*.debug=false"
);
```

**Правила форматирования:**
- Разделитель правил: `\n` (символ новой строки) в коде
- Разделитель правил: `;` (точка с запятой) в переменных окружения
- Формат: `category.level=true/false`
- Поддержка wildcards: `*` и `?`
- Порядок правил важен - последнее правило имеет приоритет

### 2. Условная компиляция для продакшн

```cpp
#ifdef QT_NO_DEBUG_OUTPUT
    // В release сборке debug логи полностью исключены из кода
#else
    qCDebug(category) << "Debug info";
#endif
```

### Отключение Qt внутренних логов

```cpp
// В коде приложения
QLoggingCategory::setFilterRules(
    "qt.widgets.painting.*=false;"
    "qt.qpa.*=false;"
    "qt.*.debug=false"
);
```

```bash
# Через переменную окружения
export QT_LOGGING_RULES="qt.widgets.painting.*=false;qt.qpa.*=false;qt.*.debug=false"
```

### Включение только определенных категорий

```cpp
QLoggingCategory::setFilterRules(
    "*.debug=false;"           // Отключить все debug
    "myapp.*.debug=true;"      // Включить debug для myapp
    "*.info=true;"             // Включить все info
    "*.warning=true;"          // Включить все warning
    "*.critical=true"          // Включить все critical
);
```

### Настройка для разработки vs продакшн

```cpp
void setupLogging(bool isDebugBuild) {
    if (isDebugBuild) {
        // Разработка - больше логов
        QLoggingCategory::setFilterRules(
            "myapp.*.debug=true;"
            "myapp.*.info=true;"
            "qt.*.debug=false;"    // Qt логи отключены
            "*.warning=true;"
            "*.critical=true"
        );
    } else {
        // Продакшн - минимум логов
        QLoggingCategory::setFilterRules(
            "*.debug=false;"
            "*.info=false;"
            "*.warning=true;"
            "*.critical=true"
        );
    }
}
```

## Проблемные Qt категории

### Часто встречающиеся "шумные" категории:

- `qt.widgets.painting.*` - отрисовка виджетов (очень много сообщений)
- `qt.qpa.input.devices.*` - события ввода
- `qt.qpa.events.*` - системные события
- `qt.quick.hover.trace.*` - отслеживание hover в QML
- `qt.network.*` - сетевые операции
- `qt.sql.*` - SQL операции

### Рекомендуемые правила для отключения шума:

```cpp
QLoggingCategory::setFilterRules(
    // Отключить шумные Qt категории
    "qt.widgets.painting.*=false;"
    "qt.qpa.input.devices.*=false;"
    "qt.qpa.events.*=false;"
    "qt.quick.hover.trace.*=false;"
    
    // Включить важные уровни
    "*.warning=true;"
    "*.critical=true;"
    
    // Настроить приложение
    "myapp.*.debug=true;"
    "myapp.*.info=true"
);
```

## Форматирование вывода

### Переменная QT_MESSAGE_PATTERN

```bash
# Подробный формат с временем и категорией
export QT_MESSAGE_PATTERN="%{time yyyy.MM.dd hh:mm:ss.zzz} [%{type}] %{category}: %{message}"

# Цветной вывод (Linux/macOS)
export QT_MESSAGE_PATTERN="%{time hh:mm:ss.zzz} %{if-debug}\033[34m[D]\033[0m%{endif}%{if-info}[I]%{endif}%{if-warning}\033[33m[W]\033[0m%{endif}%{if-critical}\033[31m[C]\033[0m%{endif} %{category}: %{message}"
```

### Доступные плейсхолдеры:

- `%{time}` - время (с форматом)
- `%{type}` - тип сообщения (debug, info, warning, critical)
- `%{category}` - категория логирования
- `%{message}` - текст сообщения
- `%{file}`, `%{line}`, `%{function}` - местоположение в коде
- `%{pid}`, `%{appname}` - процесс и имя приложения

## Лучшие практики

### 1. Структура категорий

Используйте иерархическую структуру:
```cpp
Q_LOGGING_CATEGORY(app_main, "myapp.main")
Q_LOGGING_CATEGORY(app_network, "myapp.network")
Q_LOGGING_CATEGORY(app_database, "myapp.database")
Q_LOGGING_CATEGORY(app_ui, "myapp.ui")
```

### 2. Условная компиляция для продакшн

```cpp
#ifdef QT_NO_DEBUG_OUTPUT
    // В release сборке debug логи полностью исключены из кода
#else
    qCDebug(category) << "Debug info";
#endif
```

### 3. Настройка в main()

```cpp
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // Настройка логирования в начале приложения
    setupApplicationLogging();
    
    // Остальной код...
    return app.exec();
}
```

### 4. Использование в разных модулях

```cpp
// В каждом модуле свой заголовочный файл с категориями
// module_logging.h
#include <QLoggingCategory>
Q_DECLARE_LOGGING_CATEGORY(module_network)
Q_DECLARE_LOGGING_CATEGORY(module_processing)

// module_logging.cpp
Q_LOGGING_CATEGORY(module_network, "myapp.module.network")
Q_LOGGING_CATEGORY(module_processing, "myapp.module.processing")
```

## Отладка проблем с логированием

### Проверка активных правил

```cpp
// Вывести все активные категории (только в debug сборке)
qDebug() << "Available categories:";
for (const auto& category : QLoggingCategory::allCategories()) {
    qDebug() << category->categoryName() << "debug:" << category->isDebugEnabled();
}
```

### Тестирование правил

```cpp
// Тестовая функция для проверки логирования
void testLogging() {
    qCDebug(test_category) << "This is debug";
    qCInfo(test_category) << "This is info";  
    qCWarning(test_category) << "This is warning";
    qCCritical(test_category) << "This is critical";
}
```

## Заключение

Правильная настройка логирования Qt критически важна для:
- Производительности приложения (избежание лишнего вывода)
- Удобства отладки (фокус на важных сообщениях)
- Профессионального вида приложения (отсутствие технического шума)

Всегда настраивайте логирование в начале разработки и регулярно пересматривайте правила по мере роста проекта.
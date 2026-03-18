---
inclusion: fileMatch
fileMatchPattern: '**/CMakeLists.txt'
description: "Архитектурные стандарты и CMake: структура файлов (include/src разделение), Header Guards, соответствие структуры папок, организация типов в abstract/, иерархия интерфейсов, миксины, стандарты оформления CMakeLists.txt. Подключать при создании новых модулей, редактировании CMakeLists.txt, организации структуры проекта, решении проблем сборки приложения."
---

# Стандарты Архитектуры
## Структура файлов
- В корневой директории репозитория для исходных кодов (src) не стоит добавлять какой-то функционал, лучше сделать подмодуль и импортировать его CMakeLists.txt.
- **ВСЕГДА** разделяй источники (.h) и реализацию (.cpp) для файлов
- Заголовки модуля лучше хранить в  : `src/*module-name*/include`
- Реализации для заголовков модулей лучше хранить в: `src/*module-name*/src`

### Header Guards

- Используй **Header Guards** вместо `#pragma once`
- Формат: `MODULE_NAME_FILENAME_H`

**Пример:**

```cpp
// aggregator_server.h
#ifndef AGGREGATOR_SERVER_H
#define AGGREGATOR_SERVER_H

// содержимое

#endif // AGGREGATOR_SERVER_H
```

### Правило соответствия структуры include и src

**Принцип:** Структура папок в `*module-name*/src/` должна точно соответствовать структуре папок в `*module-name*/include/*module-name*/`.

**Правило:** Если заголовочный файл находится в подпапке `include/`, то его реализация должна находиться в соответствующей подпапке `src/`.

**Примеры:**

| Заголовочный файл | Файл реализации |
|------------------|-----------------|
| `include/core/core.h` | `src/core/core.cpp` |
| `include/data_processors/signal_processing/fft_processor.h` | `src/signal_processing/fft_processor.cpp` |
| `include/visualization/renderers_2d/chart_renderer.h` | `src/visualization/renderers_2d/chart_renderer.cpp` |

## Организация типов и интерфейсов

### Структура подпапки `abstract/`

Каждый модуль содержит подпапку `abstract/` в `include/module-name/` для хранения общих компонентов:

```
include/module_name/
├── abstract/                       # Типы, интерфейсы, абстракции
│   ├── types.h                     # Типы данных модуля
│   ├── interfaces.h                # Интерфейсы модуля
│   └── mixins.h                    # Миксины (если требуются)
├── implementation1.h               # Конкретные реализации
├── implementation2.h
└── subfolder/                      # Подпапки для группировки реализаций
    └── implementation3.h
```

**Содержимое `abstract/`:**
- `types.h` — структуры, перечисления, константы, типы данных
- `interfaces.h` — интерфейсы (чистые виртуальные классы)
- `mixins.h` — миксины для добавления функциональности (логирование, метрики и т.д.)

**Назначение подпапки `abstract/`:**
- Централизованное хранение всех типов и интерфейсов модуля
- Четкое разделение между абстракциями и реализациями
- Упрощение навигации и поиска определений

### Правило: типы и интерфейсы без реализации

**Важно:** Файлы в `abstract/` содержат только определения, без реализации:
- Типы — это структуры, перечисления, константы
- Интерфейсы — это чистые виртуальные классы (только объявления методов)
- Миксины — это шаблоны для добавления функциональности
- **Реализация находится в .cpp файлах**, соответствующих классам, которые наследуют интерфейсы

**Пример:**
```
include/data_processors/
├── abstract/
│   ├── types.h                     # Определение struct SignalFrame
│   └── interfaces.h                # Определение class IDataProcessor
├── fft_processor.h                 # Наследует IDataProcessor
└── ...

src/
├── fft_processor.cpp               # Реализация FFTProcessor
└── ...
```

### Когда разделять типы и интерфейсы

На начальном этапе разработки можно размещать типы и интерфейсы в одном файле `types.h`:

```cpp
// include/module_name/abstract/types.h

// Типы данных
struct ComponentInfo { ... };
enum class ComponentState { ... };

// Интерфейсы
class IComponent { ... };
class IDataProcessor { ... };
```

Разделение на `types.h` и `interfaces.h` требуется когда:
- Файл становится слишком большим (>500 строк)
- Типы и интерфейсы логически разделяются на несколько групп
- Требуется лучшая организация для понимания кода

**Примеры:**

| Заголовочный файл | Назначение |
|------------------|-----------|
| `include/core/abstract/types.h` | Типы данных ядра |
| `include/core/abstract/interfaces.h` | Интерфейсы ядра |
| `include/data_processors/abstract/types.h` | Типы обработчиков |
| `include/data_processors/abstract/interfaces.h` | Интерфейсы обработчиков |
| `include/data_processors/abstract/mixins.h` | Миксины обработчиков |

**Важно:** Типы и интерфейсы НЕ имеют файлов реализации (.cpp). Реализация находится в классах, которые наследуют интерфейсы.

## Абстракции и интерфейсы

### Иерархия интерфейсов

**Базовые интерфейсы:**
- `IComponent` — базовый интерфейс для всех компонентов (регистрация, жизненный цикл)
- `INetworkComponent` — интерфейс для компонентов, работающих с сетью

**Специализированные интерфейсы:**
- `IDataSource` — интерфейс для источников данных
- `IDataProcessor` — интерфейс для обработчиков данных
- `IDataSink` — интерфейс для получателей данных

**Интерфейсы управления:**
- `IComponentManager` — управление компонентами (запуск, остановка)
- `IConnectionManager` — управление соединениями между компонентами
- `IEventPublisher` — публикация событий
- `IComponentRegistry` — реестр компонентов

### Миксины (Mixins)

Миксины используются для добавления функциональности к классам без множественного наследования:

**Доступные миксины:**
- `LoggingMixin` — добавляет логирование к компонентам
- `MetricsMixin` — добавляет сбор метрик (производительность, задержки)
- `CachingMixin` — добавляет кэширование результатов обработки
- `ErrorHandlingMixin` — добавляет обработку ошибок и восстановление
- `ConfigurableMixin` — добавляет возможность конфигурации компонента

**Использование миксинов:**
```cpp
// Пример (без кода, только концепция):
class FFTProcessor : public IDataProcessor, 
                     public LoggingMixin,
                     public MetricsMixin {
    // Наследует функциональность логирования и сбора метрик
};
```

# Стандарты оформления CMake-модулей

## Обзор

Этот документ определяет правила оформления `CMakeLists.txt` для библиотек и исполняемых файлов проекта.

---

## Структура модуля-библиотеки (эталон: `src/aggregator/`)

```cmake
cmake_minimum_required(VERSION 3.30)

set(LIB_NAME "my-library")
project(${LIB_NAME})

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Network)

# Создание библиотеки (shared/static управляется BUILD_SHARED_LIBS)
if (BUILD_SHARED_LIBS)
    add_library(${LIB_NAME} SHARED)
else ()
    add_library(${LIB_NAME})
endif ()

# Источники: заголовки PUBLIC, реализации PRIVATE
target_sources(${LIB_NAME} PUBLIC
    include/my_module/abstract/types.h
    include/my_module/abstract/interfaces.h
    include/my_module/my_class.h
)

target_sources(${LIB_NAME} PRIVATE
        src/my_class.cpp
)

# Публичный include — только папка include/
target_include_directories(${LIB_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Линковка
target_link_libraries(${LIB_NAME} PUBLIC
        Qt6::Core
        Qt6::Network
)
```

## Структура модуля с точкой входа (исполняемый файл)

```cmake
# Исполняемый файл точки входа
add_executable(my_app src/main.cpp)
target_link_libraries(my_app PRIVATE ${LIB_NAME})
```

## Правила оформления

1. **`cmake_minimum_required`** — всегда первая строка в корневом и модульных файлах
2. **`set(LIB_NAME ...)`** — имя цели задаётся через переменную, не хардкодится
3. **`CMAKE_CXX_STANDARD 23`** — стандарт C++ задаётся в каждом модуле явно
4. **`CMAKE_AUTOMOC ON`** — обязательно для Qt-модулей с сигналами/слотами
5. **`find_package`** — только те компоненты Qt, которые реально используются в модуле
6. **`target_sources` PUBLIC/PRIVATE** — заголовки PUBLIC (видны потребителям), реализации PRIVATE
7. **`target_include_directories` PUBLIC** — только `include/`, не `include/module_name/`
8. **`target_link_libraries`** — PUBLIC для зависимостей, которые нужны потребителям; PRIVATE для внутренних

## Что НЕ делать

```cmake
# НЕПРАВИЛЬНО — include_directories вместо target_include_directories
include_directories(include)

# НЕПРАВИЛЬНО — link_libraries вместо target_link_libraries
link_libraries(Qt6::Core)
```

## Типичные ошибки

**Линковщик не находит символы** (`undefined reference to ...` / `unresolved external symbol`):
- Причина: зависимость не добавлена в `target_link_libraries`
- Решение: добавить недостающую библиотеку в `target_link_libraries(my_app PRIVATE missing_library)`

**Заголовок не найден** (`fatal error: my_module/my_class.h: No such file or directory`):
- Причина: не добавлена папка `include` в `target_include_directories` или библиотека не подключена

# Qt Visualization Project

## Установка и конфигурация

### CLion setup

1. Install Qt from online installer;
2. Find path installed Qt;
3. Set `Path=C:\Qt\6.*.*\mingw_64\bin` to environment variables in run configuration;
4. Set `-DCMAKE_PREFIX_PATH=C:/Qt/6.*.*/mingw_64` to CMake options in CMake Profiles.

## Документация

Полная документация проекта доступна в steering файлах:

- **Карта проекта:** `#[[file:.kiro/steering/project-map.md]]` — архитектура, компоненты, система сборки
- **Стандарты кодирования:** `#[[file:.kiro/steering/cpp-coding-standards.md]]` — правила написания кода на C++
- **Архитектурные стандарты:** `#[[file:.kiro/steering/arch-coding-standards.md]]` — архитектурные принципы

### Qt документация

Справочная документация по модулям Qt 6.10:

- **Основные модули:**
  - `#[[file:.kiro/steering/qt/qt-core.md]]` — Qt Core (основной модуль)
  - `#[[file:.kiro/steering/qt/qt-gui.md]]` — Qt GUI (графический интерфейс)
  - `#[[file:.kiro/steering/qt/qt-widgets.md]]` — Qt Widgets (классические виджеты)
  - `#[[file:.kiro/steering/qt/qt-network.md]]` — Qt Network (сетевое программирование)
  - `#[[file:.kiro/steering/qt/qt-multimedia.md]]` — Qt Multimedia (аудио/видео)

- **Визуализация данных:**
  - `#[[file:.kiro/steering/qt/qt-graphs.md]]` — Qt Graphs (рекомендуется для новых проектов)
  - `#[[file:.kiro/steering/qt/qt-charts.md]]` — Qt Charts (deprecated)
  - `#[[file:.kiro/steering/qt/qt-datavisualization.md]]` — Qt Data Visualization (deprecated)

- **Дополнительные модули:**
  - `#[[file:.kiro/steering/qt/qt-serialport.md]]` — Qt Serial Port (последовательный порт)
  - `#[[file:.kiro/steering/qt/qt-concurrent.md]]` — Qt Concurrent (многопоточность)
  - `#[[file:.kiro/steering/qt/qt-sql.md]]` — Qt SQL (работа с БД)

- **Полный индекс:** `#[[file:.kiro/steering/qt/README.md]]` — индекс всех модулей Qt
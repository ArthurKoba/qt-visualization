---
inclusion: manual
---

# Qt Concurrent

## Описание

Qt Concurrent предоставляет высокоуровневые API для многопоточного программирования без использования низкоуровневых примитивов потоков. Включает функции для параллельной обработки данных.

**Назначение:** Асинхронное выполнение задач, параллельная обработка данных, многопоточность без явного управления потоками.

## Обзор

- [Qt Concurrent Overview](https://doc.qt.io/qt-6/qtconcurrent-index.html)
- [Qt Concurrent Module](https://doc.qt.io/qt-6/qtconcurrent-module.html)

## Ключевые концепции

### Многопоточность
- [Threading and Concurrent Programming](https://doc.qt.io/qt-6/threads.html)

## Ключевые классы и функции

| Класс/Функция | Описание | Ссылка |
|-------|---------|--------|
| `QtConcurrent::run()` | Запуск функции в отдельном потоке | [QtConcurrent::run](https://doc.qt.io/qt-6/qtconcurrent.html#run) |
| `QtConcurrent::map()` | Применение функции к каждому элементу | [QtConcurrent::map](https://doc.qt.io/qt-6/qtconcurrent.html#map) |
| `QtConcurrent::filter()` | Фильтрация элементов | [QtConcurrent::filter](https://doc.qt.io/qt-6/qtconcurrent.html#filter) |
| `QtConcurrent::reduce()` | Редукция данных | [QtConcurrent::reduce](https://doc.qt.io/qt-6/qtconcurrent.html#reduce) |
| `QFuture` | Результат асинхронной операции | [QFuture](https://doc.qt.io/qt-6/qfuture.html) |
| `QFutureWatcher` | Наблюдатель за QFuture | [QFutureWatcher](https://doc.qt.io/qt-6/qfuturewatcher.html) |
| `QThreadPool` | Пул потоков | [QThreadPool](https://doc.qt.io/qt-6/qthreadpool.html) |
| `QRunnable` | Интерфейс для выполняемых задач | [QRunnable](https://doc.qt.io/qt-6/qrunnable.html) |

## Примеры

- [Qt Concurrent Examples](https://doc.qt.io/qt-6/qtconcurrent-examples.html)

## CMake конфигурация

```cmake
find_package(Qt6 REQUIRED COMPONENTS Concurrent)
target_link_libraries(my_target PRIVATE Qt6::Concurrent)
```

## Связанные модули

- [Qt Core](./qt-core.md) — Основные классы (QThread)
- [Qt Network](./qt-network.md) — Для асинхронных сетевых операций

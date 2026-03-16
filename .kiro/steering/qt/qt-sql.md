---
inclusion: manual
---

# Qt SQL

## Описание

Qt SQL предоставляет классы для работы с базами данных через SQL. Поддерживает различные СУБД: SQLite, PostgreSQL, MySQL, Oracle и другие.

**Назначение:** Интеграция приложений с базами данных, выполнение SQL запросов, работа с данными.

## Обзор

- [Qt SQL Overview](https://doc.qt.io/qt-6/qtsql-index.html)
- [Qt SQL Module](https://doc.qt.io/qt-6/qtsql-module.html)

## Ключевые концепции

### Работа с БД
- [SQL Database Classes](https://doc.qt.io/qt-6/sql-programming.html)

## Ключевые классы

| Класс | Описание | Ссылка |
|-------|---------|--------|
| `QSqlDatabase` | Подключение к БД | [QSqlDatabase](https://doc.qt.io/qt-6/qsqldatabase.html) |
| `QSqlQuery` | Выполнение SQL запросов | [QSqlQuery](https://doc.qt.io/qt-6/qsqlquery.html) |
| `QSqlError` | Информация об ошибке | [QSqlError](https://doc.qt.io/qt-6/qsqlerror.html) |
| `QSqlRecord` | Запись из результата запроса | [QSqlRecord](https://doc.qt.io/qt-6/qsqlrecord.html) |
| `QSqlField` | Поле записи | [QSqlField](https://doc.qt.io/qt-6/qsqlfield.html) |
| `QSqlTableModel` | Модель таблицы БД | [QSqlTableModel](https://doc.qt.io/qt-6/qsqltablemodel.html) |
| `QSqlQueryModel` | Модель результата запроса | [QSqlQueryModel](https://doc.qt.io/qt-6/qsqlquerymodel.html) |
| `QSqlRelationalTableModel` | Модель с отношениями | [QSqlRelationalTableModel](https://doc.qt.io/qt-6/qsqlrelationaltablemodel.html) |
| `QSqlDriver` | Драйвер БД | [QSqlDriver](https://doc.qt.io/qt-6/qsqldriver.html) |

## Примеры

- [Qt SQL Examples](https://doc.qt.io/qt-6/qtsql-examples.html)

## CMake конфигурация

```cmake
find_package(Qt6 REQUIRED COMPONENTS Sql)
target_link_libraries(my_target PRIVATE Qt6::Sql)
```

## Связанные модули

- [Qt Core](./qt-core.md) — Основные классы
- [Qt Widgets](./qt-widgets.md) — Виджеты для отображения данных

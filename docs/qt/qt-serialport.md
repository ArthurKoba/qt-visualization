# Qt Serial Port

## Описание

Qt Serial Port предоставляет классы для работы с аппаратными и виртуальными последовательными портами. Используется для связи с микроконтроллерами, модемами и другими устройствами.

**Назначение:** Обмен данными через COM порты, работа с микроконтроллерами и встраиваемыми системами.

## Обзор

- [Qt Serial Port Overview](https://doc.qt.io/qt-6/qtserialport-index.html)
- [Qt Serial Port Module](https://doc.qt.io/qt-6/qtserialport-module.html)

## Ключевые классы

| Класс | Описание | Ссылка |
|-------|---------|--------|
| `QSerialPort` | Работа с последовательным портом | [QSerialPort](https://doc.qt.io/qt-6/qserialport.html) |
| `QSerialPortInfo` | Информация о портах | [QSerialPortInfo](https://doc.qt.io/qt-6/qserialportinfo.html) |

## Примеры

- [Qt Serial Port Examples](https://doc.qt.io/qt-6/qtserialport-examples.html)

## CMake конфигурация

```cmake
find_package(Qt6 REQUIRED COMPONENTS SerialPort)
target_link_libraries(my_target PRIVATE Qt6::SerialPort)
```

## Связанные модули

- [Qt Core](./qt-core.md) — Основные классы
- [Qt Network](./qt-network.md) — Сетевые классы

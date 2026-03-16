---
inclusion: manual
---

# Qt Core

## Описание

Qt Core — основной модуль Qt, предоставляющий фундаментальные классы без графики. Содержит систему сигналов/слотов, систему свойств, контейнеры, работу с файлами и потоками.

**Назначение:** Базовая функциональность для всех приложений Qt.

## Обзор

- [Qt Core Overview](https://doc.qt.io/qt-6/qtcore-index.html)
- [Qt Core Module](https://doc.qt.io/qt-6/qtcore-module.html)

## Ключевые концепции

### Система сигналов и слотов
- [Signals & Slots](https://doc.qt.io/qt-6/signalsandslots.html)
- [Meta-Object System](https://doc.qt.io/qt-6/metaobjects.html)

### Система свойств
- [The Property System](https://doc.qt.io/qt-6/properties.html)

### Объектная модель
- [Object Model](https://doc.qt.io/qt-6/object.html)
- [Object Trees & Ownership](https://doc.qt.io/qt-6/objecttrees.html)

## Многопоточность

- [Threading and Concurrent Programming](https://doc.qt.io/qt-6/threads.html)
- [QThread Documentation](https://doc.qt.io/qt-6/qthread.html)
- [Qt Concurrent Module](https://doc.qt.io/qt-6/qtconcurrent-index.html)

## Контейнеры

- [Container Classes](https://doc.qt.io/qt-6/containers.html)
- [Implicit Sharing](https://doc.qt.io/qt-6/implicit-sharing.html)

## Примеры

- [Qt Core Examples](https://doc.qt.io/qt-6/qtcore-examples.html)

## CMake конфигурация

```cmake
find_package(Qt6 REQUIRED COMPONENTS Core)
target_link_libraries(my_target PRIVATE Qt6::Core)
```

## Ключевые классы

**Актуальный список всех классов модуля:** [Qt Core C++ Classes](https://doc.qt.io/qt-6/qtcore-module.html)

### Основные классы

| Класс | Описание | Ссылка |
|-------|---------|--------|
| `QObject` | Базовый класс всех объектов Qt | [QObject](https://doc.qt.io/qt-6/qobject.html) |
| `QCoreApplication` | Основной класс приложения без GUI | [QCoreApplication](https://doc.qt.io/qt-6/qcoreapplication.html) |
| `QThread` | Управление потоками | [QThread](https://doc.qt.io/qt-6/qthread.html) |
| `QMutex` | Синхронизация доступа к ресурсам | [QMutex](https://doc.qt.io/qt-6/qmutex.html) |
| `QReadWriteLock` | Блокировка для чтения/записи | [QReadWriteLock](https://doc.qt.io/qt-6/qreadwritelock.html) |
| `QSemaphore` | Семафор для синхронизации | [QSemaphore](https://doc.qt.io/qt-6/qsemaphore.html) |
| `QWaitCondition` | Условная переменная | [QWaitCondition](https://doc.qt.io/qt-6/qwaitcondition.html) |

### Контейнеры

| Класс | Описание | Ссылка |
|-------|---------|--------|
| `QList` | Динамический массив | [QList](https://doc.qt.io/qt-6/qlist.html) |
| `QVector` | Вектор (синоним QList) | [QVector](https://doc.qt.io/qt-6/qvector.html) |
| `QHash` | Хеш-таблица | [QHash](https://doc.qt.io/qt-6/qhash.html) |
| `QMap` | Отсортированная ассоциативная таблица | [QMap](https://doc.qt.io/qt-6/qmap.html) |
| `QSet` | Множество уникальных значений | [QSet](https://doc.qt.io/qt-6/qset.html) |
| `QQueue` | Очередь FIFO | [QQueue](https://doc.qt.io/qt-6/qqueue.html) |
| `QStack` | Стек LIFO | [QStack](https://doc.qt.io/qt-6/qstack.html) |
| `QByteArray` | Массив байтов | [QByteArray](https://doc.qt.io/qt-6/qbytearray.html) |
| `QString` | Строка Unicode | [QString](https://doc.qt.io/qt-6/qstring.html) |

### Работа с файлами и директориями

| Класс | Описание | Ссылка |
|-------|---------|--------|
| `QFile` | Работа с файлами | [QFile](https://doc.qt.io/qt-6/qfile.html) |
| `QDir` | Работа с директориями | [QDir](https://doc.qt.io/qt-6/qdir.html) |
| `QFileInfo` | Информация о файле | [QFileInfo](https://doc.qt.io/qt-6/qfileinfo.html) |
| `QIODevice` | Базовый класс для I/O устройств | [QIODevice](https://doc.qt.io/qt-6/qiodevice.html) |
| `QTextStream` | Текстовый поток | [QTextStream](https://doc.qt.io/qt-6/qtextstream.html) |
| `QDataStream` | Бинарный поток | [QDataStream](https://doc.qt.io/qt-6/qdatastream.html) |

### Дата и время

| Класс | Описание | Ссылка |
|-------|---------|--------|
| `QDate` | Дата | [QDate](https://doc.qt.io/qt-6/qdate.html) |
| `QTime` | Время | [QTime](https://doc.qt.io/qt-6/qtime.html) |
| `QDateTime` | Дата и время | [QDateTime](https://doc.qt.io/qt-6/qdatetime.html) |
| `QTimer` | Таймер | [QTimer](https://doc.qt.io/qt-6/qtimer.html) |
| `QElapsedTimer` | Измерение прошедшего времени | [QElapsedTimer](https://doc.qt.io/qt-6/qelapsedtimer.html) |

### JSON и сериализация

| Класс | Описание | Ссылка |
|-------|---------|--------|
| `QJsonDocument` | JSON документ | [QJsonDocument](https://doc.qt.io/qt-6/qjsondocument.html) |
| `QJsonObject` | JSON объект | [QJsonObject](https://doc.qt.io/qt-6/qjsonobject.html) |
| `QJsonArray` | JSON массив | [QJsonArray](https://doc.qt.io/qt-6/qjsonarray.html) |
| `QJsonValue` | JSON значение | [QJsonValue](https://doc.qt.io/qt-6/qjsonvalue.html) |

### Асинхронные операции

| Класс | Описание | Ссылка |
|-------|---------|--------|
| `QFuture` | Результат асинхронной операции | [QFuture](https://doc.qt.io/qt-6/qfuture.html) |
| `QPromise` | Обещание для асинхронных операций | [QPromise](https://doc.qt.io/qt-6/qpromise.html) |
| `QFutureWatcher` | Наблюдение за QFuture | [QFutureWatcher](https://doc.qt.io/qt-6/qfuturewatcher.html) |

### Модель/Представление

| Класс | Описание | Ссылка |
|-------|---------|--------|
| `QAbstractItemModel` | Базовый класс для моделей данных | [QAbstractItemModel](https://doc.qt.io/qt-6/qabstractitemmodel.html) |
| `QAbstractListModel` | Базовый класс для моделей списков | [QAbstractListModel](https://doc.qt.io/qt-6/qabstractlistmodel.html) |
| `QAbstractTableModel` | Базовый класс для моделей таблиц | [QAbstractTableModel](https://doc.qt.io/qt-6/qabstracttablemodel.html) |
| `QModelIndex` | Индекс элемента в модели | [QModelIndex](https://doc.qt.io/qt-6/qmodelindex.html) |
| `QItemSelectionModel` | Управление выделением элементов | [QItemSelectionModel](https://doc.qt.io/qt-6/qitemselectionmodel.html) |

### Утилиты

| Класс | Описание | Ссылка |
|-------|---------|--------|
| `QDebug` | Вывод отладочной информации | [QDebug](https://doc.qt.io/qt-6/qdebug.html) |
| `QLoggingCategory` | Категория логирования | [QLoggingCategory](https://doc.qt.io/qt-6/qloggingcategory.html) |
| `QUrl` | URL адрес | [QUrl](https://doc.qt.io/qt-6/qurl.html) |
| `QUuid` | UUID идентификатор | [QUuid](https://doc.qt.io/qt-6/quuid.html) |
| `QVariant` | Универсальный тип данных | [QVariant](https://doc.qt.io/qt-6/qvariant.html) |
| `QSettings` | Сохранение параметров приложения | [QSettings](https://doc.qt.io/qt-6/qsettings.html) |

## Примеры использования

### Создание простого приложения

```cpp
#include <QCoreApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    
    qDebug() << "Hello, Qt!";
    
    return app.exec();
}
```

### Работа с потоками

```cpp
#include <QThread>
#include <QObject>

class Worker : public QObject {
    Q_OBJECT
public slots:
    void doWork() {
        qDebug() << "Working in thread:" << QThread::currentThread();
    }
};

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    
    Worker worker;
    QThread thread;
    
    worker.moveToThread(&thread);
    connect(&thread, &QThread::started, &worker, &Worker::doWork);
    
    thread.start();
    
    return app.exec();
}
```

### Работа с JSON

```cpp
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

int main()
{
    QJsonObject obj;
    obj["name"] = "John";
    obj["age"] = 30;
    
    QJsonArray array;
    array.append("item1");
    array.append("item2");
    obj["items"] = array;
    
    QJsonDocument doc(obj);
    QByteArray json = doc.toJson();
    
    qDebug() << json;
    
    return 0;
}
```

## Дополнительные ресурсы

- [Qt Core Reference Documentation](https://doc.qt.io/qt-6/qtcore-index.html)
- [Qt Core Examples](https://doc.qt.io/qt-6/qtcore-examples.html)
- [Qt Core Classes](https://doc.qt.io/qt-6/qtcore-module.html)

# Qt Widgets

## Описание

Qt Widgets предоставляет набор UI элементов для создания классических десктопных интерфейсов. Все виджеты наследуются от QWidget и используют систему сигналов/слотов для взаимодействия.

**Назначение:** Создание традиционных десктопных приложений с кнопками, полями ввода, таблицами и т.д.

## Обзор

- [Qt Widgets Overview](https://doc.qt.io/qt-6/qtwidgets-index.html)
- [Qt Widgets Module](https://doc.qt.io/qt-6/qtwidgets-module.html)

## Ключевые концепции

### Основы виджетов
- [Widgets Tutorial](https://doc.qt.io/qt-6/widgets-tutorial.html)
- [Getting Started Programming with Qt Widgets](https://doc.qt.io/qt-6/gettingstartedqt.html)

### Макеты
- [Layout Management](https://doc.qt.io/qt-6/layout.html)
- [Layout Classes](https://doc.qt.io/qt-6/layout-classes.html)

### Модель/Представление
- [Model/View Architecture](https://doc.qt.io/qt-6/model-view-programming.html)
- [Model/View Tutorial](https://doc.qt.io/qt-6/modelview.html)

### Стили
- [Styles](https://doc.qt.io/qt-6/style-reference.html)
- [Qt Style Sheets](https://doc.qt.io/qt-6/stylesheet.html)

## Ключевые классы

| Класс | Описание | Ссылка |
|-------|---------|--------|
| `QApplication` | Основной класс приложения | [QApplication](https://doc.qt.io/qt-6/qapplication.html) |
| `QWidget` | Базовый класс всех виджетов | [QWidget](https://doc.qt.io/qt-6/qwidget.html) |
| `QMainWindow` | Главное окно приложения | [QMainWindow](https://doc.qt.io/qt-6/qmainwindow.html) |
| `QDialog` | Диалоговое окно | [QDialog](https://doc.qt.io/qt-6/qdialog.html) |
| `QPushButton` | Кнопка | [QPushButton](https://doc.qt.io/qt-6/qpushbutton.html) |
| `QLineEdit` | Однострочное поле ввода | [QLineEdit](https://doc.qt.io/qt-6/qlineedit.html) |
| `QTextEdit` | Многострочный редактор текста | [QTextEdit](https://doc.qt.io/qt-6/qtextedit.html) |
| `QLabel` | Метка текста | [QLabel](https://doc.qt.io/qt-6/qlabel.html) |
| `QComboBox` | Выпадающий список | [QComboBox](https://doc.qt.io/qt-6/qcombobox.html) |
| `QListWidget` | Список элементов | [QListWidget](https://doc.qt.io/qt-6/qlistwidget.html) |
| `QTableWidget` | Таблица | [QTableWidget](https://doc.qt.io/qt-6/qtablewidget.html) |
| `QTreeWidget` | Дерево элементов | [QTreeWidget](https://doc.qt.io/qt-6/qtreewidget.html) |
| `QSlider` | Ползунок | [QSlider](https://doc.qt.io/qt-6/qslider.html) |
| `QSpinBox` | Поле для ввода чисел | [QSpinBox](https://doc.qt.io/qt-6/qspinbox.html) |
| `QCheckBox` | Флажок | [QCheckBox](https://doc.qt.io/qt-6/qcheckbox.html) |
| `QRadioButton` | Радиокнопка | [QRadioButton](https://doc.qt.io/qt-6/qradiobutton.html) |
| `QVBoxLayout` | Вертикальный макет | [QVBoxLayout](https://doc.qt.io/qt-6/qvboxlayout.html) |
| `QHBoxLayout` | Горизонтальный макет | [QHBoxLayout](https://doc.qt.io/qt-6/qhboxlayout.html) |
| `QGridLayout` | Сеточный макет | [QGridLayout](https://doc.qt.io/qt-6/qgridlayout.html) |
| `QAbstractItemModel` | Базовый класс для моделей | [QAbstractItemModel](https://doc.qt.io/qt-6/qabstractitemmodel.html) |
| `QStandardItemModel` | Стандартная модель | [QStandardItemModel](https://doc.qt.io/qt-6/qstandarditemmodel.html) |
| `QFileDialog` | Диалог выбора файла | [QFileDialog](https://doc.qt.io/qt-6/qfiledialog.html) |
| `QColorDialog` | Диалог выбора цвета | [QColorDialog](https://doc.qt.io/qt-6/qcolordialog.html) |
| `QMessageBox` | Диалог сообщения | [QMessageBox](https://doc.qt.io/qt-6/qmessagebox.html) |

## Примеры

- [Qt Widgets Examples](https://doc.qt.io/qt-6/qtwidgets-examples.html)
- [Layout Examples](https://doc.qt.io/qt-6/examples-layouts.html)

## CMake конфигурация

```cmake
find_package(Qt6 REQUIRED COMPONENTS Widgets)
target_link_libraries(my_target PRIVATE Qt6::Widgets)
```

## Связанные модули

- [Qt Core](./qt-core.md) — Основные классы
- [Qt GUI](./qt-gui.md) — Графические классы
- [Qt Charts](./qt-charts.md) — Графики и диаграммы

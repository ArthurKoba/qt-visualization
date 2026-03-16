---
inclusion: manual
---

# Qt Data Visualization

## Описание

Qt Data Visualization предоставляет функциональность для визуализации данных в 3D: столбчатые, точечные и поверхностные графики. Оптимизирована для работы с большими объемами данных и аппаратного ускорения.

**⚠️ Внимание:** Модуль deprecated с Qt 6.10. Для новых проектов используйте [Qt Graphs](https://doc.qt.io/qt-6/qtgraphs-index.html).

**Назначение:** 3D визуализация данных, работа с картами глубины, потоковые данные с датчиков.

## Обзор

- [Qt Data Visualization Overview](https://doc.qt.io/qt-6/qtdatavisualization-index.html)
- [Qt Data Visualization Module](https://doc.qt.io/qt-6/qtdatavisualization-module.html)

## Ключевые концепции

### Основы визуализации
- [Overview](https://doc.qt.io/qt-6/qtdatavisualization-overview.html)
- [Data Handling](https://doc.qt.io/qt-6/qtdatavisualization-data-handling.html)
- [Interacting with Data](https://doc.qt.io/qt-6/qtdatavisualization-interacting-with-data.html)

## Ключевые классы (C++)

| Класс | Описание | Ссылка |
|-------|---------|--------|
| `Q3DScatter` | 3D точечный график | [Q3DScatter](https://doc.qt.io/qt-6/q3dscatter.html) |
| `Q3DBars` | 3D столбчатая диаграмма | [Q3DBars](https://doc.qt.io/qt-6/q3dbars.html) |
| `Q3DSurface` | 3D поверхностный график | [Q3DSurface](https://doc.qt.io/qt-6/q3dsurface.html) |
| `QAbstractDataProxy` | Базовый класс для прокси данных | [QAbstractDataProxy](https://doc.qt.io/qt-6/qabstractdataproxy.html) |
| `QBarDataProxy` | Прокси для столбчатых данных | [QBarDataProxy](https://doc.qt.io/qt-6/qbardataproxy.html) |
| `QScatterDataProxy` | Прокси для точечных данных | [QScatterDataProxy](https://doc.qt.io/qt-6/qscatterdataproxy.html) |
| `QSurfaceDataProxy` | Прокси для поверхностных данных | [QSurfaceDataProxy](https://doc.qt.io/qt-6/qsurfacedataproxy.html) |
| `QHeightMapSurfaceDataProxy` | Прокси для карт высот | [QHeightMapSurfaceDataProxy](https://doc.qt.io/qt-6/qheightmapsurfacedataproxy.html) |
| `Q3DCamera` | Камера 3D сцены | [Q3DCamera](https://doc.qt.io/qt-6/q3dcamera.html) |
| `Q3DLight` | Источник света | [Q3DLight](https://doc.qt.io/qt-6/q3dlight.html) |
| `Q3DTheme` | Тема оформления | [Q3DTheme](https://doc.qt.io/qt-6/q3dtheme.html) |
| `QCustom3DItem` | Пользовательский 3D элемент | [QCustom3DItem](https://doc.qt.io/qt-6/qcustom3ditem.html) |
| `QCustom3DLabel` | 3D метка текста | [QCustom3DLabel](https://doc.qt.io/qt-6/qcustom3dlabel.html) |
| `Q3DInputHandler` | Обработчик ввода | [Q3DInputHandler](https://doc.qt.io/qt-6/q3dinputhandler.html) |

## Примеры

- [Qt Data Visualization Examples](https://doc.qt.io/qt-6/qtdatavisualization-examples.html)

## CMake конфигурация

```cmake
find_package(Qt6 REQUIRED COMPONENTS DataVisualization)
target_link_libraries(my_target PRIVATE Qt6::DataVisualization)
```

## Миграция на Qt Graphs

- [Qt Graphs Migration from Qt DataVisualization](https://doc.qt.io/qt-6/qtgraphs-migration-from-qtdatavisualization.html)

## Связанные модули

- [Qt GUI](./qt-gui.md) — Графические классы
- [Qt Widgets](./qt-widgets.md) — Виджеты
- [Qt Graphs](https://doc.qt.io/qt-6/qtgraphs-index.html) — Рекомендуемая замена

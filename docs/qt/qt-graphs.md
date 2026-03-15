# Qt Graphs

## Описание

Qt Graphs — современный модуль для создания интерактивных 2D и 3D графиков и диаграмм. Является рекомендуемой заменой для deprecated модулей Qt Charts и Qt Data Visualization.

**Назначение:** Визуализация данных в виде 2D/3D графиков, диаграмм, гистограмм и поверхностных графиков с поддержкой интерактивности.

## Обзор

- [Qt Graphs Overview](https://doc.qt.io/qt-6/qtgraphs-index.html)
- [Qt Graphs Module](https://doc.qt.io/qt-6/qtgraphs-module.html)
- [Qt Graphs Getting Started](https://doc.qt.io/qt-6/qtgraphs-getting-started.html)

## Ключевые концепции

### 2D Графики
- [2D Graphs](https://doc.qt.io/qt-6/qtgraphs-2d-graphs.html)
- Линейные графики, столбчатые диаграммы, круговые диаграммы, точечные графики

### 3D Графики
- [3D Graphs](https://doc.qt.io/qt-6/qtgraphs-3d-graphs.html)
- Столбчатые диаграммы, точечные графики, поверхностные графики

### Интерактивность
- [Interaction](https://doc.qt.io/qt-6/qtgraphs-interaction.html)
- Масштабирование, панорамирование, выделение данных

### Темы и стили
- [Themes](https://doc.qt.io/qt-6/qtgraphs-themes.html)
- Встроенные и пользовательские темы оформления

## Ключевые классы

**Актуальный список всех классов модуля:** [Qt Graphs C++ Classes](https://doc.qt.io/qt-6/qtgraphs-module.html)

### 2D Графики

| Класс | Описание | Ссылка |
|-------|---------|--------|
| `QGraphsView` | Основной виджет для отображения 2D графиков | [QGraphsView](https://doc.qt.io/qt-6/qgraphsview.html) |
| `QGraph2D` | Контейнер для 2D графиков | [QGraph2D](https://doc.qt.io/qt-6/qgraph2d.html) |
| `QXYSeries` | Базовый класс для XY серий данных | [QXYSeries](https://doc.qt.io/qt-6/qxyseries.html) |
| `QLineSeries` | Линейный график | [QLineSeries](https://doc.qt.io/qt-6/qxyseries.html) |
| `QScatterSeries` | Точечный график | [QScatterSeries](https://doc.qt.io/qt-6/qscatterseries.html) |
| `QAreaSeries` | График с заполненной областью | [QAreaSeries](https://doc.qt.io/qt-6/qareaseries.html) |
| `QBarSeries` | Столбчатая диаграмма | [QBarSeries](https://doc.qt.io/qt-6/qbarseries.html) |
| `QBarSet` | Набор данных для столбчатой диаграммы | [QBarSet](https://doc.qt.io/qt-6/qbarset.html) |
| `QPieSeries` | Круговая диаграмма | [QPieSeries](https://doc.qt.io/qt-6/qpieseries.html) |
| `QPieSlice` | Сегмент круговой диаграммы | [QPieSlice](https://doc.qt.io/qt-6/qpieslice.html) |

### 3D Графики

| Класс | Описание | Ссылка |
|-------|---------|--------|
| `Q3DScatter` | 3D точечный график | [Q3DScatter](https://doc.qt.io/qt-6/q3dscatter.html) |
| `Q3DBars` | 3D столбчатая диаграмма | [Q3DBars](https://doc.qt.io/qt-6/q3dbars.html) |
| `Q3DSurface` | 3D поверхностный график | [Q3DSurface](https://doc.qt.io/qt-6/q3dsurface.html) |
| `QScatterDataProxy` | Прокси для данных 3D точечного графика | [QScatterDataProxy](https://doc.qt.io/qt-6/qscatterdataproxy.html) |
| `QBarDataProxy` | Прокси для данных 3D столбчатой диаграммы | [QBarDataProxy](https://doc.qt.io/qt-6/qbardataproxy.html) |
| `QSurfaceDataProxy` | Прокси для данных 3D поверхностного графика | [QSurfaceDataProxy](https://doc.qt.io/qt-6/qsurfacedataproxy.html) |

### Оси и легенды

| Класс | Описание | Ссылка |
|-------|---------|--------|
| `QAbstractAxis` | Базовый класс для осей | [QAbstractAxis](https://doc.qt.io/qt-6/qabstractaxis.html) |
| `QValueAxis` | Числовая ось | [QValueAxis](https://doc.qt.io/qt-6/qvalueaxis.html) |
| `QCategoryAxis` | Категориальная ось | [QCategoryAxis](https://doc.qt.io/qt-6/qcategoryaxis.html) |
| `QDateTimeAxis` | Ось времени | [QDateTimeAxis](https://doc.qt.io/qt-6/qdatetimeaxis.html) |
| `QLogValueAxis` | Логарифмическая ось | [QLogValueAxis](https://doc.qt.io/qt-6/qlogvalueaxis.html) |
| `QLegend` | Легенда графика | [QLegend](https://doc.qt.io/qt-6/qlegend.html) |

## Примеры

- [Qt Graphs Examples](https://doc.qt.io/qt-6/qtgraphs-examples.html)
- [2D Graph Examples](https://doc.qt.io/qt-6/qtgraphs-2d-examples.html)
- [3D Graph Examples](https://doc.qt.io/qt-6/qtgraphs-3d-examples.html)

## CMake конфигурация

```cmake
find_package(Qt6 REQUIRED COMPONENTS Graphs)
target_link_libraries(my_target PRIVATE Qt6::Graphs)
```

## Миграция с deprecated модулей

### С Qt Charts

- [Migration from Qt Charts](https://doc.qt.io/qt-6/qtgraphs-migration-from-qtcharts.html)
- Основные отличия в API и структуре

### С Qt Data Visualization

- [Migration from Qt Data Visualization](https://doc.qt.io/qt-6/qtgraphs-migration-from-qtdatavisualization.html)
- Обновление 3D графиков

## Связанные модули

- [Qt Widgets](./qt-widgets.md) — Виджеты
- [Qt GUI](./qt-gui.md) — Графические классы
- [Qt Charts](./qt-charts.md) — Deprecated модуль (используйте Qt Graphs)
- [Qt Data Visualization](./qt-datavisualization.md) — Deprecated модуль (используйте Qt Graphs)


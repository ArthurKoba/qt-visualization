---
inclusion: manual
---

# Qt Charts

## Описание

Qt Charts предоставляет набор компонентов для создания графиков и диаграмм. Может использоваться как QWidget, QGraphicsWidget или QML типы.

**⚠️ Внимание:** Модуль deprecated с Qt 6.10. Для новых проектов используйте [Qt Graphs](https://doc.qt.io/qt-6/qtgraphs-index.html).

**Назначение:** Визуализация данных в виде графиков, диаграмм и гистограмм.

## Обзор

- [Qt Charts Overview](https://doc.qt.io/qt-6/qtcharts-index.html)
- [Qt Charts Module](https://doc.qt.io/qt-6/qtcharts-module.html)

## Ключевые концепции

### Основы графиков
- [Qt Charts Overview](https://doc.qt.io/qt-6/qtcharts-overview.html)

## Ключевые классы

| Класс | Описание | Ссылка |
|-------|---------|--------|
| `QChart` | Основной класс графика | [QChart](https://doc.qt.io/qt-6/qchart.html) |
| `QChartView` | Виджет для отображения графика | [QChartView](https://doc.qt.io/qt-6/qchartview.html) |
| `QAbstractSeries` | Базовый класс для серий данных | [QAbstractSeries](https://doc.qt.io/qt-6/qabstractseries.html) |
| `QLineSeries` | Линейный график | [QLineSeries](https://doc.qt.io/qt-6/qlineseries.html) |
| `QSplineSeries` | График со сплайнами | [QSplineSeries](https://doc.qt.io/qt-6/qsplineseries.html) |
| `QScatterSeries` | Точечный график | [QScatterSeries](https://doc.qt.io/qt-6/qscatterseries.html) |
| `QAreaSeries` | График с заполненной областью | [QAreaSeries](https://doc.qt.io/qt-6/qareaseries.html) |
| `QBarSeries` | Столбчатая диаграмма | [QBarSeries](https://doc.qt.io/qt-6/qbarseries.html) |
| `QBarSet` | Набор данных для столбчатой диаграммы | [QBarSet](https://doc.qt.io/qt-6/qbarset.html) |
| `QPieSeries` | Круговая диаграмма | [QPieSeries](https://doc.qt.io/qt-6/qpieseries.html) |
| `QPieSlice` | Сегмент круговой диаграммы | [QPieSlice](https://doc.qt.io/qt-6/qpieslice.html) |
| `QAbstractAxis` | Базовый класс для осей | [QAbstractAxis](https://doc.qt.io/qt-6/qabstractaxis.html) |
| `QValueAxis` | Числовая ось | [QValueAxis](https://doc.qt.io/qt-6/qvalueaxis.html) |
| `QCategoryAxis` | Категориальная ось | [QCategoryAxis](https://doc.qt.io/qt-6/qcategoryaxis.html) |
| `QDateTimeAxis` | Ось времени | [QDateTimeAxis](https://doc.qt.io/qt-6/qdatetimeaxis.html) |
| `QLogValueAxis` | Логарифмическая ось | [QLogValueAxis](https://doc.qt.io/qt-6/qlogvalueaxis.html) |
| `QLegend` | Легенда графика | [QLegend](https://doc.qt.io/qt-6/qlegend.html) |
| `QChartTheme` | Тема оформления | [QChart::ChartTheme](https://doc.qt.io/qt-6/qchart.html#ChartTheme-enum) |

## Примеры

- [Qt Charts Examples](https://doc.qt.io/qt-6/qtcharts-examples.html)

## CMake конфигурация

```cmake
find_package(Qt6 REQUIRED COMPONENTS Charts)
target_link_libraries(my_target PRIVATE Qt6::Charts)
```

## Миграция на Qt Graphs

- [Qt Graphs Migration from Qt Charts](https://doc.qt.io/qt-6/qtgraphs-migration-from-qtcharts.html)

## Связанные модули

- [Qt Widgets](./qt-widgets.md) — Виджеты
- [Qt GUI](./qt-gui.md) — Графические классы
- [Qt Graphs](https://doc.qt.io/qt-6/qtgraphs-index.html) — Рекомендуемая замена

#include "line.h"

#include <QLineSeries>

LineChartView::LineChartView() {
    chart()->setTitle("Line chart");
    _series = new QLineSeries;
    _init_series();
}

void LineChartView::_update_chart_data() {
    if (not _need_update) return;

    // Используйте replace вместо clear + append, это оптимизировано для производительности
    QList<QPointF> points;
    points.reserve(_data.size());
    for (int i = 0; i < _data.size(); i++) {
        points.append(QPointF(i * 11, _data[i]));
    }
    reinterpret_cast<QLineSeries *>(_series)->replace(points);
    _on_update();
    _need_update = false;
}



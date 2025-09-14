#include "line.h"

#include <QLineSeries>

LineChartView::LineChartView() {
    chart()->setTitle("Line chart");
    _series = new QLineSeries;
    _init_series();
}

void LineChartView::_update_chart_data() {
    QList<QPointF> points;
    points.reserve(qsizetype(_data.size()));
    for (int i = 0; i < _data.size(); i++) {
        points.append(QPointF(i, _data[i]));
    }
    reinterpret_cast<QLineSeries *>(_series)->replace(points);
}



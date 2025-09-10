#include "bar.h"

#include <QBarSeries>

BarChartView::BarChartView() {
    chart()->setTitle("Bar chart");
    _series = new QBarSeries;
    _set = new QBarSet("");
    pen = _set->pen();

    reinterpret_cast<QBarSeries *>(_series)->append(_set);
    _init_series();
}

void BarChartView::_update_chart_data() {
    if (not _need_update) return;

    if (_set) {
        reinterpret_cast<QBarSeries *>(_series)->remove(_set);
    }
    _set = new QBarSet("");
    _set->setPen(pen);
    _set->remove(0, _set->count());
    for (float i: _data) {
        _set->append(i);
    }
    reinterpret_cast<QBarSeries *>(_series)->append(_set);

    _on_update();
    _need_update = true;
}

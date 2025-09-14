#include "freq.h"

FreqChartView::FreqChartView(qreal freq_step) {
    chart()->setTitle("Freq Chart");
//    _pen = QPen(Qt::blue, 1, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin);

    chart()->addAxis(&_freq_axis, Qt::AlignBottom);
    _freq_axis.setLabelFormat("%.0f");
    _freq_axis.hide();
    update_freq_step(freq_step);
}


void FreqChartView::_on_updated_data_size(size_t new_size) {
    AbstractChartView::_on_updated_data_size(new_size);
//    size_t width = new_size > 1000 ? 1 : new_size > 500 : 2, ;
//    pen.setWidth(0);
    update_freq_step(_freq_step);
}

void FreqChartView::update_freq_step(qreal freq_step) {
    if (freq_step and freq_step > 1) {
        _freq_step = freq_step;
        _freq_axis.setRange(0, _freq_step * qreal(_data.size()));
        _freq_axis.show();
    }
}

#include "freq.h"

FreqChartView::FreqChartView() {
    chart()->setTitle("Freq Chart");
    pen = QPen(Qt::blue, 3, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin);
    pen.setWidth(3);
//    categoryAxis.append("LOW", 20);
//    categoryAxis.append("MIDDLE", 250);
//    categoryAxis.append("HIGH", 300);
//    chart()->addAxis(&categoryAxis, Qt::AlignBottom);
//    chart()->addAxis(&freqAxis, Qt::AlignBottom);
//    freqAxis.set_range(0, qreal(_data.size()));
//    auto labels = freqAxis.categoriesLabels();
//    for (const auto &label: labels) {
//        freqAxis.remove(label);
//    }
//    for (int i = 0; i < _data.size() / 10; ++i) {
//        freqAxis.append(QString::number(qreal(float(i) * 11.72f)), i * 10);
//    }
}

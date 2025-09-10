#ifndef QT_VISUALIZATION_FREQ_H
#define QT_VISUALIZATION_FREQ_H

#include "bar.h"

class FreqChartView final: public BarChartView {
protected:
    QCategoryAxis category_axis;
//    QCategoryAxis freq_axis;
public:
    FreqChartView();
};

#endif //QT_VISUALIZATION_FREQ_H

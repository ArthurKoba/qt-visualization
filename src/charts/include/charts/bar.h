#ifndef QT_VISUALIZATION_BAR_H
#define QT_VISUALIZATION_BAR_H

#include "fps.h"

#include <QBarSet>

class BarChartView : public FPSChartView {
protected:
    QBarSet *_set;
    QPen pen;
public:
    BarChartView();
    void _update_chart_data() override;
};

#endif //QT_VISUALIZATION_BAR_H

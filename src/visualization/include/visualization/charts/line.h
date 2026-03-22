#ifndef QT_VISUALIZATION_LINE_H
#define QT_VISUALIZATION_LINE_H

#include "visualization/abstract/fps.h"

class LineChartView : public FPSChartView {
public:
    LineChartView();

    void _update_chart_data() override;
};


#endif //QT_VISUALIZATION_LINE_H

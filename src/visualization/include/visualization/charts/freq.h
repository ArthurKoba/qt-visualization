#ifndef QT_VISUALIZATION_FREQ_H
#define QT_VISUALIZATION_FREQ_H

#include "visualization/charts/bar.h"

class FreqChartView final: public BarChartView {
protected:
    qreal _freq_step;
    QValueAxis  _freq_axis;
    void _on_updated_data_size(size_t new_size) override;

public:
    explicit FreqChartView(qreal freq_step = 1);

    void update_freq_step(qreal freq_step);
};

#endif //QT_VISUALIZATION_FREQ_H

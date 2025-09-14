#ifndef QT_VISUALIZATION_FPS_H
#define QT_VISUALIZATION_FPS_H

#include <QLabel>
#include <QCategoryAxis>

#include "mixins.h"
#include "abs.h"

class FPSChartView : public AbstractChartView, public FPSMixin {
protected:
    QLabel *_fps_label;
    void _setFPS(qreal fps);
    void _on_chart_updated() override;
public:
    explicit FPSChartView();
};

#endif //QT_VISUALIZATION_FPS_H

#ifndef QT_VISUALIZATION_FPS_H
#define QT_VISUALIZATION_FPS_H

#include <QLabel>

#include "visualization/abstract/abs.h"
#include "visualization/abstract/mixins.h"

class FPSChartView : public AbstractChartView, public FPSMixin {
public:
    explicit FPSChartView();

protected:
    inline static auto _label = QStringLiteral("FPS: %1");
    QLabel *_fps_label;

    void _setFPS(qreal fps) const;

    void _on_chart_updated() override;
};

#endif //QT_VISUALIZATION_FPS_H

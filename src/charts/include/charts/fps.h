#ifndef QT_VISUALIZATION_FPS_H
#define QT_VISUALIZATION_FPS_H

#include <QChartView>
#include <QtTypes>
#include <QTimer>
#include <QLabel>
#include <QXYSeries>
#include <QValueAxis>
#include <QLineSeries>

#include "mixins.h"


class FPSChartView final : public QChartView, public FPSMixin {


    QTimer _dataUpdateTimer;
    QLabel *fpsLabel;
    std::vector<float> _data;
    bool need_update = false;

    void _setFPS(qreal fps);

public:
    QXYSeries *series;
    QValueAxis axisX;
    QValueAxis axisY;

    explicit FPSChartView();

    void updateChartData();

    void update(std::vector<float> &data);


};

#endif //QT_VISUALIZATION_FPS_H

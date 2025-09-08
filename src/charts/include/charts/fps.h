#ifndef QT_VISUALIZATION_FPS_H
#define QT_VISUALIZATION_FPS_H

#include <QChartView>
#include <QtTypes>
#include <QTimer>
#include <QLabel>
#include <QXYSeries>
#include <QValueAxis>
#include <QCategoryAxis>
#include <QLineSeries>
#include <QBarSet>

#include "mixins.h"


class FPSChartView final : public QChartView, public FPSMixin {
public:
    enum ChartType {LINE, BAR};
private:
    ChartType _type;

    QTimer _dataUpdateTimer;
    QLabel *fpsLabel;
    std::vector<float> _data;
    bool need_update = false;
    bool auto_resizing = false;
    float lastMin;
    float lastMax;

    void _setFPS(qreal fps);

public:

    QAbstractSeries *series;
    QBarSet *set;
    QValueAxis axisX;
    QCategoryAxis categoryAxis;
    QCategoryAxis freqAxis;
    QValueAxis axisY;

    explicit FPSChartView(ChartType type = LINE);

    void setAutoResizing(bool value);

    void updateChartData();

    void update(std::vector<float> &data);
};

#endif //QT_VISUALIZATION_FPS_H

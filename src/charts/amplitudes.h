#ifndef QT_VISUALIZATION_AMPLITUDES_H
#define QT_VISUALIZATION_AMPLITUDES_H


#include <QChart>
#include <QChartView>
#include <QBarSeries>
#include <QBarSet>
#include <QValueAxis>
#include "abs_view.h"


class AmplitudesChart : public QChart {
public:
    QBarSet *sets = nullptr;
    QBarSeries *series = nullptr;

    AmplitudesChart();
    void changeDataSize(size_t newSize);
private:
    QValueAxis *axisX{};
    QValueAxis *axisY{};
};


class AmplitudesChartView : public ABSView {
public:
    AmplitudesChartView();
    void execPacket(Packet &packet) override;
    AmplitudesChart *chart() const;
private:
    size_t dataSize = 0;
    int *data = nullptr;
//    AmplitudesChart *chart;
    ~AmplitudesChartView() override;
    void refreshGuiData() const override;
};

#endif //QT_VISUALIZATION_AMPLITUDES_H

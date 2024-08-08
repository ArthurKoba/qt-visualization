
#ifndef QT_VISUALIZATION_SAMPLES_H
#define QT_VISUALIZATION_SAMPLES_H

#include <QLineSeries>
#include <QChart>
#include <QChartView>
#include <QValueAxis>

#include "abs_view.h"
#include "BDSP.h"


class SamplesChart : public QChart {
public:
    QLineSeries *series = nullptr;

    SamplesChart();
    void changeDataSize(size_t newSize);
private:
    QValueAxis *axisX{};
    QValueAxis *axisY{};

};

class SamplesChartView : public ABSView {
public:
    SamplesChartView();
    void execPacket(Packet &packet) override;
    SamplesChart *chart() const;
private:
    size_t dataSize = 0;
    int *data = nullptr;
    ~SamplesChartView() override;
    void refreshGuiData() const override;
};

#endif //QT_VISUALIZATION_SAMPLES_H

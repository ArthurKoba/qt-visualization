#ifndef QT_VISUALIZATION_APP_H
#define QT_VISUALIZATION_APP_H

#include <QApplication>
#include <QMainWindow>
#include <QLineSeries>
#include <QChartView>
#include <QSplitter>
#include <QBarSeries>
#include <QList>


#include "BDSP.h"
#include "serial.h"

class QApplicationCustom : public QApplication {
public:
    QApplicationCustom(int &argc, char **argv, int = ApplicationFlags);

    QLineSeries *samplesSeries = nullptr;
    QChart *samplesChart = nullptr;
    QChartView *samplesChartView = nullptr;

    QBarSeries *amplitudesSeries = nullptr;
    QChart *amplitudesChart = nullptr;
    QBarSet *amplitudesSet = nullptr;
    QChartView *amplitudesChartView = nullptr;

    Serial serial;
    void packet_execute(Packet &packet) const;
    void configure_charts();


private:
    QSplitter *splitter;
    QMainWindow window;
    BDSPReceiver receiver;
};


#endif //QT_VISUALIZATION_APP_H

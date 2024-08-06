#ifndef QT_VISUALIZATION_APP_H
#define QT_VISUALIZATION_APP_H

#include <QApplication>
#include <QMainWindow>
#include <QSplitter>

#include <QLineSeries>
#include <QChartView>
#include <QBarSeries>

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

    int16_t samples[256]{};
    uint8_t amplitudes[128]{};

    Serial serial;
    void packet_execute(Packet &packet);
    void configure_charts();
    void refreshGuiData();


private:
    QTimer *timer;
    QSplitter *splitter;
    QMainWindow window;
    BDSPReceiver receiver;
};


#endif //QT_VISUALIZATION_APP_H

#ifndef QT_VISUALIZATION_APP_H
#define QT_VISUALIZATION_APP_H

#include <QApplication>
#include <QMainWindow>
#include <QSplitter>

#include "charts/samples.h"
#include "charts/amplitudes.h"

#include "BDSP.h"
#include "serial.h"

class Application : public QApplication {
public:

    SamplesChartView *samplesChartView = nullptr;
    AmplitudesChartView *amplitudesChartView = nullptr;

    Serial serial;

    Application(int &argc, char **argv, int = ApplicationFlags);

private:
    QSplitter *splitter;
    QMainWindow window;
    BDSPReceiver receiver;
};


#endif //QT_VISUALIZATION_APP_H

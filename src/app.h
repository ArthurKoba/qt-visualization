#ifndef QT_VISUALIZATION_APP_H
#define QT_VISUALIZATION_APP_H

#include <QApplication>
#include <QMainWindow>
#include <QSplitter>

#include "charts/samples.h"
//#include "charts/amplitudes.h"

//#include "BDSP.h"
//#include "communication/serial.h"

//#include "wasapi.h"
#include "audio/loopback/types.h"

class Application : public QApplication {
public:

    SamplesChartView *samplesChartView1 = nullptr;
    SamplesChartView *samplesChartView2 = nullptr;
//    AmplitudesChartView *amplitudesChartView = nullptr;

//    Serial serial;

    Application(int &argc, char **argv, int = ApplicationFlags);
    ~Application();
private:
    audio::loopback::IAudioLoopback *loopback;
    QSplitter *splitter;
    QMainWindow window;
//    BDSPReceiver receiver;
};


#endif //QT_VISUALIZATION_APP_H

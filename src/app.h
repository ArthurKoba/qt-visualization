#ifndef QT_VISUALIZATION_APP_H
#define QT_VISUALIZATION_APP_H

#include <QApplication>
#include <QMainWindow>
#include <QSplitter>
#include <QSerialPort>

#include "analyzer/analyzer.h"
#include "analyzer/generator/generator.h"
#include "loopback/factory.h"
#include "charts/abs.h"
#include "surfacegraph.h"
#include "BDSP/receivers.h"

class Application : public QApplication {
public:
    struct Config {
        bool run_loopback = true;
        bool run_serial = false;
        bool run_generator = false;
        bool run_analyser = true;
        bool show_serial_samples = false;
        bool show_generator_samples = false;
        bool show_raw_samples = false;
        bool show_samples = false;
        bool show_amplitudes = true;
        bool show_test_amplitudes = true;
        bool show_serial_fast_amplitudes = false;
        bool show_serial_audio_spectre = false;
        bool show_surface = true;
    };

    Application(int &argc, char **argv, int = ApplicationFlags);
    ~Application() override;
private:
    Config _cfg;

    AbstractChartView *rawSamplesView = nullptr;
    AbstractChartView *serialSamplesView = nullptr;
    AbstractChartView *serialFastAmplitudesView = nullptr;
    AbstractChartView *serialSpectreView = nullptr;
    AbstractChartView *generatorSamplesView = nullptr;
    AbstractChartView *samplesView = nullptr;
    AbstractChartView *amplitudesView = nullptr;
    AbstractChartView *testAmplitudesView = nullptr;
    SurfaceGraph *surfaceView = nullptr;

    Analyzer *analyzer = nullptr;
    Generator *generator = nullptr;
    audio::loopback::IAudioLoopback *loopback = nullptr;
    QSerialPort *serial{};
    BDSP::COBSZPEReceiver *receiver = nullptr;
    QSplitter *splitter;
    QTabWidget tabWidget;

    void _run_analyzer();
    void _run_generator();
    void _run_loopback();
    void _run_serial();
};


#endif //QT_VISUALIZATION_APP_H

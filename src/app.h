#ifndef QT_VISUALIZATION_APP_H
#define QT_VISUALIZATION_APP_H

#include <QApplication>
#include <QMainWindow>
#include <QSplitter>
#include <QSerialPort>

#include "analyzer/analyzer.h"
#include "analyzer/generator/generator.h"
#include "loopback/factory.h"
#include "charts/fps.h"
#include "BDSP/receiver.h"
#include "BDSP/streams/cobs/reader.h"

class Application : public QApplication {
public:
    struct Config {
        bool run_loopback = false;
        bool run_serial = true;
        bool run_generator = false;
        bool run_analyser = false;
        bool show_serial_samples = false;
        bool show_generator_samples = false;
        bool show_raw_samples = false;
        bool show_samples = false;
        bool show_amplitudes = false;
        bool show_serial_audio_spectre = true;
    };

    Application(int &argc, char **argv, int = ApplicationFlags);
    ~Application() override;
private:
    Config _cfg;

    FPSChartView *rawSamplesView = nullptr;
    FPSChartView *serialSamplesView = nullptr;
    FPSChartView *serialSpectreView = nullptr;
    FPSChartView *generatorSamplesView = nullptr;
    FPSChartView *samplesView = nullptr;
    FPSChartView *amplitudesView = nullptr;
    FPSChartView *amplitudesView2 = nullptr;
    Analyzer *analyzer = nullptr;
    Generator *generator = nullptr;
    audio::loopback::IAudioLoopback *loopback = nullptr;
    QSerialPort *serial{};
    BDSP::BDSPReceiver *receiver = nullptr;
    BDSP::streams::cobs::COBSZPEReaderStream *reader_stream = nullptr;
    QSplitter *splitter;
    QMainWindow window;

    void _run_analyzer();
    void _run_generator();
    void _run_loopback();
    void _run_serial();
};


#endif //QT_VISUALIZATION_APP_H

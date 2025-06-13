#ifndef QT_VISUALIZATION_APP_H
#define QT_VISUALIZATION_APP_H

#include <QApplication>
#include <QMainWindow>
#include <QSplitter>

#include "analyzer/analyzer.h"
#include "analyzer/generator/generator.h"
#include "loopback/factory.h"
#include "charts/fps.h"

class Application : public QApplication {
public:
    struct Config {
        bool run_loopback = true;
        bool run_generator = false;
        bool run_analyser = true;
        bool show_generator_samples = false;
        bool show_raw_samples = false;
        bool show_samples = false;
        bool show_amplitudes = true;
    };

    Application(int &argc, char **argv, int = ApplicationFlags);
    ~Application() override;
private:
    Config _cfg;

    FPSChartView *rawSamplesView = nullptr;
    FPSChartView *generatorSamplesView = nullptr;
    FPSChartView *samplesView = nullptr;
    FPSChartView *amplitudesView = nullptr;
    FPSChartView *amplitudesView2 = nullptr;
    Analyzer *analyzer = nullptr;
    Generator *generator = nullptr;
    audio::loopback::IAudioLoopback *loopback = nullptr;
    QSplitter *splitter;
    QMainWindow window;

    void _run_analyzer();
    void _run_generator();
    void _run_loopback();
};


#endif //QT_VISUALIZATION_APP_H

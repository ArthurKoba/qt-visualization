#include "app.h"

Application::Application(int &argc, char **argv, int flags) : QApplication(argc, argv, flags) {

    if (_cfg.run_analyser) {
        _run_analyzer();
    }

    if (_cfg.run_loopback) {
        _run_loopback();
    }

    if (_cfg.run_generator) {
        _run_generator();
    }

    splitter = new QSplitter;
    splitter->setOrientation(Qt::Vertical);

    if (_cfg.show_raw_samples and loopback) {
        rawSamplesView = new FPSChartView();
        rawSamplesView->axisY.setRange(-1, 1);
        rawSamplesView->chart()->setTitle("Raw samples");
        splitter->addWidget(rawSamplesView);
    } else if (_cfg.show_raw_samples) {
        qInfo("Raw samples only can show when loopback running");
    }

    if (_cfg.show_generator_samples and generator) {
        generatorSamplesView = new FPSChartView();
        generatorSamplesView->axisY.setRange(-1, 1);
        generatorSamplesView->chart()->setTitle("Generator samples");
        splitter->addWidget(generatorSamplesView);
    } else if (_cfg.show_generator_samples) {
        qInfo("Generator samples only can show when generator running");
    }

    if (_cfg.show_samples and analyzer) {
        samplesView = new FPSChartView();
        samplesView->axisY.setRange(-1, 1);
        samplesView->chart()->setTitle("Analyzer samples");
        splitter->addWidget(samplesView);
    } else if (_cfg.show_samples) {
        qInfo("Analyzer samples only can show when analyzer running");
    }

    if (_cfg.show_amplitudes and analyzer) {
        amplitudesView = new FPSChartView();
        amplitudesView2 = new FPSChartView();

//        amplitudesView->axisY.setRange(0, 1);
        amplitudesView->chart()->setTitle("Amplitudes FFT 2-radix");
        amplitudesView->setAutoResizing(true);

//        amplitudesView2->axisY.setRange(-0.10, 1.2);
        amplitudesView2->chart()->setTitle("Amplitudes FHT 2-radix tests");
        amplitudesView2->setAutoResizing(true);

        splitter->addWidget(amplitudesView);
        splitter->addWidget(amplitudesView2);
    } else if (_cfg.show_amplitudes) {
        qInfo("Analyzer amplitudes only can show when analyzer running");
    }

    window.setCentralWidget(splitter);
    window.resize(1200, 800);
//    window.grabGesture(Qt::PanGesture);
//    window.grabGesture(Qt::PinchGesture);
    window.show();
}

Application::~Application() {
    if (generator) {
        generator->stop();
        qInfo("Generator stopped successfully");
    }
    if (analyzer) {
        analyzer->stop();
        qInfo("Analyzer stopped successfully");
    }
    if (loopback) {
        loopback->set_audio_handler(nullptr);
        loopback->stop();
        qInfo("Loopback stopped successfully");
    }
}

void Application::_run_analyzer() {
    if (analyzer) {
        return qDebug("Analyzer already started");
    }

    analyzer = new Analyzer;

    analyzer->set_update_handler([this]() {
        if (samplesView) {
            samplesView->update(analyzer->samples.left);
        }

        if (amplitudesView) {
            amplitudesView->update(analyzer->amplitudes.left);
        }
        if (amplitudesView2) {
            amplitudesView2->update(analyzer->amplitudes.right);
        }
    });

    analyzer->start();
}


void Application::_run_loopback() {
    if (loopback) {
        return qDebug("Loopback already started");
    }
    if (not analyzer) {
        return qDebug("Analyser not exist");
    }

    loopback = audio::loopback::LoopbackFactory::get_loopback();

    if (not loopback) {
        return qCritical("Failed get loopback from Factory");
    }

    loopback->set_audio_handler([this](audio::loopback::loopback_audio data) {
        Samples samples(data.samples);
        for (int i = 0; i < data.samples; ++i) {
            samples.left[i] = data.data[i];
            samples.right[i] = data.data[i + data.samples];
        }
        if (_cfg.show_raw_samples and rawSamplesView) {
            rawSamplesView->update(samples.left);
        }
        if (analyzer) {
            analyzer->add_samples(samples);
        }
    });

    loopback->start();
}

void Application::_run_generator() {
    if (generator) {
        return qDebug("Generator already started");
    }

    generator = new Generator;

    generator->set_handler([this](std::vector<float> data) {
        Samples samples(data.size());
        for (int i = 0; i < data.size(); ++i) {
            samples.left[i] = data[i];
            samples.right[i] = data[i];
        }
        if (generatorSamplesView) {
            generatorSamplesView->update(samples.left);
        }
    });
    generator->start();
}

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

    if (_cfg.run_serial) {
        _run_serial();
        serialView = new FPSChartView(FPSChartView::BAR);
//        serialSamplesView->axisY.setRange(-100, 100);
        serialView->chart()->setTitle("Serial");
        serialView->setAutoResizing(true);
//        std::vector<float> test;
//        for (int i = 0; i < 64; ++i) {
//            test.push_back(sin(float(i)));
//        }
//        serialSamplesView->update(test);
//        splitter->addWidget(serialSamplesView);
        window.setCentralWidget(serialView);
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
    if (not _cfg.run_serial) {
        window.setCentralWidget(splitter);
    }
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

    if (serial) {
        serial->close();
        qInfo("Serial stopped successfully");
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

void Application::_run_serial() {
    reader_stream = new BDSP::streams::cobs::COBSZPEReaderStream;

    auto cfg = reader_stream->get_strategy().get_config();
    cfg.delimiter_byte = '\n';
    reader_stream->get_strategy().set_config(cfg);
    receiver = new BDSP::BDSPReceiver;
    receiver->set_stream_reader(reader_stream);
    receiver->set_error_handler([](BDSP::parse_packet_status_t error, void *ctx) {
        qInfo("receiver error: %d", error);
    }, nullptr);

    if (serial) {
        delete serial;
    }
    serial = new QSerialPort(this);

    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setDataBits(QSerialPort::Data8);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    serial->setBaudRate(1000000);
    serial->setReadBufferSize(64);
    serial->setPortName("COM8");


    connect(serial, &QSerialPort::readyRead, this, [this]() {
        if (this->serial) {
            auto data = this->serial->readAll();
//        qInfo("Got data. Size bytes: %d\n", data.size());
            reader_stream->read(reinterpret_cast<uint8_t *>(data.data()), data.size());
        };
    });

    receiver->set_packet_handler([](BDSP::packet_context_t &packet_context, void *packet_handler_context) {
        auto &app = *reinterpret_cast<Application *>(packet_handler_context);
        if (not app.serialView) return;
        if (packet_context.packet_id not_eq 1) {
            qInfo("Got unknown packet. Packet ID: %d", packet_context.packet_id);
            return;
        }
        static uint32_t j = 0;
//        qInfo("[%d] Got packet. Packet ID: %d, size: %d", j++, packet_context.packet_id, packet_context.size);
//        return;
        size_t samples_size = packet_context.size;
        samples_size /= 2;
        std::vector<float> samples;
        auto *packet_samples = reinterpret_cast<int16_t *>(packet_context.data_ptr);
        for (int i = 0; i < samples_size; ++i) {
            samples.push_back(float(packet_samples[i]));
        }
        app.serialView->update(samples);
    }, this);


    if (serial->open(QSerialPort::ReadOnly)) {
        qInfo("port opened");
    }
}
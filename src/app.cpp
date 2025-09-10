#include "app.h"

#include "charts/line.h"
#include "charts/bar.h"
#include "charts/freq.h"

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

    }

    splitter = new QSplitter;
    splitter->setOrientation(Qt::Vertical);



    if (_cfg.show_serial_samples) {
        serialSamplesView = new LineChartView();
        serialSamplesView->set_range(-100, 100);
        serialSamplesView->chart()->setTitle("Serial Samples");
        serialSamplesView->set_auto_resizing(true);
        serialSamplesView->set_auto_gain(true, 100, -100);
        splitter->addWidget(serialSamplesView);
    }

    if (_cfg.show_serial_audio_spectre) {
        serialSpectreView = new BarChartView();
//        serialSpectreView->axisX.setTickCount(31);
//        serialSpectreView->axisX.setLabelFormat("%.0f");
//        serialSpectreView->axisX.setMinorTickCount(9);
        serialSpectreView->chart()->setTitle("Serial Spectre");
        serialSpectreView->set_auto_resizing(true);
        serialSpectreView->set_auto_gain(true, 255, 0, 2);
        serialSpectreView->set_range(0, 255);

        splitter->addWidget(serialSpectreView);
    }



    if (receiver and (_cfg.show_serial_samples or _cfg.show_serial_audio_spectre)) {
        receiver->set_packet_handler([](BDSP::packet_context_t &packet_context, void *packet_handler_context) {
            auto &app = *reinterpret_cast<Application *>(packet_handler_context);
            if (packet_context.packet_id < 1 or packet_context.packet_id > 2) {
                qInfo("Got unknown packet. Packet ID: %d", packet_context.packet_id);
                return;
            }
            if (packet_context.packet_id == 1 and app.serialSamplesView) {
                size_t samples_size = packet_context.size;
                samples_size /= 2;
                std::vector<float> samples;
                auto *packet_samples = reinterpret_cast<int16_t *>(packet_context.data_ptr);
                for (int i = 0; i < samples_size; ++i) {
                    samples.push_back(float(packet_samples[i]));
                }
                app.serialSamplesView->update(samples);
                return;
            } else if (packet_context.packet_id == 2 and app.serialSpectreView) {
                size_t spectre_size = packet_context.size;
                spectre_size = 300;
//                spectre_size /= 2;
                std::vector<float> spectre;
                auto *packet_samples = reinterpret_cast<uint8_t *>(packet_context.data_ptr);
                for (int i = 0; i < spectre_size; ++i) {
                    spectre.push_back(float(packet_samples[i]));
                }
                app.serialSpectreView->update(spectre);
                return;
            }
//            static uint32_t j = 0;
//        qInfo("[%d] Got packet. Packet ID: %d, size: %d", j++, packet_context.packet_id, packet_context.size);
//        return;
            qWarning("Incorrect serial packet processing\n");
        }, this);
    }


    if (_cfg.show_raw_samples and loopback) {
        rawSamplesView = new LineChartView();
        rawSamplesView->set_range(-1, 1);
        rawSamplesView->chart()->setTitle("Raw samples");
        splitter->addWidget(rawSamplesView);
    } else if (_cfg.show_raw_samples) {
        qInfo("Raw samples only can show when loopback running");
    }

    if (_cfg.show_generator_samples and generator) {
        generatorSamplesView = new LineChartView();
        generatorSamplesView->set_range(-1, 1);
        generatorSamplesView->chart()->setTitle("Generator samples");
        splitter->addWidget(generatorSamplesView);
    } else if (_cfg.show_generator_samples) {
        qInfo("Generator samples only can show when generator running");
    }

    if (_cfg.show_samples and analyzer) {
        samplesView = new LineChartView();
        samplesView->set_range(-1, 1);
        samplesView->chart()->setTitle("Analyzer samples");
        splitter->addWidget(samplesView);
    } else if (_cfg.show_samples) {
        qInfo("Analyzer samples only can show when analyzer running");
    }

    if (_cfg.show_amplitudes and analyzer) {
        amplitudesView = new FreqChartView();

        amplitudesView->set_range(0, 1);
        amplitudesView->chart()->setTitle("Amplitudes FFT 4-radix");
        amplitudesView->set_auto_resizing(true);

        splitter->addWidget(amplitudesView);
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
            samplesView->update(analyzer->samples.left.get_window());
        }

        if (amplitudesView) {
            amplitudesView->update(analyzer->amplitudes.left);
        }
    });

    analyzer->start();
}


void Application::_run_loopback() {
    if (loopback) {
        return qDebug("Loopback already started");
    }

    loopback = audio::loopback::LoopbackFactory::get_loopback();

    if (not loopback) {
        return qCritical("Failed get loopback from Factory");
    }

    loopback->set_audio_handler([this](audio::loopback::loopback_audio data) {
        std::vector<float> left;
        std::vector<float> right;
        left.resize(data.samples);
        right.resize(data.samples);
        for (int i = 0; i < data.samples; ++i) {
            left[i] = data.data[i];
            right[i] = data.data[i + data.samples];
        }
        if (_cfg.show_raw_samples and rawSamplesView) {
            rawSamplesView->update(left);
        }
        if (analyzer) {
            analyzer->add_samples(left, right);
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
        if (generatorSamplesView) {
            generatorSamplesView->update(data);
        }
    });
    generator->start();
}

void Application::_run_serial() {
    reader_stream = new BDSP::streams::cobs::COBSZPEReaderStream;
//    reader_stream = new BDSP::streams::ppp::PPPReaderStream;

    auto cfg = reader_stream->get_strategy().get_config();
    cfg.delimiter_byte = '\n';
    cfg.size_of_the_sequence_to_be_replaced = 4;
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



    if (serial->open(QSerialPort::ReadOnly)) {
        qInfo("port opened");
    }
}
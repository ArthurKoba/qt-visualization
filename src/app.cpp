#include "app.h"
#include <cinttypes>
#include <chrono>
#include <QValueAxis>
#include <QBarSeries>
#include <QBarSet>


uint64_t get_time() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();;
}

void QApplicationCustom::packet_execute(Packet &packet) const {

    if (packet.id == 1) return;

    static uint64_t last_update = 0;
    if (get_time() - last_update < 1000/60) return;
    last_update = get_time();

    if (packet.id == 1 and packet.size / 2 == 256) {
        auto *samples = reinterpret_cast<int16_t*>(packet.data_ptr);
        int samples_size = packet.size / 2;
        for (int i = 0; i < samples_size; i++) {
            samplesSeries->replace(i, i, samples[i]);
        }
    }

    if (packet.id == 2 and packet.size == 128) {
        for (int i = 0; i < 256; ++i) {
            amplitudesSet->replace(i, packet.data_ptr[i]);
        }
    }
}


QApplicationCustom::QApplicationCustom(int &argc, char **argv, int flags) : QApplication(argc, argv, flags) {

    serial.set_data_handler([] (uint8_t *data_p, size_t size, void *ctx) {
        auto &receiver_ = *reinterpret_cast<BDSPReceiver*>(ctx);
        receiver_.parse(data_p, size);
    }, &receiver);

    COBS::config_t config = {.delimiter = '\n', .depth = 255};

    receiver.set_config(config, [] (Packet &packet, void *ctx) {
        auto &t = *reinterpret_cast<QApplicationCustom*>(ctx);
        t.packet_execute(packet);
    }, this);

    configure_charts();

    splitter = new QSplitter;
    splitter->setOrientation(Qt::Vertical);

    samplesChartView = new QChartView(samplesChart);
    amplitudesChartView = new QChartView(amplitudesChart);

    splitter->addWidget(samplesChartView);
    splitter->addWidget(amplitudesChartView);


    QTimer *t = new QTimer(this);
    t->setInterval(1000);
    connect(t, SIGNAL(timeout()), this, SLOT(refreshGuiData()));
    t->start();

    window.setCentralWidget(splitter);
    window.resize(800, 400);
//    window.grabGesture(Qt::PanGesture);
//    window.grabGesture(Qt::PinchGesture);
    window.show();
}

void QApplicationCustom::configure_charts() {
    samplesChart = new QChart;
    samplesChart->legend()->hide();
    samplesChart->setTitle("Samples");
    samplesChart->setAnimationOptions(QChart::NoAnimation);

    auto samplesAxisX = new QValueAxis;
    auto samplesAxisY = new QValueAxis;
    samplesAxisX->setRange(0, 255);
    samplesAxisY->setRange(-600, 600);

    samplesSeries = new QLineSeries;
    samplesSeries->setUseOpenGL(true);
    samplesChart->addSeries(samplesSeries);

    samplesChart->addAxis(samplesAxisX, Qt::AlignBottom);
    samplesChart->addAxis(samplesAxisY, Qt::AlignLeft);
    samplesSeries->attachAxis(samplesAxisX);
    samplesSeries->attachAxis(samplesAxisY);

    for (int i = 0; i < 256; i++) *samplesSeries << QPointF(i, 0);

    amplitudesChart = new QChart;
    amplitudesChart->legend()->hide();
    amplitudesChart->setTitle("Amplitudes");
    amplitudesChart->setAnimationOptions(QChart::NoAnimation);

    auto amplitudesAxisX = new QValueAxis;
    auto amplitudesAxisXAxisY = new QValueAxis;
    amplitudesAxisX->setRange(0, 128);
    amplitudesAxisXAxisY->setRange(0, 255);


    amplitudesSeries = new QBarSeries;
    amplitudesSeries->setUseOpenGL();
    amplitudesSet = new QBarSet("Amplitudes");
    amplitudesSeries->append(amplitudesSet);
    amplitudesChart->addSeries(amplitudesSeries);
    amplitudesChart->addAxis(amplitudesAxisX, Qt::AlignBottom);
    amplitudesChart->addAxis(amplitudesAxisXAxisY, Qt::AlignLeft);
    amplitudesSeries->attachAxis(amplitudesAxisX);
    amplitudesSeries->attachAxis(amplitudesAxisXAxisY);

    for (int i = 0; i < 128; ++i) *amplitudesSet << 0;
}



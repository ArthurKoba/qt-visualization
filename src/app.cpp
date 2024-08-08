#include "app.h"
#include <cinttypes>


Application::Application(int &argc, char **argv, int flags) : QApplication(argc, argv, flags) {

    COBS::config_t config = {.delimiter = '\n', .depth = 255};

    receiver.set_config(config, [] (Packet &packet, void *ctx) {
        auto &t = *reinterpret_cast<Application*>(ctx);
        if (t.samplesChartView) t.samplesChartView->execPacket(packet);
        if (t.amplitudesChartView) t.amplitudesChartView->execPacket(packet);
    }, this);

    serial.set_data_handler([] (uint8_t *data_p, size_t size, void *ctx) {
        auto &receiver_ = *reinterpret_cast<BDSPReceiver*>(ctx);
        receiver_.parse(data_p, size);
    }, &receiver);

    samplesChartView = new SamplesChartView;
    amplitudesChartView = new AmplitudesChartView;

    splitter = new QSplitter;
    splitter->setOrientation(Qt::Vertical);

    splitter->addWidget(samplesChartView);
    splitter->addWidget(amplitudesChartView);

    window.setCentralWidget(splitter);
    window.resize(800, 400);
//    window.grabGesture(Qt::PanGesture);
//    window.grabGesture(Qt::PinchGesture);
    window.show();
}
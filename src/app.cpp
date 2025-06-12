#include "app.h"

#include "audio/loopback/factory.h"

Application::Application(int &argc, char **argv, int flags) : QApplication(argc, argv, flags) {

    loopback = audio::loopback::LoopbackFactory::get_loopback();

    samplesChartView1 = new SamplesChartView;
    samplesChartView2 = new SamplesChartView;
//    amplitudesChartView = new AmplitudesChartView;

    loopback->set_audio_handler([this] (audio::loopback::loopback_audio data) {
        Packet packet1;
        packet1.id = 10;
        packet1.size = data.samples;
        packet1.data_ptr = data.data;
        Packet packet2 = packet1;
        packet2.data_ptr = data.data + data.samples;
        samplesChartView1->execPacket(packet1);
        samplesChartView2->execPacket(packet2);
    });


    splitter = new QSplitter;
    splitter->setOrientation(Qt::Vertical);

    splitter->addWidget(samplesChartView1);
    splitter->addWidget(samplesChartView2);

    window.setCentralWidget(splitter);
    window.resize(800, 400);
//    window.grabGesture(Qt::PanGesture);
//    window.grabGesture(Qt::PinchGesture);

    loopback->start();
    window.show();
}

Application::~Application() {
}

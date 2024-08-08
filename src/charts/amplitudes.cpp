#include "amplitudes.h"
#include <QValueAxis>

AmplitudesChart::AmplitudesChart() {
    legend()->hide();
    setTitle("Amplitudes");
    setAnimationOptions(QChart::NoAnimation);

    axisX = new QValueAxis;
    axisY = new QValueAxis;

    axisY->setRange(0, 128);
    axisX->setTickCount(32);

    series = new QBarSeries;
    series->setUseOpenGL();
    series->append(sets);
    addSeries(series);
    addAxis(axisX, Qt::AlignBottom);
    addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);
}

void AmplitudesChart::changeDataSize(size_t newSize) {
    axisX->setRange(0, static_cast<double>(newSize - 1));
    delete sets;
    sets = new QBarSet("Amplitudes");
    for (int i = 0; i < newSize; i++) *sets << 0;
}





AmplitudesChartView::AmplitudesChartView() {
    setChart(new AmplitudesChart);
    timer->start(1000/60);
    dataSize = 128;
    data = new int[dataSize];
    for (int i = 0; i < dataSize; ++i) data[i] = i;
    chart()->changeDataSize(dataSize);
}

void AmplitudesChartView::execPacket(Packet &packet) {
    if (packet.id not_eq 2) return;

    size_t numberOfFromPacket = packet.size;

    if (numberOfFromPacket != dataSize) {
        delete [] data;
        data = new int[numberOfFromPacket];
        dataSize = numberOfFromPacket;
        chart()->changeDataSize(numberOfFromPacket);
    }

    const auto *new_samples = reinterpret_cast<int16_t*>(packet.data_ptr);
    for (int i = 0; i < dataSize; i++) data[i] = new_samples[i];
}

AmplitudesChartView::~AmplitudesChartView() {
    delete [] data;
}

void AmplitudesChartView::refreshGuiData() const {
    if (not data) return;
    for (int i = 0; i < dataSize; ++i) {
        chart()->sets->replace(i, data[i]);
    }
}

AmplitudesChart *AmplitudesChartView::chart() const {
    return dynamic_cast<AmplitudesChart*>(QChartView::chart());
}

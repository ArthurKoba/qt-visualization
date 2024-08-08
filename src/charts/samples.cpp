#include "samples.h"


SamplesChart::SamplesChart() {
    legend()->hide();
    setTitle("Samples");
    setAnimationOptions(QChart::NoAnimation);

    axisX = new QValueAxis;
    axisY = new QValueAxis;

    axisY->setRange(-600, 600);

    series = new QLineSeries;
    series->setUseOpenGL(true);
    addSeries(series);

    addAxis(axisX, Qt::AlignBottom);
    addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);
}

void SamplesChart::changeDataSize(size_t newSize) {
    axisX->setRange(0, static_cast<double>(newSize - 1));
    series->clear();
    for (int i = 0; i < newSize; i++) *series << QPointF(0, 0);
}




SamplesChartView::SamplesChartView() {
    timer->start(1000/60);
    setChart(new SamplesChart);
    timer->start(1000/60);
    dataSize = 256;
    data = new int[dataSize];
    for (int i = 0; i < dataSize; ++i) data[i] = i;
    chart()->changeDataSize(dataSize);
}

SamplesChartView::~SamplesChartView() {
    delete [] data;
}

void SamplesChartView::execPacket(Packet &packet) {
    if (packet.id not_eq 1) return;

    size_t numberOfSamplesFromPacket = packet.size / 2;

    if (numberOfSamplesFromPacket != dataSize) {
        delete [] data;
        data = new int[numberOfSamplesFromPacket];
        dataSize = numberOfSamplesFromPacket;
        chart()->changeDataSize(numberOfSamplesFromPacket);
    }

    const auto *newSamples = reinterpret_cast<int16_t*>(packet.data_ptr);
    for (int i = 0; i < dataSize; i++) data[i] = newSamples[i];
}

void SamplesChartView::refreshGuiData() const {
    if (not data) return;
    for (int i = 0; i < dataSize; ++i) {
        chart()->series->replace(i, i, data[i]);
    }
}


SamplesChart *SamplesChartView::chart() const {
    return dynamic_cast<SamplesChart*>(QChartView::chart());
}

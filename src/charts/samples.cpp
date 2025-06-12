#include "samples.h"


SamplesChart::SamplesChart() {
    legend()->hide();
    setTitle("Samples");
    setAnimationOptions(QChart::NoAnimation);

    axisX = new QValueAxis;
    axisY = new QValueAxis;

    int range = 1;
    axisY->setRange(-range, range);

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


void SamplesChartView::updateDataSize(size_t size) {
    if (size == dataSize) return;
    dataSize = size;
    delete [] data;
    data = new double[dataSize];
    chart()->changeDataSize(dataSize);
}

SamplesChartView::SamplesChartView() {
    timer->start(1000/60);
    setChart(new SamplesChart);
    timer->start(1000/60);
    updateDataSize(100);
    for (int i = 0; i < dataSize; ++i) data[i] = i;
}

SamplesChartView::~SamplesChartView() {
    delete [] data;
}

void SamplesChartView::execPacket(Packet &packet) {
    updateDataSize(packet.size);

    size_t numberOfSamplesFromPacket = packet.size;

    for (int i = 0; i < dataSize; ++i) {
        data[i] = packet.data_ptr[i];
    }

//    if (packet.id == 1) {
//        const auto *newSamples = reinterpret_cast<int16_t*>(packet.data_ptr);
//        for (int i = 0; i < dataSize; i++) data[i] = newSamples[i];
//    } else if (packet.id == 10) {
//        for (int i = 0; i < dataSize; i++) data[i] = i;
//    }

}

void SamplesChartView::refreshGuiData() const {
    if (not data) return;
    for (int i = 0; i < dataSize; ++i) {
        chart()->series->replace(i, i, data[i]);
    }
}


SamplesChart *SamplesChartView::chart() const {
    return reinterpret_cast<SamplesChart*>(QChartView::chart());
}

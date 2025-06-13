#include "fps.h"


void FPSChartView::_setFPS(qreal fps) {
    static QString labelText = QStringLiteral("FPS: %1");
    fpsLabel->setText(labelText.arg(QString::number(fps, 'f', 1)));
    fpsLabel->adjustSize();
}


FPSChartView::FPSChartView(): FPSMixin(this) {
    setChart(new QChart);
    chart()->legend()->hide();

    chart()->addAxis(&axisX, Qt::AlignBottom);
    chart()->addAxis(&axisY, Qt::AlignLeft);

    chart()->setTitle("Custom chart");

    series = new QLineSeries;
    series->setUseOpenGL(true);
    chart()->addSeries(series);
    series->attachAxis(&axisX);
    series->attachAxis(&axisY);

    axisY.setRange(-1, 1);

    fpsLabel = new QLabel(this);
    fpsLabel->setStyleSheet("color: red;");

    fpsLabel->move(10, 10);
    fpsLabel->adjustSize();
    fpsLabel->raise();
    fpsLabel->show();
    fpsLabel->raise();

    QObject::connect(&_dataUpdateTimer, &QTimer::timeout, this, &FPSChartView::updateChartData);

    _dataUpdateTimer.setInterval(1000 / 200);
    _dataUpdateTimer.start();
    setFPSCallback([this](qreal fps) {
        _setFPS(fps);
    });
}

void FPSChartView::updateChartData() {
    if (not need_update) return;
    QList<QPointF> points;
    points.reserve(_data.size());

    for (int i = 0; i < _data.size(); i++) {
        points.append(QPointF(i, _data[i]));
    }
    // Используйте replace вместо clear + append, это оптимизировано для производительности
    series->replace(points);
    _frameCall();
    need_update = false;
}

void FPSChartView::update(std::vector<float> &data) {

    if (data.size() not_eq _data.size()) {
        _data.resize(data.size(), 0);
        axisX.setRange(0, qreal(_data.size() - 1));
        need_update = true;
    }
    for (int i = 0; i < data.size(); ++i) {
        // todo AVX optimisation
        if (data[i] not_eq _data[i]) need_update = true;
        _data[i] = data[i];
    }
}




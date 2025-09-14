#include "fps.h"


void FPSChartView::_setFPS(qreal fps) {
    static QString labelText = QStringLiteral("FPS: %1");
    _fps_label->setText(labelText.arg(QString::number(fps, 'f', 1)));
    _fps_label->adjustSize();
}


FPSChartView::FPSChartView() : FPSMixin(this) {
    _fps_label = new QLabel(this);
    _fps_label->setStyleSheet("color: red;");

    _fps_label->move(10, 10);
    _fps_label->adjustSize();
    _fps_label->raise();
    _fps_label->show();
    _fps_label->raise();

    setFPSCallback([this](qreal fps) {
        _setFPS(fps);
    });
}

void FPSChartView::_on_chart_updated() {
    _frameCall();
}
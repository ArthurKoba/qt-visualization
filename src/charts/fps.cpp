#include <QBarSeries>
#include "fps.h"


void FPSChartView::_setFPS(qreal fps) {
    static QString labelText = QStringLiteral("FPS: %1");
    fpsLabel->setText(labelText.arg(QString::number(fps, 'f', 1)));
    fpsLabel->adjustSize();
}


FPSChartView::FPSChartView(ChartType type) : FPSMixin(this) {
    setChart(new QChart);
    chart()->legend()->hide();

    axisX.setTickCount(31);
    axisX.setLabelFormat("%.0f");
    axisX.setMinorTickCount(9);



    chart()->addAxis(&axisX, Qt::AlignBottom);
    chart()->addAxis(&axisY, Qt::AlignLeft);

    chart()->setTitle("Custom chart");

    _type = type;

    switch (_type) {
        case BAR:
            series = new QBarSeries;
            set = new QBarSet("Data");

            categoryAxis.append("LOW", 20);
            categoryAxis.append("MIDDLE", 250);
            categoryAxis.append("HIGH", 300);
            chart()->addAxis(&categoryAxis, Qt::AlignBottom);
//            chart()->addAxis(&freqAxis, Qt::AlignBottom);

            reinterpret_cast<QBarSeries *>(series)->append(set);
            break;
        default:
            series = new QLineSeries;
            break;
    }

    if (set) {
        QPen pen(Qt::blue, 3, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin);
        pen.setWidth(3);
        set->setPen(pen);
    }

    series->setUseOpenGL(true);
    chart()->addSeries(series);
    series->attachAxis(&axisX);
    series->attachAxis(&axisY);

    lastMax = -std::numeric_limits<float>::infinity();
    lastMin = std::numeric_limits<float>::infinity();

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
    if (_type == BAR) {
        set->remove(0, set->count());
        for (float i : _data) {
            set->append(i);
        }
    } else {
        // Используйте replace вместо clear + append, это оптимизировано для производительности
        QList<QPointF> points;
        points.reserve(_data.size());
        for (int i = 0; i < _data.size(); i++) {
            points.append(QPointF(i * 11, _data[i]));
        }
        reinterpret_cast<QLineSeries *>(series)->replace(points);
    }
    _frameCall();
    need_update = false;
}

void FPSChartView::update(std::vector<float> &data) {

    if (data.size() not_eq _data.size()) {
        _data.resize(data.size(), 0);
        axisX.setRange(0, qreal(_data.size()));
        categoryAxis.setRange(0, qreal(_data.size()));

        if (_type == BAR) {
//            freqAxis.setRange(0, qreal(_data.size()));
//            auto labels = freqAxis.categoriesLabels();
//            for (const auto& label : labels) {
//                freqAxis.remove(label);
//            }
//            for (int i = 0; i < _data.size() / 10; ++i) {
//                freqAxis.append(QString::number(qreal(float(i) * 11.72f)), i * 10);
//            }
        }

        need_update = true;
    }
    for (int i = 0; i < data.size(); ++i) {
        // todo AVX optimisation
        if (data[i] not_eq _data[i]) need_update = true;
        _data[i] = data[i];
    }
    if (auto_resizing) {
        auto max = std::max_element(data.begin(), data.end());
        auto min = std::min_element(data.begin(), data.end());
        static unsigned int counter = 0;

        bool needUpdate = false;

        if (counter++ > 10) {
            lastMax *= 0.9;
            lastMin *= 0.9;

            if (lastMax < 100) lastMax = 100;
            counter = 0;
            needUpdate = true;
        }

        if (*max > lastMax) {
            lastMax = *max;
            needUpdate = true;
        }
        if (*min < lastMin) {
            lastMin = *min;
            needUpdate = true;
        }
        if (needUpdate) {
            axisY.setRange(lastMin, lastMax);
        }
    }
}

void FPSChartView::setAutoResizing(bool value) {
    auto_resizing = value;
}

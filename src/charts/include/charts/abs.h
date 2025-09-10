#ifndef QT_VISUALIZATION_ABS_H
#define QT_VISUALIZATION_ABS_H

#include <vector>
#include <QChartView>
#include <QValueAxis>
#include <QTimer>

class AbstractChartView : public QChartView {
protected:
    std::vector<float> _data;
    bool _need_update = false;
    bool _auto_resizing = false;
    bool _auto_gain = false;
    float _gain_min_max{};
    float _gain_min_min{};
    size_t _gain_speed{};
    float _last_min_value;
    float _last_max_value;

    QTimer _data_update_timer;
    QAbstractSeries *_series;
    QValueAxis _axis_x;
    QValueAxis _axis_y;

    void _init_series();
    virtual void _update_chart_data() = 0;
    virtual void _on_update() {};
public:
    AbstractChartView();
    void set_auto_resizing(bool value);
    void set_auto_gain(bool value, float min_max = 100.0f, float min_min = -100.0f, size_t speed = 10);
    void set_range(qreal min, qreal max);
    void update(std::vector<float> &data);
};

#endif //QT_VISUALIZATION_ABS_H

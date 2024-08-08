#ifndef QT_VISUALIZATION_ABS_VIEW_H
#define QT_VISUALIZATION_ABS_VIEW_H

#include <QChartView>
#include <QTimer>
#include <BDSP.h>


class ABSView : public QChartView {
public:
    virtual void execPacket(Packet &packet) = 0;
protected:
    ABSView();
    QTimer *timer = nullptr;
    virtual void refreshGuiData() const = 0;
};

#endif //QT_VISUALIZATION_ABS_VIEW_H

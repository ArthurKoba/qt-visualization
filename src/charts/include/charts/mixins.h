#ifndef QT_VISUALIZATION_MIXINS_H
#define QT_VISUALIZATION_MIXINS_H

#include <functional>
#include <QTimer>

class FPSMixin {
private:
    typedef std::function<void(qreal)> FPSCallback;
    QTimer _timer;
    FPSCallback _fpsCallback = nullptr;
    quint64 _numberOfCalls = 0;

    void _calcFPS();

protected:
    void _frameCall();

public:
    explicit FPSMixin(QObject *parent);

    void setFPSTimeout(quint64 timeoutMs);

    void setFPSCallback(FPSCallback callback);
};


#endif //QT_VISUALIZATION_MIXINS_H

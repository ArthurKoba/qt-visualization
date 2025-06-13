#include "mixins.h"

void FPSMixin::_calcFPS() {
    if (_fpsCallback) {
        _fpsCallback(qreal(_numberOfCalls) / (1000.0 / _timer.interval()));
    }
    _numberOfCalls = 0;
}

void FPSMixin::_frameCall() {
    _numberOfCalls++;
}

FPSMixin::FPSMixin(QObject *parent) {
    QObject::connect(&_timer, &QTimer::timeout, parent, [this] () {
        _calcFPS();
    });
    _timer.setInterval(1000);
}

void FPSMixin::setFPSTimeout(quint64 timeoutMs) {
    _timer.setInterval(int(timeoutMs));
}

void FPSMixin::setFPSCallback(FPSMixin::FPSCallback callback) {
    _fpsCallback = std::move(callback);
    if (not _fpsCallback) {
        _timer.stop();
    } else {
        _timer.start();
        _numberOfCalls = 0;
    }
}

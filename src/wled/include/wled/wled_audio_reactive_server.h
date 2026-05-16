#ifndef AGGREGATOR_SERVER_H
#define AGGREGATOR_SERVER_H

#include <QElapsedTimer>
#include <qloggingcategory.h>
#include <QHostAddress>
#include <QUdpSocket>

#include "core/abstract/abs_task.h"
#include "abstract/types.h"

Q_DECLARE_LOGGING_CATEGORY(wled_audio_reactive_server);

class WLEDAudioReactiveServer final: public AbstractTask {
    std::vector<float> _data;
    QHostAddress _interface_address;
    QHostAddress _broadcast_address;
    quint16 _broadcast_port;
    QUdpSocket *_udp_socket = nullptr;
    uint8_t _frame_counter = 0;
    QElapsedTimer timer;
    bool _need_update = false;
public:
    explicit WLEDAudioReactiveServer(const QHostAddress &broadcast_address, quint16 broadcast_port);
    ~WLEDAudioReactiveServer() override;

    void update(const std::vector<float>& spectrum);

protected:
    uint64_t _task() override;
    void _handler();
};


#endif // AGGREGATOR_SERVER_H

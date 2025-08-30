#ifndef SERIAL_WORKER_H
#define SERIAL_WORKER_H

#include <QObject>
#include <QSerialPort>
#include <QTimer>
#include <QMutex>
#include <QQueue>
#include <QByteArray>

class SerialWorker : public QObject
{
    Q_OBJECT

public:
    explicit SerialWorker(QObject *parent = nullptr);
    ~SerialWorker();

    void setSerialPort(QSerialPort *port);
    void setUpdateInterval(int interval);
    void startReading();
    void stopReading();

signals:
    void dataReceived(const QByteArray &data);
    void errorOccurred(const QString &error);

private slots:
    void readData();
    void handleError(QSerialPort::SerialPortError error);

private:
    QSerialPort *m_serialPort;
    QTimer *m_readTimer;
    QMutex *m_portMutex;
    int m_updateInterval;
    bool m_isReading;
};

#endif // SERIAL_WORKER_H
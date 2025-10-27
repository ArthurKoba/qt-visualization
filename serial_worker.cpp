#include "serial_worker.h"
#include <QSerialPort>
#include <QTimer>
#include <QMutex>
#include <QDebug>

SerialWorker::SerialWorker(QObject *parent)
    : QObject(parent)
    , m_serialPort(nullptr)
    , m_readTimer(new QTimer(this))
    , m_portMutex(new QMutex())
    , m_updateInterval(10)
    , m_isReading(false)
{
    // Настройка таймера для чтения данных с минимальными задержками
    m_readTimer->setTimerType(Qt::PreciseTimer);
    
    // Подключение сигналов
    connect(m_readTimer, &QTimer::timeout, this, &SerialWorker::readData);
}

SerialWorker::~SerialWorker()
{
    stopReading();
    delete m_portMutex;
}

void SerialWorker::setSerialPort(QSerialPort *port)
{
    QMutexLocker locker(m_portMutex);
    m_serialPort = port;
}

void SerialWorker::setUpdateInterval(int interval)
{
    m_updateInterval = qMax(1, interval); // Минимальный интервал 1 мс
    if (m_isReading) {
        m_readTimer->setInterval(m_updateInterval);
    }
}

void SerialWorker::startReading()
{
    if (m_isReading || !m_serialPort) {
        return;
    }
    
    QMutexLocker locker(m_portMutex);
    
    if (m_serialPort && m_serialPort->isOpen()) {
        m_isReading = true;
        m_readTimer->start(m_updateInterval);
        
        // Также подключаемся к сигналу readyRead для мгновенного чтения
        connect(m_serialPort, &QSerialPort::readyRead, this, &SerialWorker::readData, Qt::DirectConnection);
        
        qDebug() << "Чтение данных запущено с интервалом" << m_updateInterval << "мс";
    }
}

void SerialWorker::stopReading()
{
    if (!m_isReading) {
        return;
    }
    
    QMutexLocker locker(m_portMutex);
    
    m_isReading = false;
    m_readTimer->stop();
    
    if (m_serialPort) {
        disconnect(m_serialPort, &QSerialPort::readyRead, this, &SerialWorker::readData);
    }
    
    qDebug() << "Чтение данных остановлено";
}

void SerialWorker::readData()
{
    if (!m_isReading || !m_serialPort) {
        return;
    }
    
    QMutexLocker locker(m_portMutex);
    
    if (!m_serialPort || !m_serialPort->isOpen()) {
        return;
    }
    
    // Читаем все доступные данные
    while (m_serialPort->bytesAvailable() > 0) {
        QByteArray data = m_serialPort->readAll();
        
        if (!data.isEmpty()) {
            // Отправляем данные в главный поток
            emit dataReceived(data);
        }
    }
}

void SerialWorker::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::NoError) {
        return;
    }
    
    QString errorString = "Неизвестная ошибка";
    if (m_serialPort) {
        errorString = m_serialPort->errorString();
    }
    
    emit errorOccurred(errorString);
}
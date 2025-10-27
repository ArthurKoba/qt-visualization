#ifndef SERIAL_VIEWER_H
#define SERIAL_VIEWER_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include <QThread>
#include <QMutex>
#include <QQueue>
#include <QByteArray>

QT_BEGIN_NAMESPACE
class QComboBox;
class QPushButton;
class QTextEdit;
class QLabel;
class QSpinBox;
QT_END_NAMESPACE

class SerialWorker;

class SerialViewer : public QMainWindow
{
    Q_OBJECT

public:
    SerialViewer(QWidget *parent = nullptr);
    ~SerialViewer();

private slots:
    void refreshPorts();
    void connectToPort();
    void disconnectFromPort();
    void sendData();
    void onDataReceived(const QByteArray &data);
    void onErrorOccurred(QSerialPort::SerialPortError error);

private:
    void setupUI();
    void updatePortList();
    void logMessage(const QString &message);

    QComboBox *m_portComboBox;
    QComboBox *m_baudRateComboBox;
    QComboBox *m_dataBitsComboBox;
    QComboBox *m_parityComboBox;
    QComboBox *m_stopBitsComboBox;
    QComboBox *m_flowControlComboBox;
    
    QPushButton *m_connectButton;
    QPushButton *m_disconnectButton;
    QPushButton *m_refreshButton;
    QPushButton *m_sendButton;
    QPushButton *m_clearButton;
    
    QTextEdit *m_receivedDataTextEdit;
    QTextEdit *m_sendDataTextEdit;
    QLabel *m_statusLabel;
    QSpinBox *m_updateIntervalSpinBox;
    
    QSerialPort *m_serialPort;
    SerialWorker *m_serialWorker;
    QThread *m_workerThread;
    
    bool m_isConnected;
    QTimer *m_refreshTimer;
};

#endif // SERIAL_VIEWER_H
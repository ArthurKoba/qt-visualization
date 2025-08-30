#include "serial_viewer.h"
#include "serial_worker.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QComboBox>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <QSpinBox>
#include <QMessageBox>
#include <QApplication>
#include <QStyle>
#include <QTimer>
#include <QThread>
#include <QSerialPortInfo>
#include <QScrollBar>
#include <QFont>
#include <QFontMetrics>
#include <QTime>

SerialViewer::SerialViewer(QWidget *parent)
    : QMainWindow(parent)
    , m_serialPort(nullptr)
    , m_serialWorker(nullptr)
    , m_workerThread(nullptr)
    , m_isConnected(false)
    , m_refreshTimer(new QTimer(this))
{
    setWindowTitle("COM Serial Viewer - Минимальные задержки");
    setMinimumSize(800, 600);
    resize(1000, 700);
    
    setupUI();
    
    // Инициализация COM порта
    m_serialPort = new QSerialPort(this);
    
    // Создание рабочего потока для чтения данных
    m_workerThread = new QThread(this);
    m_serialWorker = new SerialWorker();
    m_serialWorker->moveToThread(m_workerThread);
    
    // Подключение сигналов
    connect(m_serialPort, &QSerialPort::errorOccurred, this, &SerialViewer::onErrorOccurred);
    connect(m_serialWorker, &SerialWorker::dataReceived, this, &SerialViewer::onDataReceived);
    connect(m_workerThread, &QThread::started, m_serialWorker, &SerialWorker::startReading);
    connect(m_workerThread, &QThread::finished, m_serialWorker, &SerialWorker::stopReading);
    
    // Запуск потока
    m_workerThread->start();
    
    // Таймер для обновления списка портов
    m_refreshTimer->setInterval(2000); // Обновляем каждые 2 секунды
    connect(m_refreshTimer, &QTimer::timeout, this, &SerialViewer::refreshPorts);
    m_refreshTimer->start();
    
    // Первоначальное обновление списка портов
    refreshPorts();
    
    logMessage("Приложение запущено. Выберите COM порт для подключения.");
}

SerialViewer::~SerialViewer()
{
    if (m_isConnected) {
        disconnectFromPort();
    }
    
    if (m_workerThread) {
        m_workerThread->quit();
        m_workerThread->wait();
    }
}

void SerialViewer::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    
    // Группа настроек порта
    QGroupBox *portGroup = new QGroupBox("Настройки COM порта");
    QGridLayout *portLayout = new QGridLayout(portGroup);
    
    // COM порт
    portLayout->addWidget(new QLabel("Порт:"), 0, 0);
    m_portComboBox = new QComboBox();
    m_portComboBox->setMinimumWidth(120);
    portLayout->addWidget(m_portComboBox, 0, 1);
    
    // Скорость
    portLayout->addWidget(new QLabel("Скорость:"), 0, 2);
    m_baudRateComboBox = new QComboBox();
    m_baudRateComboBox->addItems({"9600", "19200", "38400", "57600", "115200", "230400", "460800", "921600"});
    m_baudRateComboBox->setCurrentText("115200");
    portLayout->addWidget(m_baudRateComboBox, 0, 3);
    
    // Биты данных
    portLayout->addWidget(new QLabel("Биты данных:"), 1, 0);
    m_dataBitsComboBox = new QComboBox();
    m_dataBitsComboBox->addItems({"5", "6", "7", "8"});
    m_dataBitsComboBox->setCurrentText("8");
    portLayout->addWidget(m_dataBitsComboBox, 1, 1);
    
    // Четность
    portLayout->addWidget(new QLabel("Четность:"), 1, 2);
    m_parityComboBox = new QComboBox();
    m_parityComboBox->addItems({"None", "Even", "Odd", "Space", "Mark"});
    portLayout->addWidget(m_parityComboBox, 1, 3);
    
    // Стоп-биты
    portLayout->addWidget(new QLabel("Стоп-биты:"), 2, 0);
    m_stopBitsComboBox = new QComboBox();
    m_stopBitsComboBox->addItems({"1", "1.5", "2"});
    portLayout->addWidget(m_stopBitsComboBox, 2, 1);
    
    // Управление потоком
    portLayout->addWidget(new QLabel("Управление потоком:"), 2, 2);
    m_flowControlComboBox = new QComboBox();
    m_flowControlComboBox->addItems({"None", "Hardware", "Software"});
    portLayout->addWidget(m_flowControlComboBox, 2, 3);
    
    // Интервал обновления
    portLayout->addWidget(new QLabel("Интервал обновления (мс):"), 3, 0);
    m_updateIntervalSpinBox = new QSpinBox();
    m_updateIntervalSpinBox->setRange(1, 1000);
    m_updateIntervalSpinBox->setValue(10); // Минимальная задержка
    m_updateIntervalSpinBox->setSuffix(" мс");
    portLayout->addWidget(m_updateIntervalSpinBox, 3, 1);
    
    mainLayout->addWidget(portGroup);
    
    // Кнопки управления
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    m_refreshButton = new QPushButton("Обновить порты");
    m_refreshButton->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
    buttonLayout->addWidget(m_refreshButton);
    
    m_connectButton = new QPushButton("Подключиться");
    m_connectButton->setIcon(style()->standardIcon(QStyle::SP_DialogOkButton));
    m_connectButton->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; border: none; padding: 8px; border-radius: 4px; }");
    buttonLayout->addWidget(m_connectButton);
    
    m_disconnectButton = new QPushButton("Отключиться");
    m_disconnectButton->setIcon(style()->standardIcon(QStyle::SP_DialogCancelButton));
    m_disconnectButton->setStyleSheet("QPushButton { background-color: #f44336; color: white; border: none; padding: 8px; border-radius: 4px; }");
    m_disconnectButton->setEnabled(false);
    buttonLayout->addWidget(m_disconnectButton);
    
    buttonLayout->addStretch();
    
    mainLayout->addLayout(buttonLayout);
    
    // Группа для отображения данных
    QGroupBox *dataGroup = new QGroupBox("Полученные данные");
    QVBoxLayout *dataLayout = new QVBoxLayout(dataGroup);
    
    m_receivedDataTextEdit = new QTextEdit();
    m_receivedDataTextEdit->setReadOnly(true);
    m_receivedDataTextEdit->setFont(QFont("Consolas", 10));
    m_receivedDataTextEdit->setMaximumHeight(300);
    dataLayout->addWidget(m_receivedDataTextEdit);
    
    QHBoxLayout *dataButtonLayout = new QHBoxLayout();
    m_clearButton = new QPushButton("Очистить");
    m_clearButton->setIcon(style()->standardIcon(QStyle::SP_DialogResetButton));
    dataButtonLayout->addWidget(m_clearButton);
    dataButtonLayout->addStretch();
    dataLayout->addLayout(dataButtonLayout);
    
    mainLayout->addWidget(dataGroup);
    
    // Группа для отправки данных
    QGroupBox *sendGroup = new QGroupBox("Отправка данных");
    QVBoxLayout *sendLayout = new QVBoxLayout(sendGroup);
    
    m_sendDataTextEdit = new QTextEdit();
    m_sendDataTextEdit->setMaximumHeight(100);
    m_sendDataTextEdit->setPlaceholderText("Введите данные для отправки...");
    sendLayout->addWidget(m_sendDataTextEdit);
    
    QHBoxLayout *sendButtonLayout = new QHBoxLayout();
    m_sendButton = new QPushButton("Отправить");
    m_sendButton->setIcon(style()->standardIcon(QStyle::SP_MessageBoxInformation));
    m_sendButton->setStyleSheet("QPushButton { background-color: #2196F3; color: white; border: none; padding: 8px; border-radius: 4px; }");
    sendButtonLayout->addWidget(m_sendButton);
    sendButtonLayout->addStretch();
    sendLayout->addLayout(sendButtonLayout);
    
    mainLayout->addWidget(sendGroup);
    
    // Статус
    m_statusLabel = new QLabel("Готов к подключению");
    m_statusLabel->setStyleSheet("QLabel { padding: 8px; background-color: #f0f0f0; border: 1px solid #ccc; border-radius: 4px; }");
    mainLayout->addWidget(m_statusLabel);
    
    // Подключение сигналов
    connect(m_refreshButton, &QPushButton::clicked, this, &SerialViewer::refreshPorts);
    connect(m_connectButton, &QPushButton::clicked, this, &SerialViewer::connectToPort);
    connect(m_disconnectButton, &QPushButton::clicked, this, &SerialViewer::disconnectFromPort);
    connect(m_sendButton, &QPushButton::clicked, this, &SerialViewer::sendData);
    connect(m_clearButton, &QPushButton::clicked, m_receivedDataTextEdit, &QTextEdit::clear);
    
    // Настройка стилей
    setStyleSheet(R"(
        QGroupBox {
            font-weight: bold;
            border: 2px solid #cccccc;
            border-radius: 5px;
            margin-top: 1ex;
            padding-top: 10px;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 5px 0 5px;
        }
        QTextEdit {
            border: 1px solid #cccccc;
            border-radius: 4px;
            padding: 5px;
        }
        QComboBox {
            border: 1px solid #cccccc;
            border-radius: 4px;
            padding: 5px;
        }
    )");
}

void SerialViewer::refreshPorts()
{
    m_portComboBox->clear();
    
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos) {
        QString portName = info.portName();
        QString description = info.description();
        QString manufacturer = info.manufacturer();
        
        QString displayText = portName;
        if (!description.isEmpty()) {
            displayText += " (" + description + ")";
        }
        
        m_portComboBox->addItem(displayText, portName);
    }
    
    if (m_portComboBox->count() == 0) {
        m_portComboBox->addItem("Нет доступных портов");
        m_connectButton->setEnabled(false);
    } else {
        m_connectButton->setEnabled(!m_isConnected);
    }
}

void SerialViewer::connectToPort()
{
    if (m_isConnected) {
        return;
    }
    
    if (m_portComboBox->count() == 0) {
        QMessageBox::warning(this, "Ошибка", "Нет доступных COM портов");
        return;
    }
    
    QString portName = m_portComboBox->currentData().toString();
    if (portName.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Выберите COM порт");
        return;
    }
    
    // Настройка параметров порта
    m_serialPort->setPortName(portName);
    m_serialPort->setBaudRate(m_baudRateComboBox->currentText().toInt());
    m_serialPort->setDataBits(static_cast<QSerialPort::DataBits>(m_dataBitsComboBox->currentText().toInt()));
    
    QString parity = m_parityComboBox->currentText();
    if (parity == "None") m_serialPort->setParity(QSerialPort::NoParity);
    else if (parity == "Even") m_serialPort->setParity(QSerialPort::EvenParity);
    else if (parity == "Odd") m_serialPort->setParity(QSerialPort::OddParity);
    else if (parity == "Space") m_serialPort->setParity(QSerialPort::SpaceParity);
    else if (parity == "Mark") m_serialPort->setParity(QSerialPort::MarkParity);
    
    QString stopBits = m_stopBitsComboBox->currentText();
    if (stopBits == "1") m_serialPort->setStopBits(QSerialPort::OneStop);
    else if (stopBits == "1.5") m_serialPort->setStopBits(QSerialPort::OneAndHalfStop);
    else if (stopBits == "2") m_serialPort->setStopBits(QSerialPort::TwoStop);
    
    QString flowControl = m_flowControlComboBox->currentText();
    if (flowControl == "None") m_serialPort->setFlowControl(QSerialPort::NoFlowControl);
    else if (flowControl == "Hardware") m_serialPort->setFlowControl(QSerialPort::HardwareControl);
    else if (flowControl == "Software") m_serialPort->setFlowControl(QSerialPort::SoftwareControl);
    
    // Открытие порта
    if (m_serialPort->open(QIODevice::ReadWrite)) {
        m_isConnected = true;
        
        // Настройка рабочего потока
        m_serialWorker->setSerialPort(m_serialPort);
        m_serialWorker->setUpdateInterval(m_updateIntervalSpinBox->value());
        
        // Обновление UI
        m_connectButton->setEnabled(false);
        m_disconnectButton->setEnabled(true);
        m_portComboBox->setEnabled(false);
        m_baudRateComboBox->setEnabled(false);
        m_dataBitsComboBox->setEnabled(false);
        m_parityComboBox->setEnabled(false);
        m_stopBitsComboBox->setEnabled(false);
        m_flowControlComboBox->setEnabled(false);
        m_updateIntervalSpinBox->setEnabled(false);
        
        m_statusLabel->setText(QString("Подключено к %1").arg(portName));
        m_statusLabel->setStyleSheet("QLabel { padding: 8px; background-color: #4CAF50; color: white; border: 1px solid #45a049; border-radius: 4px; }");
        
        logMessage(QString("Подключение к %1 установлено").arg(portName));
        
        // Запуск чтения данных
        m_serialWorker->startReading();
    } else {
        QMessageBox::critical(this, "Ошибка", 
            QString("Не удалось открыть порт %1: %2").arg(portName, m_serialPort->errorString()));
    }
}

void SerialViewer::disconnectFromPort()
{
    if (!m_isConnected) {
        return;
    }
    
    // Остановка чтения данных
    m_serialWorker->stopReading();
    
    // Закрытие порта
    m_serialPort->close();
    m_isConnected = false;
    
    // Обновление UI
    m_connectButton->setEnabled(true);
    m_disconnectButton->setEnabled(false);
    m_portComboBox->setEnabled(true);
    m_baudRateComboBox->setEnabled(true);
    m_dataBitsComboBox->setEnabled(true);
    m_parityComboBox->setEnabled(true);
    m_stopBitsComboBox->setEnabled(true);
    m_flowControlComboBox->setEnabled(true);
    m_updateIntervalSpinBox->setEnabled(true);
    
    m_statusLabel->setText("Отключено");
    m_statusLabel->setStyleSheet("QLabel { padding: 8px; background-color: #f0f0f0; border: 1px solid #ccc; border-radius: 4px; }");
    
    logMessage("Соединение разорвано");
}

void SerialViewer::sendData()
{
    if (!m_isConnected) {
        QMessageBox::warning(this, "Ошибка", "Сначала подключитесь к COM порту");
        return;
    }
    
    QString data = m_sendDataTextEdit->toPlainText();
    if (data.isEmpty()) {
        return;
    }
    
    QByteArray bytes = data.toUtf8();
    qint64 written = m_serialPort->write(bytes);
    
    if (written == bytes.size()) {
        logMessage(QString("Отправлено: %1").arg(data));
        m_sendDataTextEdit->clear();
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось отправить все данные");
    }
}

void SerialViewer::onDataReceived(const QByteArray &data)
{
    if (data.isEmpty()) {
        return;
    }
    
    // Отображение данных в hex и текстовом формате
    QString hexData = data.toHex(' ').toUpper();
    QString textData = QString::fromUtf8(data);
    
    // Замена непечатаемых символов
    for (int i = 0; i < textData.length(); ++i) {
        if (!textData[i].isPrint() && textData[i] != '\n' && textData[i] != '\r' && textData[i] != '\t') {
            textData[i] = '.';
        }
    }
    
    QString displayText = QString("[%1] %2 | %3")
        .arg(QTime::currentTime().toString("hh:mm:ss.zzz"))
        .arg(hexData)
        .arg(textData);
    
    // Добавление в текстовое поле
    m_receivedDataTextEdit->append(displayText);
    
    // Автопрокрутка к последней строке
    QScrollBar *scrollBar = m_receivedDataTextEdit->verticalScrollBar();
    scrollBar->setValue(scrollBar->maximum());
}

void SerialViewer::onErrorOccurred(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::NoError) {
        return;
    }
    
    QString errorString = m_serialPort->errorString();
    logMessage(QString("Ошибка порта: %1").arg(errorString));
    
    if (error == QSerialPort::ResourceError) {
        // Порт был отключен
        disconnectFromPort();
        QMessageBox::warning(this, "Ошибка", "COM порт был отключен");
    }
}

void SerialViewer::logMessage(const QString &message)
{
    QString timestamp = QTime::currentTime().toString("hh:mm:ss.zzz");
    QString logEntry = QString("[%1] %2").arg(timestamp, message);
    
    m_receivedDataTextEdit->append(logEntry);
    
    // Автопрокрутка
    QScrollBar *scrollBar = m_receivedDataTextEdit->verticalScrollBar();
    scrollBar->setValue(scrollBar->maximum());
}
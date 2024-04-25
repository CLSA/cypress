#include "audiometer_manager.h"
#include "data/hearing/tests/hearing_test.h"

#include <QDateTime>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QSerialPortInfo>
#include <QSettings>
#include <QStandardItemModel>
#include <QtMath>
#include <QMessageBox>



AudiometerManager::AudiometerManager(QSharedPointer<AudiometerSession> session)
    : SerialPortManager(session)
{
    m_portName = CypressSettings::readSetting("audiometer/portName").toString();

    qInfo() << "AudiometerManager";
    qInfo() << "portName" << m_portName;

    m_test.reset(new HearingTest);
    //m_test->setExpectedMeasurementCount(16);
}

QByteArray AudiometerManager::END_CODE = AudiometerManager::initEndCode();

QByteArray AudiometerManager::initEndCode()
{
    static const char data[] = { '~', 'p', '\x17', 'Z', '^', QChar(QChar::SpecialCharacter::CarriageReturn).toLatin1() };

    return QByteArray(data);
}

bool AudiometerManager::hasEndCode(const QByteArray &arr) const
{
    // If the
    bool ok = false;

    // arr.size() = 10
    // len = 9 (0-9)
    int len = arr.size() - 1;

    if (5 < len) {
        ok = (END_CODE.at(5) == arr.at(len) && END_CODE.at(2) == arr.at(len - 3)
              && END_CODE.at(1) == arr.at(len - 4) && END_CODE.at(0) == arr.at(len - 5));
    }
    return ok;
}

bool AudiometerManager::isRS232Port(const QSerialPortInfo& portInfo)
{
    // Check if the system location starts with "COM"
    return portInfo.description().contains("communications port", Qt::CaseInsensitive);
}

bool AudiometerManager::isInstalled()
{
    const QList<QSerialPortInfo> portList = QSerialPortInfo::availablePorts();
    if (portList.isEmpty())
        return false;

    foreach (const QSerialPortInfo& portInfo, portList) {
        if (!isRS232Port(portInfo))
            continue;

        const QList<qint32> baudRates = portInfo.standardBaudRates();
        foreach (const qint32 baudRate, baudRates) {
            if (baudRate >= 600 && baudRate <= 19200)
                return true;
        }
    }

    return false;
}

void AudiometerManager::addManualEntry(const QString side, const QString test, const int level, const bool pass) {
    auto audiometerTest = qSharedPointerCast<HearingTest>(m_test);
    QSharedPointer<HearingMeasurement> measure(new HearingMeasurement(side, test, level, pass));
    if (measure->isValid()) {
        audiometerTest->addMeasurement(measure);
        emit dataChanged(audiometerTest);
        emit canFinish();
    }

    qDebug() << audiometerTest->toJsonObject();
}

bool AudiometerManager::start()
{
    qDebug() << "AudiometerManager::start";

    if (!setUp())
        return false;

    scanDevices();

    emit started(m_test);
    emit dataChanged(m_test);

    return true;
}

bool AudiometerManager::setUp()
{
    qInfo() << "AudiometerManager::setUp";
    return true;
}

void AudiometerManager::connectDevice()
{
    qInfo() << "AudiometerManager::connectDevice";

    // Connect to the serial port and set up listeners
    if (m_port.isOpen()) {
        disconnect(&m_port, &QSerialPort::readyRead,
            this,    &AudiometerManager::readDevice);

        disconnect(&m_port, &QSerialPort::errorOccurred,
            this,    &AudiometerManager::handleSerialPortError);

        disconnect(&m_port, &QSerialPort::dataTerminalReadyChanged,
            this,    &AudiometerManager::handleDataTerminalReadyChanged);

        disconnect(&m_port, &QSerialPort::requestToSendChanged,
            this,    &AudiometerManager::handleRequestToSendChanged);
    }

    m_port.setDataBits(QSerialPort::Data8);
    m_port.setParity(QSerialPort::NoParity);
    m_port.setStopBits(QSerialPort::OneStop);
    m_port.setBaudRate(QSerialPort::Baud9600);

    connect(&m_port, &QSerialPort::readyRead, this, &AudiometerManager::readDevice);

    connect(&m_port, &QSerialPort::errorOccurred, this, &AudiometerManager::handleSerialPortError);

    connect(&m_port,
            &QSerialPort::dataTerminalReadyChanged,
            this,
            &AudiometerManager::handleDataTerminalReadyChanged);

    connect(&m_port,
            &QSerialPort::requestToSendChanged,
            this,
            &AudiometerManager::handleRequestToSendChanged);

    if (m_port.open(QSerialPort::ReadWrite)) {
        emit deviceConnected(QSerialPortInfo(m_port));
        emit canMeasure();
    }
}

void AudiometerManager::selectDevice(const QSerialPortInfo &port)
{
    SerialPortManager::selectDevice(port);
    CypressSettings::writeSetting("audiometer/portName", port.portName());
    m_portName = port.portName();
}

void AudiometerManager::measure()
{
    qInfo() << "AudiometerManager::measure";

    if (m_sim)
    {
        m_test->simulate(QVariantMap({{"barcode", m_session->getBarcode()}}));

        emit dataChanged(m_test);
        emit canFinish();

        return;
    }

    static const char cmd[] = { 0x05, '4', 0x0d };

    m_request = QByteArray::fromRawData(cmd, 3);
    writeDevice();
}

bool AudiometerManager::cleanUp()
{
    qInfo() << "AudiometerManager::cleanUp";

    clearData();

    m_buffer.clear();
    m_deviceList.clear();

    return true;
}

void AudiometerManager::readDevice()
{
    qInfo() << "AudiometerManager::readDevice";

    // read received data whenever the data ready signal is emitted and add it to the buffer
    // if the end code is received, validate the data and signal that the test is complete
    //
    const QByteArray data = m_port.readAll();
    m_buffer += data;

    auto hearingTest = qSharedPointerCast<HearingTest>(m_test);
    if(hasEndCode(m_buffer))
    {
        clearData();
        hearingTest->fromArray(m_buffer);
        if(hearingTest->isValid())
        {
            emit dataChanged(m_test);
            emit canFinish();
        }
        m_buffer.clear();
    }
}

void AudiometerManager::writeDevice()
{
    qInfo() << "AudiometerManager::writeDevice";

    // send a request to the audiometer to start data collection
    //
    m_buffer.clear();
    const int bytesWritten = m_port.write(m_request);
    if (bytesWritten < 0) {
        QMessageBox::critical(nullptr, "Error", "Could not write data to the serial port");
    }
}

bool AudiometerManager::clearData()
{
    qInfo() << "AudiometerManager::clearData";

    m_test->reset();

    emit dataChanged(m_test);

    return true;
}





#include "audiometer_manager.h"
#include "../../data/hearing/tests/hearing_test.h"

#include "cypress_settings.h"

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

QByteArray AudiometerManager::END_CODE = AudiometerManager::initEndCode();

QByteArray AudiometerManager::initEndCode()
{
    const char data[] = { '~', 'p', '\x17', 'Z', '^', QChar(QChar::SpecialCharacter::CarriageReturn).toLatin1() };

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

AudiometerManager::AudiometerManager(QSharedPointer<AudiometerSession> session)
    : SerialPortManager(session)
{
    m_test.reset(new HearingTest);
    m_test->setExpectedMeasurementCount(16);
}

bool AudiometerManager::isRS232Port(const QSerialPortInfo& portInfo)
{
    // Check if the system location starts with "COM"
    return portInfo.description().contains("communications port", Qt::CaseInsensitive);
}

bool AudiometerManager::isInstalled()
{
    QList<QSerialPortInfo> portList = QSerialPortInfo::availablePorts();
    if (portList.isEmpty()) {
        return false;
    }

    bool supportsBaudRates = false;
    foreach (const QSerialPortInfo& portInfo, portList) {
        if (!isRS232Port(portInfo))
            continue;

        QList<qint32> baudRates = portInfo.standardBaudRates();
        foreach (qint32 baudRate, baudRates) {
            if (baudRate >= 600 && baudRate <= 19200) {
                qInfo() << "port:"                 << portInfo.portName();
                qInfo() << "supported baud rates:" << baudRates;
                qInfo() << "description:"          << portInfo.description();
                qInfo() << "manufacturer:"         << portInfo.manufacturer();
                qInfo() << "serial number:"        << portInfo.serialNumber();
                qInfo() << "system location:"      << portInfo.systemLocation();
                qInfo() << "vendor identifier:"    << portInfo.vendorIdentifier();
                qInfo() << "product identifier:"   << portInfo.productIdentifier();
                qInfo() << "-------------------------------------";

                supportsBaudRates = true;
            }
        }
    }

    return supportsBaudRates;
}

void AudiometerManager::start()
{
    if (m_debug)
    {
        qDebug() << "AudiometerManager::start";
    }

    emit started(m_test.get());
    emit canMeasure();
}

bool AudiometerManager::setUp()
{
    if (m_debug)
    {
        qDebug() << "AudiometerManager::setUp";
    }

    return true;
}

void AudiometerManager::measure()
{
    if (m_debug)
    {
        qDebug() << "AudiometerManager::measure";
    }

    m_test.reset();

    if (CypressSettings::isSimMode())
    {
        m_test->simulate(QVariantMap({{"barcode", m_session->getBarcode()}}));

        emit measured(m_test.get());
        emit canFinish();
    }

    const char cmd[] = { 0x05, '4', 0x0d };
    m_request = QByteArray::fromRawData(cmd, 3);
    writeDevice();
}

void AudiometerManager::finish()
{
    if (m_debug)
    {
        qDebug() << "AudiometerManager::finish";
    }

    int answer_id = m_session->getAnswerId();

    QJsonObject testJson = m_test->toJsonObject();
    QJsonObject sessionObj = m_session->getJsonObject();

    testJson.insert("session", sessionObj);

    QJsonObject responseJson {};
    responseJson.insert("value", testJson);

    QJsonDocument jsonDoc(responseJson);
    QByteArray serializedData = jsonDoc.toJson();

    QString answerUrl = CypressSettings::getAnswerUrl(answer_id);
    sendHTTPSRequest("PATCH", answerUrl, "application/json", serializedData);

    emit success("");

    cleanUp();
}

bool AudiometerManager::cleanUp()
{
    if (m_debug)
    {
        qDebug() << "AudiometerManager::cleanUp";
    }

    clearData();

    m_buffer.clear();
    m_deviceList.clear();
    //m_deviceData.reset();

    return true;
}

void AudiometerManager::readDevice()
{
    if (m_debug)
    {
        qDebug() << "AudiometerManager::readDevice";
    }

    // read received data whenever the data ready signal is emitted and add it to the buffer
    // if the end code is received, validate the data and signal that the test is complete

    QByteArray data = m_port.readAll();

    m_buffer += data;

    HearingTest* hearingTest = static_cast<HearingTest*>(m_test.get());

    if(hasEndCode(m_buffer))
    {
        hearingTest->fromArray(m_buffer);
        if(hearingTest->isValid())
        {
            emit measured(m_test.get());
            emit canFinish();
        }
    }
}

void AudiometerManager::writeDevice()
{
    if (m_debug)
    {
        qDebug() << "AudiometerManager::writeDevice";
    }

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
    if (m_debug)
    {
        qDebug() << "AudiometerManager::clearData";
    }

    m_test.reset();

    return true;
}





#include <QDateTime>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QSerialPortInfo>
#include <QSettings>
#include <QStandardItemModel>
#include <QtMath>

#include "CypressApplication.h"
#include "auxiliary/JsonSettings.h"

#include "AudiometerManager.h"

QByteArray AudiometerManager::END_CODE = AudiometerManager::initEndCode();

QByteArray AudiometerManager::initEndCode()
{
    const char data[] = { '~', 'p', '\x17', 'Z', '^', QChar(QChar::SpecialCharacter::CarriageReturn).toLatin1() };

    return QByteArray(data);
}

bool AudiometerManager::hasEndCode(const QByteArray &arr)
{
    bool ok = false;
    int len = arr.size() - 1;
    if(5 < len)
    {
      ok = (
        END_CODE.at(5) == arr.at(len) &&
        END_CODE.at(2) == arr.at(len-3) &&
        END_CODE.at(1) == arr.at(len-4) &&
        END_CODE.at(0) == arr.at(len-5)
        );
    }
    return ok;
}

AudiometerManager::AudiometerManager()
{
    m_test.setExpectedMeasurementCount(16);
}

bool AudiometerManager::isInstalled()
{
    //bool found = scanDevices();
    //return found;
    return false;
}

bool AudiometerManager::setUp()
{
    return true;
}

void AudiometerManager::setInputData(const QVariantMap& inputData)
{
    if (inputData.value("participant_id").isNull())
    {
        qDebug() << "no participant id";
    }
    m_test.addMetaData("participant_id", inputData.value("participant_id"));
}

void AudiometerManager::measure()
{
    clearData();
    const char cmd[] = { 0x05, '4', 0x0d };
    m_request = QByteArray::fromRawData(cmd, 3);
    writeDevice();
}

void AudiometerManager::finish()
{
    if(m_port.isOpen())
        m_port.close();

    QJsonObject results;
    if (CypressApplication::mode == Mode::Sim)
    {
        results = JsonSettings::readJsonFromFile(
            QCoreApplication::applicationDirPath() + "/src/tests/fixtures/audiometer/output.json"
        );
    }
    else
    {
        results = m_test.toJsonObject();
    }

    if (results.empty())
    {
        qCritical() << "AudiometerManager::finish - error: test results are empty";
        return;
    }

    bool ok = sendResultsToPine(results);
    if (!ok)
    {
        qCritical() << "AudiometerManager::finish - error: unable to send results";
    }

    CypressApplication::status = Status::Waiting;

    ok = cleanUp();
    if (!ok)
    {
        qCritical() << "AudiometerManager::finish - error: device cleanup failed";
    }
}

bool AudiometerManager::cleanUp()
{
    clearData();
    return true;
}

void AudiometerManager::readDevice()
{
    // read received data whenever the data ready signal is emitted and add it to the buffer
    // if the end code is received, validate the data and signal that the test is complete

    QByteArray data = m_port.readAll();

    m_buffer += data;

    if(hasEndCode(m_buffer))
    {
        m_test.fromArray(m_buffer);
        if(m_test.isValid())
        {
            // emit the can write signal
            qDebug() << "Audiometer::readDevice - test is valid";
            emit canWrite();
        }
    }
}

void AudiometerManager::writeDevice()
{
    // send a request to the audiometer to start data collection
    //
    m_buffer.clear();
    const int bytesWritten = m_port.write(m_request);
    if (bytesWritten < 0) {
        qCritical() << "AudiometerManager::writeDevice - Error: could not write data to the serial port";
    }
}

bool AudiometerManager::clearData()
{
    // Reset the device and data
    m_buffer.clear();
    //m_deviceList.clear();
    //m_deviceData.reset();
    m_test.reset();
    return true;
}





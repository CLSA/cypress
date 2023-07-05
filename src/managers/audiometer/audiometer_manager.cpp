#include <QDateTime>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QSerialPortInfo>
#include <QSettings>
#include <QStandardItemModel>
#include <QtMath>

#include "cypress_application.h"
#include "auxiliary/json_settings.h"

#include "audiometer_manager.h"

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

AudiometerManager::AudiometerManager(const CypressSession& session)
{
    m_test.setExpectedMeasurementCount(16);
    m_inputData = jsonObjectToVariantMap(session.getInputData());
    qDebug() << "m_inputdata: " << m_inputData;
}

bool AudiometerManager::isRS232Port(const QSerialPortInfo& portInfo)
{
#ifdef Q_OS_WIN
    // Check if the system location starts with "COM"
    return portInfo.description().contains("communications port", Qt::CaseInsensitive);
#elif defined(Q_OS_LINUX)
    // Check if the system location matches the pattern "/dev/ttyS*"
    return portInfo.systemLocation().startsWith("/dev/ttyS", Qt::CaseSensitive);
#else
    // Unsupported platform
    return false;
#endif
}

bool AudiometerManager::isInstalled()
{
    QList<QSerialPortInfo> portList = QSerialPortInfo::availablePorts();
    if (portList.isEmpty()) {
      qInfo() << "no RS232 ports available.";
      qInfo() << "-------------------------------------";
      return false;
    } else {
      qInfo() << "checking for installed devices on available COM ports:";
      bool isInstalled { false };

      for (const QSerialPortInfo& portInfo : portList) {

        if (!isRS232Port(portInfo))
        {
            continue;
        }

        QSerialPort serialPort;
        serialPort.setPort(portInfo);
        if (serialPort.open(QIODevice::ReadWrite)) {
            qDebug() << "device installed on port" << portInfo.portName();
            serialPort.close();
            isInstalled = true;
        } else {
            qDebug() << "no device installed on port" << portInfo.portName();
        }
        qInfo() << "-------------------------------------";
      }
      return isInstalled;
    }
    return false;
}

bool AudiometerManager::isAvailable()
{
    QList<QSerialPortInfo> portList = QSerialPortInfo::availablePorts();
    if (portList.isEmpty()) {
      qWarning() << "audiometer: no RS232 ports available.";
      return false;
    } else {
      qInfo() << "audiometer: available RS232 ports with supported baud rates between 600 and 19200:";
      bool supportsBaudRates = false;
      for (const QSerialPortInfo& portInfo : portList) {
          if (!isRS232Port(portInfo))
          {
              continue;
          }

          QList<qint32> baudRates = portInfo.standardBaudRates();
          for (qint32 baudRate : baudRates) {
              if (baudRate >= 600 && baudRate <= 19200) {
                  supportsBaudRates = true;
                  break;
              }
          }

          if (supportsBaudRates) {
            qInfo() << "port:"                 << portInfo.portName();
            qInfo() << "supported baud rates:" << baudRates;
            qInfo() << "description:"          << portInfo.description();
            qInfo() << "manufacturer:"         << portInfo.manufacturer();
            qInfo() << "serial number:"        << portInfo.serialNumber();
            qInfo() << "system location:"      << portInfo.systemLocation();
            qInfo() << "vendor identifier:"    << portInfo.vendorIdentifier();
            qInfo() << "product identifier:"   << portInfo.productIdentifier();
            qInfo() << "-------------------------------------";
          }
      }

      return supportsBaudRates;
    }
    return false;
}

bool AudiometerManager::setUp()
{
    return true;
}

void AudiometerManager::setInputData(const QVariantMap& inputData)
{
    m_inputData = inputData;
    qDebug() << "INPUT DATA";
    //if (inputData.value("participant_id").isNull())
    //{
    //    qDebug() << "no participant id";
    //}
    //m_test.addMetaData("participant_id", inputData.value("participant_id"));
}

void AudiometerManager::measure()
{
    //clearData();
    //const char cmd[] = { 0x05, '4', 0x0d };
    //m_request = QByteArray::fromRawData(cmd, 3);
    //writeDevice();
    if (Cypress::getInstance().isSimulation())
    {
      sendResultsToPine("C:/dev/clsa/cypress/src/tests/fixtures/audiometer/output.json");
      return;
    }

    //if (results.empty()) return;

    //results["uuid"] = m_uuid;
    //results["answer_id"] = m_answerId;
    //results["barcode"] = m_barcode;
    //results["interviewer"] = m_interviewer;

    //bool ok = sendResultsToPine(results);
    //if (!ok)
    //{
    //    qDebug() << "Could not send results to Pine";
    //}

    //qDebug() << "measure called";
}

void AudiometerManager::finish()
{
    //if(m_port.isOpen())
    //    m_port.close();

    //QJsonObject results;
    //if (CypressApplication::getInstance().isSimulation())
    //{
    //    results = JsonSettings::readJsonFromFile(
    //        QCoreApplication::applicationDirPath() + "/src/tests/fixtures/audiometer/output.json"
    //    );
    //}
    //else
    //{
    //    results = m_test.toJsonObject();
    //}

    //if (results.empty())
    //{
    //    qCritical() << "AudiometerManager::finish - error: test results are empty";
    //    return;
    //}

    //bool ok = sendResultsToPine(results);
    //if (!ok)
    //{
    //    qCritical() << "AudiometerManager::finish - error: unable to send results";
    //}

    //ok = cleanUp();
    //if (!ok)
    //{
    //    qCritical() << "AudiometerManager::finish - error: device cleanup failed";
    //}
}

bool AudiometerManager::cleanUp()
{
    //clearData();
    return true;
}

void AudiometerManager::readDevice()
{
    // read received data whenever the data ready signal is emitted and add it to the buffer
    // if the end code is received, validate the data and signal that the test is complete

    //QByteArray data = m_port.readAll();

    //m_buffer += data;

    //if(hasEndCode(m_buffer))
    //{
    //    m_test.fromArray(m_buffer);
    //    if(m_test.isValid())
    //    {
    //        // emit the can write signal
    //        qDebug() << "Audiometer::readDevice - test is valid";
    //        emit canFinish();
    //    }
    //}
}

void AudiometerManager::writeDevice()
{
    // send a request to the audiometer to start data collection
    //
    //m_buffer.clear();
    //const int bytesWritten = m_port.write(m_request);
    //if (bytesWritten < 0) {
    //    qCritical() << "AudiometerManager::writeDevice - Error: could not write data to the serial port";
    //}
}

bool AudiometerManager::clearData()
{
    //// Reset the device and data
    //m_buffer.clear();
    ////m_deviceList.clear();
    ////m_deviceData.reset();
    //m_test.reset();
    return true;
}





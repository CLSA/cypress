#include "AudiometerManager.h"

#include <QDateTime>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QSerialPortInfo>
#include <QSettings>
#include <QStandardItemModel>
#include <QtMath>

QByteArray AudiometerManager::END_CODE = AudiometerManager::initEndCode();

QByteArray AudiometerManager::initEndCode()
{
    const char data[] =
      { '~','p','\x17','Z','^',
        QChar(QChar::SpecialCharacter::CarriageReturn).toLatin1() };
    return QByteArray(data);
}

AudiometerManager::AudiometerManager(QWidget *parent)
    : SerialPortManager(parent)
{
    m_test.setExpectedMeasurementCount(16);
}

void AudiometerManager::clearData()
{
    m_test.reset();
}

void AudiometerManager::finish()
{
    qDebug() << "slot finish";

    if(m_port.isOpen())
        m_port.close();

    QJsonObject results = m_test.toJsonObject();
    //m_deviceData.reset();
    //m_deviceList.clear();
    //m_test.reset();
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

void AudiometerManager::readDevice()
{
  QByteArray data = m_port.readAll();
  m_buffer += data;
  qInfo() << "read device received buffer " << QString(m_buffer);

    if(hasEndCode(m_buffer))
    {
        m_test.fromArray(m_buffer);
        if(m_test.isValid())
        {
            // emit the can write signal
            qDebug() << "signal canWrite";
            emit canWrite();
        }
    }
}

void AudiometerManager::measure()
{
    qDebug() << "slot measure";
    clearData();
    const char cmd[] = {0x05,'4',0x0d};
    m_request = QByteArray::fromRawData(cmd,3);
    writeDevice();
}

void AudiometerManager::writeDevice()
{
    // prepare to receive data
    //
    m_buffer.clear();
    qDebug() << "slot writeDevice";
    m_port.write(m_request);
}

#include "cypress_application.h"
#include "cypress_session.h"

#include "data/weigh_scale/tests/weigh_scale_test.h"
#include "data/weigh_scale/measurements/weight_measurement.h"

#include "weigh_scale_manager.h"

#include <QDateTime>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QRandomGenerator>
#include <QSerialPortInfo>
#include <QSettings>
#include <QStandardItemModel>
#include <QtMath>
#include <QJsonDocument>
#include <QMessageBox>

WeighScaleManager::WeighScaleManager(QSharedPointer<WeighScaleSession> &session)
    : SerialPortManager(session)
{
    m_test.reset(new WeighScaleTest);
    m_test->setExpectedMeasurementCount(2);
}

bool WeighScaleManager::isInstalled()
{
    return false;
}

void WeighScaleManager::start()
{
    if (m_debug)
    {
        qDebug() << "WeighScaleManager::measure";
    }

    scanDevices();

    emit started(m_test.get());
    emit canMeasure();
}

void WeighScaleManager::measure()
{
    if (m_debug)
    {
        qDebug() << "WeighScaleManager::measure";
    }

    if (m_sim)
    {
        m_test->simulate();

        emit measured(m_test.get());
        emit canFinish();

        return;
    }

    m_request = QByteArray("p");
    writeDevice();
}

void WeighScaleManager::finish()
{
    if (m_debug)
    {
        qDebug() << "WeighScaleManager::finish";
    }

    int answer_id = m_session->getAnswerId();

    QJsonObject testJson = m_test->toJsonObject();
    QJsonObject sessionObj = m_session->getJsonObject();
    testJson.insert("session", sessionObj);

    QJsonObject responseJson;
    responseJson.insert("value", testJson);

    QJsonDocument jsonDoc(responseJson);
    QByteArray serializedData = jsonDoc.toJson();

    QString answerUrl = CypressSettings::getAnswerUrl(answer_id);
    sendHTTPSRequest("PATCH", answerUrl, "application/json", serializedData);

    emit success("");
}

void WeighScaleManager::connectDevice()
{
    if (m_debug)
    {
        qDebug() << "WeighScaleManager::connectDevice";
    }

    // Connect to the serial port and set up listeners

    if(m_port.isOpen())
    {
        m_port.close();
    }

    m_port.setDataBits(QSerialPort::Data8);
    m_port.setParity(QSerialPort::NoParity);
    m_port.setStopBits(QSerialPort::OneStop);
    m_port.setBaudRate(QSerialPort::Baud9600);

    if (m_port.open(QSerialPort::ReadWrite))
    {
        qDebug() << "connected";
        emit deviceConnected();

        // try and read the scale ID, if we can do that then emit the
        // canMeasure signal
        // the canMeasure signal is emitted from readDevice slot on successful read
        //
        m_request = QByteArray("i");
        writeDevice();
    }
    else
    {
        qDebug() << "could not open port";
    }
}

void WeighScaleManager::zeroDevice()
{
    if (m_debug)
    {
        qDebug() << "WeighScaleManager::zeroDevice";
    }

    m_request = QByteArray("z");
    writeDevice();
}

void WeighScaleManager::readDevice()
{
    if (m_debug)
    {
        qDebug() << "WeighScaleManager::readDevice";
    }

    QByteArray data = m_port.readAll();
    m_buffer += data;

    qDebug() << "read device received buffer " << QString(m_buffer);

    if (m_buffer.isEmpty())
    {
        return;
    }

    if("i" == QString(m_request))
    {
        //m_deviceData.setAttribute("software_id", QString(m_buffer.simplified()));
        // signal the GUI that the measure button can be clicked
        //
        emit canMeasure();
    }
    else if("p" == QString(m_request))
    {
        WeighScaleTest* test = static_cast<WeighScaleTest*>(m_test.get());
        test->fromArray(m_buffer);

        qDebug() << "received p request, read buffer" << m_buffer;

        if(m_test->isValid())
        {
            qDebug() << "test is valid, can save results";
            // emit the can write signal
            emit measured(m_test.get());
            emit canFinish();
        }
        else
        {
            QMessageBox::critical(nullptr, "Received invalid data", "Data received was invalid");

            emit canMeasure();
        }
    }
    else if("z" == QString(m_request))
    {
        WeightMeasurement m;
        m.fromArray(m_buffer);
        if(m.isZero())
        {
            // signal the GUI that the measure button can be clicked
            //
            emit canMeasure();
        }
    }
}

void WeighScaleManager::writeDevice()
{
    if (m_debug)
    {
        qDebug() << "WeighScaleManager::writeDevice";
    }

    // prepare to receive data
    //
    m_buffer.clear();
    m_port.write(m_request);
}

bool WeighScaleManager::setUp()
{
    if (m_debug)
    {
        qDebug() << "WeighScaleManager::setUp";
    }

    return true;
}

bool WeighScaleManager::cleanUp()
{
    if (m_debug)
    {
        qDebug() << "WeighScaleManager::cleanUp";
    }

    return clearData();
}

bool WeighScaleManager::clearData()
{
    if (m_debug)
    {
        qDebug() << "WeighScaleManager::clearData";
    }

    m_test->reset();
    return true;
}



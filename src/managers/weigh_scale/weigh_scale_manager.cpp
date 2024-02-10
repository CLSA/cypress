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
    //m_test->setExpectedMeasurementCount(2);

    m_deviceName = CypressSettings::readSetting("weight_scale/portName").toString();

    if (m_debug) {
        qDebug() << "WeighScaleManager";

        qDebug() << session->getSessionId();
        qDebug() << session->getBarcode();
        qDebug() << session->getInterviewer();
        qDebug() << session->getInputData();
    }
}

bool WeighScaleManager::isInstalled()
{
    return true;
}

bool WeighScaleManager::start()
{
    if (m_debug)
        qDebug() << "WeighScaleManager::measure";

    scanDevices();

    emit started(m_test);
    emit dataChanged(m_test);
    emit canMeasure();

    return true;
}

void WeighScaleManager::measure()
{
    if (m_debug)
        qDebug() << "WeighScaleManager::measure";

    if (m_sim) {
        m_test->simulate();

        emit dataChanged(m_test);
        emit canFinish();

        return;
    }

    m_request = QByteArray("p");
    writeDevice();
}

void WeighScaleManager::addManualMeasurement()
{
    QSharedPointer<WeighScaleTest> test = qSharedPointerCast<WeighScaleTest>(m_test);
    QSharedPointer<WeightMeasurement> measure(new WeightMeasurement);

    test->addMeasurement(measure);

    double average = test->calculateAverage();
    if (m_debug)
      qDebug() << "WeighScaleTest::fromArray - new average" << average << "kg";


    emit dataChanged(m_test);
}

void WeighScaleManager::connectDevice()
{
    if (m_debug)
        qDebug() << "WeighScaleManager::connectDevice";

    // Connect to the serial port and set up listeners

    if (m_port.isOpen())
        m_port.close();

    m_port.setDataBits(QSerialPort::Data8);
    m_port.setParity(QSerialPort::NoParity);
    m_port.setStopBits(QSerialPort::OneStop);
    m_port.setBaudRate(QSerialPort::Baud9600);

    connect(&m_port, &QSerialPort::readyRead, this, &WeighScaleManager::readDevice);

    connect(&m_port, &QSerialPort::errorOccurred, this, &WeighScaleManager::handleSerialPortError);

    connect(&m_port,
            &QSerialPort::dataTerminalReadyChanged,
            this,
            &WeighScaleManager::handleDataTerminalReadyChanged);

    connect(&m_port,
            &QSerialPort::requestToSendChanged,
            this,
            &WeighScaleManager::handleRequestToSendChanged);

    if (m_port.open(QSerialPort::ReadWrite)) {
        emit deviceConnected(QSerialPortInfo(m_port));

        // try and read the scale ID, if we can do that then emit the
        // canMeasure signal
        // the canMeasure signal is emitted from readDevice slot on successful read
        //
        m_request = QByteArray("i");
        writeDevice();
    } else
        qDebug() << "could not open port";
}

void WeighScaleManager::selectDevice(const QSerialPortInfo &port)
{
    SerialPortManager::selectDevice(port);

    CypressSettings::writeSetting("weight_scale/portName", port.portName());
    m_deviceName = port.portName();
}

void WeighScaleManager::zeroDevice()
{
    if (m_debug)
        qDebug() << "WeighScaleManager::zeroDevice";

    m_request = QByteArray("z");
    writeDevice();
}

void WeighScaleManager::readDevice()
{
    if (m_debug)
        qDebug() << "WeighScaleManager::readDevice";

    QByteArray data = m_port.readAll();
    m_buffer += data;

    if (m_debug)
        qDebug() << "WeighScaleManager::readDevice - read device received buffer "
                 << m_buffer.toHex();

    if (m_buffer.isEmpty())
        return;

    if ("i" == QString(m_request)) {
        m_deviceData["software_id"] = QString(m_buffer.simplified());
        // signal the GUI that the measure button can be clicked
        //
        emit canMeasure();
    } else if ("p" == QString(m_request)) {
        WeighScaleTest* test = static_cast<WeighScaleTest*>(m_test.get());
        test->fromArray(m_buffer);

        if (m_debug)
            qDebug() << "received p request, read buffer" << m_buffer;

        if (m_test->isValid()) {
            if (m_debug)
                qDebug() << "test is valid, can save results";

            emit dataChanged(m_test);
            emit canFinish();
        } else {
            QMessageBox::critical(nullptr, "Received invalid data", "Data received was invalid");
            emit canMeasure();
        }
    } else if ("z" == QString(m_request)) {
        WeightMeasurement m;
        m.fromArray(m_buffer);

        // signal the GUI that the measure button can be clicked
        //
        if (m.isZero())
            emit canMeasure();
    }
}

void WeighScaleManager::writeDevice()
{
    if (m_debug)
        qDebug() << "WeighScaleManager::writeDevice";

    // prepare to receive data
    //
    m_buffer.clear();
    m_port.write(m_request);
}

bool WeighScaleManager::setUp()
{
    if (m_debug)
        qDebug() << "WeighScaleManager::setUp";

    return true;
}

bool WeighScaleManager::cleanUp()
{
    if (m_debug)
        qDebug() << "WeighScaleManager::cleanUp";

    return clearData();
}

bool WeighScaleManager::clearData()
{
    if (m_debug)
        qDebug() << "WeighScaleManager::clearData";

    m_test->reset();

    return true;
}



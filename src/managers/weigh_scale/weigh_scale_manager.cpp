//#include "auxiliary/Utilities.h"
#include "data/weigh_scale/tests/weigh_scale_test.h"
#include "weigh_scale_manager.h"
#include "cypress_session.h"

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

WeighScaleManager::WeighScaleManager(const CypressSession& session)
    : ManagerBase(session)
{
    m_test.reset(new WeighScaleTest);
    m_test->setExpectedMeasurementCount(2);
}

bool WeighScaleManager::isAvailable()
{
    return false;
}

bool WeighScaleManager::isInstalled()
{
    return false;
}

bool WeighScaleManager::clearData()
{
    m_test->reset();
    return true;
}

void WeighScaleManager::start()
{
    emit started(m_test.get());
    emit canMeasure();
}

void WeighScaleManager::finish()
{
    int answer_id = m_session.getAnswerId();

    QJsonObject testJson = m_test->toJsonObject();
    QJsonObject sessionObj = m_session.getJsonObject();

    QJsonObject responseJson;
    testJson.insert("session", sessionObj);

    responseJson.insert("value", testJson);

    QJsonDocument jsonDoc(responseJson);
    QByteArray serializedData = jsonDoc.toJson();

    sendHTTPSRequest("PATCH",
        "https://blueberry.clsa-elcv.ca/qa/pine/api/answer/" + QString::number(answer_id),
        "application/json",
        serializedData
    );

    emit success("");
}

//void WeighScaleManager::connectDevice()
//{
//    if(m_port.isOpen())
//        m_port.close();
//
//    if(m_port.open(QSerialPort::ReadWrite))
//    {
//      m_port.setDataBits(QSerialPort::Data8);
//      m_port.setParity(QSerialPort::NoParity);
//      m_port.setStopBits(QSerialPort::OneStop);
//      m_port.setBaudRate(QSerialPort::Baud9600);
//
//      connect(&m_port, &QSerialPort::readyRead,
//               this, &WeighScaleManager::readDevice);
//
//      connect(&m_port, &QSerialPort::errorOccurred,
//              this,[this](QSerialPort::SerialPortError error){
//                  Q_UNUSED(error)
//                  qDebug() << "AN ERROR OCCURED: " << m_port.errorString();
//              });
//
//      connect(&m_port, &QSerialPort::dataTerminalReadyChanged,
//              this,[](bool set){
//          qDebug() << "data terminal ready DTR changed to " << (set?"high":"low");
//      });
//
//      connect(&m_port, &QSerialPort::requestToSendChanged,
//              this,[](bool set){
//          qDebug() << "request to send RTS changed to " << (set?"high":"low");
//      });
//
//      // try and read the scale ID, if we can do that then emit the
//      // canMeasure signal
//      // the canMeasure signal is emitted from readDevice slot on successful read
//      //
//      m_request = QByteArray("i");
//      writeDevice();
//    }
//}

//void WeighScaleManager::zeroDevice()
//{
//    m_request = QByteArray("z");
//    writeDevice();
//}

void WeighScaleManager::measure()
{
    m_test->reset();
    m_test->simulate();

    emit measured(m_test.get());

    if (m_test->isValid())
    {
        emit canFinish();
    }
}

//void WeighScaleManager::readDevice()
//{
//      QByteArray data = m_port.readAll();
//      m_buffer += data;
//      qDebug() << "read device received buffer " << QString(m_buffer);
//
//    if(!m_buffer.isEmpty())
//    {
//      if("i" == QString(m_request))
//      {
//        //m_deviceData.setAttribute("software_id", QString(m_buffer.simplified()));
//        // signal the GUI that the measure button can be clicked
//        //
//        emit canMeasure();
//      }
//      else if("p" == QString(m_request))
//      {
//         m_test.fromArray(m_buffer);
//         qDebug() << "received p request, read buffer" << m_buffer;
//         if(m_test.isValid())
//         {
//           qDebug() << "test is valid, can save results";
//           // emit the can write signal
//           emit canFinish();
//         }
//         else
//             qDebug() << "invalid test";
//      }
//      else if("z" == QString(m_request))
//      {
//          WeightMeasurement m;
//          m.fromArray(m_buffer);
//          if(m.isZero())
//          {
//            // signal the GUI that the measure button can be clicked
//            //
//            emit canMeasure();
//          }
//      }
//
//    }
//}

//void WeighScaleManager::writeDevice()
//{
//    // prepare to receive data
//    //
//    m_buffer.clear();
//    m_port.write(m_request);
//}

// set input parameters for the test
void WeighScaleManager::setInputData(const QVariantMap& inputData)
{
    Q_UNUSED(inputData)
}

bool WeighScaleManager::setUp()
{
    return true;
}

bool WeighScaleManager::cleanUp()
{
    return true;
}



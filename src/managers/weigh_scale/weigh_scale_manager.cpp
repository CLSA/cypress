//#include "auxiliary/Utilities.h"
#include "auxiliary/json_settings.h"
#include "data/weigh_scale/tests/weigh_scale_test.h"

#include "weigh_scale_manager.h"
#include "cypress_application.h"

#include <QDateTime>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QRandomGenerator>
#include <QSerialPortInfo>
#include <QSettings>
#include <QStandardItemModel>
#include <QtMath>

WeighScaleManager::WeighScaleManager(const CypressSession& session)
    : SerialPortManager(session)
{
  m_test.setExpectedMeasurementCount(2);
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
    m_test.reset();
    return false;
}

void WeighScaleManager::finish()
{
    //QJsonObject results = JsonSettings::readJsonFromFile(
    //    "C:/dev/clsa/cypress/src/tests/fixtures/weigh_scale/output.json"
    //);
    //if (results.empty()) return;

    //results["cypress_session"] = m_uuid;
    //results["answer_id"] = m_answerId;
    //results["barcode"] = m_barcode;
    //results["interviewer"] = m_interviewer;

    //bool ok = sendResultsToPine(results);
    //if (!ok)
    //{
    //    qDebug() << "Could not send results to Pine";
    //}
    //m_deviceData.reset();
    //m_deviceList.clear();
    //m_test.reset();
    //if(m_port.isOpen())
    //    m_port.close();
}

void WeighScaleManager::connectDevice()
{
    if(m_port.isOpen())
        m_port.close();

    if(m_port.open(QSerialPort::ReadWrite))
    {
      m_port.setDataBits(QSerialPort::Data8);
      m_port.setParity(QSerialPort::NoParity);
      m_port.setStopBits(QSerialPort::OneStop);
      m_port.setBaudRate(QSerialPort::Baud9600);

      connect(&m_port, &QSerialPort::readyRead,
               this, &WeighScaleManager::readDevice);

      connect(&m_port, &QSerialPort::errorOccurred,
              this,[this](QSerialPort::SerialPortError error){
                  Q_UNUSED(error)
                  qDebug() << "AN ERROR OCCURED: " << m_port.errorString();
              });

      connect(&m_port, &QSerialPort::dataTerminalReadyChanged,
              this,[](bool set){
          qDebug() << "data terminal ready DTR changed to " << (set?"high":"low");
      });

      connect(&m_port, &QSerialPort::requestToSendChanged,
              this,[](bool set){
          qDebug() << "request to send RTS changed to " << (set?"high":"low");
      });

      // try and read the scale ID, if we can do that then emit the
      // canMeasure signal
      // the canMeasure signal is emitted from readDevice slot on successful read
      //
      m_request = QByteArray("i");
      writeDevice();
    }
}

void WeighScaleManager::zeroDevice()
{
    m_request = QByteArray("z");
    writeDevice();
}

void WeighScaleManager::measure()
{
    if (Cypress::getInstance().isSimulation()) {
      QFile file("C:/dev/clsa/cypress/src/tests/fixtures/weigh_scale/output.json");
      if (!file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text)) {
          qDebug() << "Could not open file for write";
          return;
      }
      QByteArray data = file.readAll();

      QJsonDocument json = QJsonDocument::fromJson(data);
      QJsonObject dataData = json.object();

      QJsonObject valueObj = dataData["value"].toObject();
      QJsonObject results = valueObj["results"].toObject();

      results["body_mass_index"] = valueObj["average_weight"].toObject()["value"].toDouble() / std::pow(m_inputData["height"].toDouble(), 2);

      valueObj["results"] = results;
      dataData["value"] = valueObj;

      QJsonDocument doc(dataData);
      QTextStream outStream(&file);

      outStream << doc.toJson(QJsonDocument::Indented);
      outStream.flush();

      file.close();

      sendResultsToPine("C:/dev/clsa/cypress/src/tests/fixtures/weigh_scale/output.json");
      return;
    };

    //m_request = QByteArray("p");
    //writeDevice();
}

void WeighScaleManager::readDevice()
{
      QByteArray data = m_port.readAll();
      m_buffer += data;
      qDebug() << "read device received buffer " << QString(m_buffer);

    if(!m_buffer.isEmpty())
    {
      if("i" == QString(m_request))
      {
        //m_deviceData.setAttribute("software_id", QString(m_buffer.simplified()));
        // signal the GUI that the measure button can be clicked
        //
        emit canMeasure();
      }
      else if("p" == QString(m_request))
      {
         m_test.fromArray(m_buffer);
         qDebug() << "received p request, read buffer" << m_buffer;
         if(m_test.isValid())
         {
           qDebug() << "test is valid, can save results";
           // emit the can write signal
           emit canFinish();
         }
         else
             qDebug() << "invalid test";
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
}

void WeighScaleManager::writeDevice()
{
    // prepare to receive data
    //
    m_buffer.clear();
    m_port.write(m_request);
}

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



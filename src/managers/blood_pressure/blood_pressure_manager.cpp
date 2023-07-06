#include "blood_pressure_manager.h"
#include "auxiliary/json_settings.h"

#include "cypress_application.h"
#include "bpm_communication.h"

#include <QCoreApplication>
#include <QDebug>
#include <QtUsb/QHidDevice>
#include <QJsonObject>
#include <QSettings>
#include <QStandardItemModel>
#include <QtUsb/QtUsb>

BloodPressureManager::BloodPressureManager(const CypressSession& session)
    : ManagerBase(session), m_comm(new BPMCommunication())
{
    m_test.setExpectedMeasurementCount(6);
}

BloodPressureManager::~BloodPressureManager()
{
    delete m_comm;
}

//bool BloodPressureManager::isAvailable()
//{
//    bool found = scanDevices();
//    return found;
//}

void BloodPressureManager::start()
{
    // connect manager to communication
    //connect(this, &BloodPressureManager::attemptConnection, m_comm, &BPMCommunication::connect);
    //connect(this, &BloodPressureManager::startMeasurement, m_comm, &BPMCommunication::measure);
    //connect(this, &BloodPressureManager::abortMeasurement, m_comm, &BPMCommunication::abort);

    //// connect communication to manager
    //connect(m_comm, &BPMCommunication::abortFinished, this, &BloodPressureManager::abortComplete);
    //connect(m_comm, &BPMCommunication::connectionStatus, this, &BloodPressureManager::connectionStatusChanged);
    //connect(m_comm, &BPMCommunication::measurementReady, this, &BloodPressureManager::measurementAvailable);
    //connect(m_comm, &BPMCommunication::averageReady, this, &BloodPressureManager::averageAvailable);
    //connect(m_comm, &BPMCommunication::finalReviewReady, this, &BloodPressureManager::finalReviewAvailable);
    ////connect(m_comm, &BPMCommunication::measurementError, this, &BloodPressureManager::message);

    //// move communication to thread and start
    //if(m_comm->thread() != &m_thread)
    //{
    //  m_comm->moveToThread(&m_thread);
    //  m_thread.start();
    //}

    //scanDevices();
    //emit dataChanged();
}

//bool BloodPressureManager::isDefined(const QString &label) const
//{
//    return false;
//}

bool BloodPressureManager::isAvailable()
{
  return false;
}

bool BloodPressureManager::isInstalled()
{
  return false;
}

void BloodPressureManager::selectDeviceById(const QUsb::Id& info)
{
    Q_UNUSED(info)
}

void BloodPressureManager::selectDevice(const QString &label)
{
    Q_UNUSED(label)
}

bool BloodPressureManager::scanDevices()
{
    bool found = false;
    return found;
}

void BloodPressureManager::setDevice(const QUsb::Id &info)
{
    Q_UNUSED(info)
}

void BloodPressureManager::setCuffSize(const QString &size)
{
  if(size.isNull() || 0 == size.length()) return;
  if(size.toLower() != m_cuffSize)
  {
    m_cuffSize = size.toLower();
    m_test.setCuffSize(m_cuffSize);
    emit cuffSizeChanged(m_cuffSize);
  }
}

void BloodPressureManager::setSide(const QString &side)
{
  if(side.isNull() || 0 == side.length()) return;
  if(side.toLower() != m_side)
  {
    m_side = side.toLower();
    m_test.setSide(m_side);
    emit sideChanged(m_side);
  }
}

// slot for UI communication
//
void BloodPressureManager::measure()
{
  if (Cypress::getInstance().isSimulation())
  {
    sendResultsToPine("C:/dev/clsa/cypress/src/tests/fixtures/blood_pressure/output.json");
    return;
  }
}


//void BloodPressureManager::clearData()
//{
//}

// slot for UI communication
//
void BloodPressureManager::disconnectDevice()
{

}

// slot for UI communication
//
void BloodPressureManager::finish()
{
    //QJsonObject results = JsonSettings::readJsonFromFile(
    //    "C:/dev/clsa/cypress/src/tests/fixtures/blood_pressure/output.json"
    //);

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

// slot for BPMCommunication
//
void BloodPressureManager::measurementAvailable(
  const int& readingNum, const int& sbp, const int& dbp, const int& pulse,
  const QDateTime& start, const QDateTime& end)
{
    Q_UNUSED(end)
    Q_UNUSED(start)
    Q_UNUSED(pulse)
    Q_UNUSED(readingNum)
    Q_UNUSED(sbp)
    Q_UNUSED(dbp)
}

// slot for BPMCommunication
//
void BloodPressureManager::averageAvailable(
  const int& sbp, const int& dbp, const int& pulse)
{
    Q_UNUSED(sbp)
    Q_UNUSED(dbp)
    Q_UNUSED(pulse)
}

// slot for BPMCommunication
//
void BloodPressureManager::finalReviewAvailable(
  const int& sbp, const int& dbp, const int& pulse)
{
    Q_UNUSED(sbp)
    Q_UNUSED(dbp)
    Q_UNUSED(pulse)
}

// slot for UI communication
//
void BloodPressureManager::connectDevice()
{
}

// slot for BPMCommunication
//
void BloodPressureManager::connectionStatusChanged(const bool& connected)
{
    Q_UNUSED(connected)
}

// slot for BPMCommunication
//
void BloodPressureManager::deviceInfoAvailable()
{

}

// slot for BPMCommunication
//
void BloodPressureManager::abortComplete(const bool& success)
{
    Q_UNUSED(success)
}

// Set up device
bool BloodPressureManager::setUp()
{
    return true;
}

// Clean up the device for next time
bool BloodPressureManager::cleanUp()
{
    return true;
}

// Clean up the device for next time
bool BloodPressureManager::clearData()
{
    return true;
}

void BloodPressureManager::setInputData(const QVariantMap& inputData)
{
    Q_UNUSED(inputData)
}


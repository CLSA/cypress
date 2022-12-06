#include "PatScanQueryHelper.h"
#include <QSqlError>
#include <QDebug>

PatScanQueryHelper::PatScanQueryHelper(QSettings& settings)
{
   const QString databaseDriver = settings.value("dxa/patScan/driver", "QODBC").toString();
   const QString hostname = settings.value("dxa/patScan/hostname", "").toString();
   const QString database = settings.value("dxa/patScan/database", "").toString();
   const QString username = settings.value("dxa/patScan/username", "").toString();
   const QString password = settings.value("dxa/patScan/password", "").toString();

   m_database = QSqlDatabase::addDatabase(databaseDriver);
   m_database.setHostName(hostname);
   m_database.setDatabaseName(database);
   m_database.setUserName(username);
   m_database.setPassword(password);
}

bool PatScanQueryHelper::connect()
{
    if (!m_database.isValid()) return false;

    return m_database.open();
}

void PatScanQueryHelper::disconnect()
{
    m_database.close();
}

QMap<QString, QVariant> PatScanQueryHelper::getScanAnalysisData(QString patientKey, QString scanType)
{
    bool connected = connect();
    if (!connected) {
        throw std::exception("PatScanQueryHelper::could not connect to PatScan.mdb");
    }

    QSqlQuery query;
    QString scanId;
    QString scanMode;
    QString scanDate;

    query.prepare("SELECT SCANID, SCAN_MODE, SCAN_DATE FROM ScanAnalysis WHERE PATIENT_KEY = ? AND SCAN_TYPE = ?");
    query.bindValue(0, patientKey);
    query.bindValue(1, scanType);
    query.exec();

    QSqlError lastError = query.lastError();
    if (lastError.type() != QSqlError::NoError) {
        qDebug() << lastError.text();
        throw std::exception("PatScanQueryHelper:: Query error");
    }

    // only interested in most recent scan?
    query.last();
    scanId = query.value(0).toString();
    scanMode = query.value(1).toString();
    scanDate = query.value(2).toString();


    return QMap<QString, QVariant> {
        { "scanId", scanId },
        { "scanMode", scanMode },
        { "scanDate", scanDate },
    };

    disconnect();
}

QMap<QString, QVariant> PatScanQueryHelper::getScanData(QString patientKey, QString scanType)
{
    QByteArray dicomBlobs[10]; // num of files for scan
    QMap<QString, QVariant> data;

    try {
        data = getScanAnalysisData(patientKey, scanType);
        // Get dicom files based on patientKey and scanType
        // check if dicom files valid (compare tags)
        // if valid:
            // convert to binary blobs, encode as base64, and add to data

        // compute t and z scores, add to metadata or measurement?
        // compress using gzip and send to pine server

        return data;
    }
    catch (...)
    {

    }
}
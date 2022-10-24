#include <QProcess>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QJsonDocument>
#include <QFileInfo>
#include <QSettings>

#include "RetinalCameraManager.h"
#include "qsqlerror.h"


RetinalCameraManager::RetinalCameraManager(QObject *parent)
    : ManagerBase{parent}
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");

    m_runnableName      = settings.value("instruments/retinal_scan/exe").toString();
    m_defaultPatientUid = settings.value("instruments/retinal_scan/patient_id").toString();
    m_databaseVersion   = settings.value("instruments/retinal_scan/database/version").toString();
    m_databaseDriver    = settings.value("instruments/retinal_scan/database/driver").toString();
    m_database		    = settings.value("instruments/retinal_scan/database/database").toString();
    m_databaseHost      = settings.value("instruments/retinal_scan/database/host").toString();
    m_databasePort      = settings.value("instruments/retinal_scan/database/port").toString();
    m_databaseUser      = settings.value("instruments/retinal_scan/database/user").toString();
    m_databasePassword  = settings.value("instruments/retinal_scan/database/password").toString();

    qInfo() << m_runnableName;
    qInfo() << m_defaultPatientUid;
    qInfo() << m_databaseVersion;
    qInfo() << m_databaseDriver;
    qInfo() << m_databaseHost;
    qInfo() << m_databasePort;
    qInfo() << m_databaseUser;
    qInfo() << m_databasePassword;
}

// load and save device, paths and other constant settings to .ini
//
void RetinalCameraManager::loadSettings(const QSettings &)
{
    if (m_verbose)
        qDebug() << "RetinalCameraManager::loadSettings";
}

void RetinalCameraManager::saveSettings(QSettings*) const
{
    if (m_verbose)
        qDebug() << "RetinalCameraManager::saveSettings";
}


void RetinalCameraManager::setInputData(const QVariantMap &data)
{
    if (m_verbose)
        qDebug() << "RetinalCameraManager::setInputData";

    m_inputData = data;
}

// collate test results and device and other meta data
// for the main application to write to .json
//
QJsonObject RetinalCameraManager::toJsonObject() const
{
    if (m_verbose)
        qDebug() << "RetinalCameraManager::toJsonObject";

    return QJsonDocument::fromVariant(m_inputData).object();
}

// build a model from test and measurement data for UI display
// of the results
//
void RetinalCameraManager::initializeModel()
{
    if (m_verbose)
        qDebug() << "RetinalCameraManager::initializeModel";
}

// update the model whenever the test data changes
//
void RetinalCameraManager::updateModel()
{
    if (m_verbose)
        qDebug() << "RetinalCameraManager::updateModel";
}

// subclasses call methods after main initialization just prior
// to running (eg., emit dataChanged signal)
//

// org.obiba.onyx.jade.instrument.topcon.imagenetr4lite.workDir=C:/Program\ Files/TOPCON/IMAGEnet\ R4/
// org.obiba.onyx.jade.instrument.topcon.imagenetr4lite.executable=imagenet.exe
//
// org.obiba.onyx.jade.instrument.topcon.imagenetr4lite.driver=net.sourceforge.jtds.jdbc.Driver
// org.obiba.onyx.jade.instrument.topcon.imagenetr4lite.url=jdbc:jtds:sqlserver://localhost:1433/IMAGEnet_R4
// org.obiba.onyx.jade.instrument.topcon.imagenetr4lite.username=
// org.obiba.onyx.jade.instrument.topcon.imagenetr4lite.password=
void RetinalCameraManager::start()
{
    if (m_verbose)
        qDebug() << "RetinalCameraManager::start";

    QFileInfo info(m_runnableName);
    QStringList arguments;


    // Open DB connection and clean up database
    QSqlDatabase db = QSqlDatabase::addDatabase(m_databaseDriver);
    db.setHostName(m_databaseHost);
    db.setDatabaseName(m_database);
    db.setUserName(m_databaseUser);
    db.setPassword(m_databasePassword);

    bool ok = db.open();

    if (!ok)
    {
        qDebug() << "could not open db";
    }

    QSqlQuery query;
    QSqlQuery locationQuery;

    // clean up
    //

    // delete all images returned
    ok = query.exec("SELECT FileName, FileExt, StoragePathUid FROM dbo.Media WHERE PatientUid = '" + m_defaultPatientUid + "'");
    if (!ok) qDebug() << query.lastError();
    while (query.next()) {
        QString fileName = query.value(0).toString();
        QString fileExt = query.value(1).toString();
        QString storagePathUid = query.value(2).toString();

        ok = locationQuery.exec("SELECT Location FROM dbo.StoragePaths WHERE StoragePathUid = '" + storagePathUid + "'");

        if (!ok) qDebug() << query.lastError();

        while (locationQuery.next()) {
            QString location = locationQuery.value(0).toString();
            qInfo() << "Deleting: " << location << "/" << fileName + fileExt;
        }

        qDebug() << fileName << " " << fileExt << " " << storagePathUid;
    }

    ok = query.exec("DELETE FROM dbo.Exams WHERE PatientUid = '" + m_defaultPatientUid + "'");
    if (!ok) qDebug() << query.lastError();

    ok = query.exec("DELETE FROM dbo.Media WHERE PatientUid = '" + m_defaultPatientUid + "'");
    if (!ok) qDebug() << query.lastError();

    ok = query.exec("DELETE FROM dbo.Patients WHERE PatientUid = '" + m_defaultPatientUid + "'");
    if (!ok) qDebug() << query.lastError();

    ok = query.exec("DELETE FROM dbo.Persons WHERE PersonUid = '" + m_defaultPatientUid + "'");
    if (!ok) qDebug() << query.lastError();

    // initialize DB data
    ok = query.exec("INSERT INTO dbo.Persons (PersonUid, SurName, ForeName) values('11111111-2222-3333-4444-555555555555', 'Anthony', 'Hoare')");
    if (!ok) qDebug() << query.lastError();

    ok = query.exec("INSERT INTO dbo.Patients (PatientUid, PatientIdentifier, PersonUid) values('11111111-2222-3333-4444-555555555555', '11111111-2222-3333-4444-555555555555', '11111111-2222-3333-4444-555555555555')");
    if (!ok) qDebug() << query.lastError();

    if (!info.exists())
    {
       qDebug() << "Program does not exist: " << m_runnableName;
       return;
    }

    if (!info.isExecutable())
    {
        qDebug() << "Program is not executable: " << m_runnableName;
        return;
    }

    // clean last participant data from database
    //
    if (m_verbose)
        qDebug() << "cleaning database..";


    m_process.setProgram(m_runnableName);
    m_process.start();

    bool started = m_process.waitForStarted();
    if (!started)
    {
        if (m_verbose)
            qDebug() << "program running..";
        return;
    }

    // enter participant data to database
    //
    if (m_verbose)
        qDebug() << "entering new participant data..";

    // launch process
    //
    if (m_verbose)
        qDebug() << "launching process..";

    // wait for process to finish
    //
    if (m_verbose)
        qDebug() << "started, now waiting..";

    m_process.waitForFinished(360000);

    if (m_verbose)
        qDebug() << "process finished..";

    // process data for left and right eye
    //
    QSqlQuery dataQuery;

    ok = dataQuery.exec("SELECT FileName, FileExt, StoragePathUid FROM dbo.Media WHERE PatientUid = '" + m_defaultPatientUid + "'");
    if (!ok) qDebug() << dataQuery.lastError();

    while (dataQuery.next()) {
        QString fileName = dataQuery.value(0).toString();
        QString fileExt = dataQuery.value(1).toString();
        QString storagePathUid = dataQuery.value(2).toString();

        ok = locationQuery.exec("SELECT Location FROM dbo.StoragePaths WHERE StoragePathUid = '" + m_defaultPatientUid + "'");

        if (!ok) qDebug() << query.lastError();

        while (locationQuery.next()) {
            QString location = locationQuery.value(0).toString();
            qInfo() << "Located image: " << location << "/" << fileName + fileExt;

            // get image bytes and put into json
            // get eye side name and put into json

        }

        qDebug() << fileName << " " << fileExt << " " << storagePathUid;
    }

    // save json data locally and send to Pine
    //
    if (m_verbose)
        qDebug() << "submit..";
}

// SLOT
// actual measure will only execute if the barcode has been
// verified.  Subclasses must reimplement accordingly.
//
void RetinalCameraManager::measure()
{
    if (m_verbose)
        qDebug() << "RetinalCameraManager::measure";
}

// SLOT
// subclasses call methods just prior to main close event
//
void RetinalCameraManager::finish()
{
    if (m_verbose)
        qDebug() << "RetinalCameraManager::finish";
}

// SLOT
// Context dependent clear test data and possibly device data (eg., serial port info)
void RetinalCameraManager::clearData()
{
    if (m_verbose)
        qDebug() << "RetinalCameraManager::clearData";
}

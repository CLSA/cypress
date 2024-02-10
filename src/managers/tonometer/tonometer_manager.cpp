#include "cypress_settings.h"

#include "tonometer_manager.h"
#include "server/sessions/tonometer_session.h"

#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardItemModel>

TonometerManager::TonometerManager(QSharedPointer<TonometerSession> session)
    : ManagerBase(session)
{
    m_test.reset(new TonometerTest);
    m_test->setExpectedMeasurementCount(2);

    m_db = QSqlDatabase::addDatabase("QODBC");

    m_runnableName = CypressSettings::readSetting("tonometer/runnableName").toString();
    m_runnablePath = CypressSettings::readSetting("tonometer/runnablePath").toString();
    m_databasePath = CypressSettings::readSetting("tonometer/databasePath").toString();
    m_temporaryPath = CypressSettings::readSetting("tonometer/temporaryPath").toString();

    m_db.setDatabaseName("Driver={Microsoft Access Driver (*.mdb)};DBQ=" + QDir::toNativeSeparators(m_databasePath));

    if (!m_db.open()) {
        qWarning() << "Error: Unable to connect to database.";
        qWarning() << "Database error:" << m_db.lastError().text();
    }
}

TonometerManager::~TonometerManager()
{
    m_db.close();
}

bool TonometerManager::start()
{
    if (m_debug)
        qDebug() << "TonometerManager::start";

    if (!setUp()) {
        emit error("Something went wrong. Please contact support");
        return false;
    }

    measure();

    return true;
}


bool TonometerManager::isInstalled()
{
    bool isDebugMode = CypressSettings::isDebugMode();

    QString runnableName = CypressSettings::readSetting("tonometer/runnableName").toString();
    QString runnablePath = CypressSettings::readSetting("tonometer/runnablePath").toString();
    QString databasePath = CypressSettings::readSetting("tonometer/databasePath").toString();
    QString temporaryPath = CypressSettings::readSetting("tonometer/temporaryPath").toString();

    if (runnablePath.isNull() || runnablePath.isEmpty()) {
        if (isDebugMode)
            qDebug() << "TonometerManager: runnablePath is not defined";
        return false;
    }

    if (runnableName.isNull() || runnableName.isEmpty()) {
        if (isDebugMode)
            qDebug() << "TonometerManager: runnableName is not defined";
        return false;
    }

    if (databasePath.isNull() || databasePath.isEmpty()) {
        if (isDebugMode)
            qDebug() << "TonometerManager: databasePath is not defined";
        return false;
    }

    if (temporaryPath.isNull() || temporaryPath.isEmpty()) {
        if (isDebugMode)
            qDebug() << "TonometerManager: temporaryPath is not defined";
        return false;
    }

    QDir runnablePathInfo(runnablePath);
    if (!runnablePathInfo.exists()) {
        if (isDebugMode) {
            qDebug() << "TonometerManager: runnable does not exist";
        }
        return false;
    }


    QFileInfo runnableNameInfo(runnableName);
    if (!runnableNameInfo.exists()) {
        if (isDebugMode) {
            qDebug() << "TonometerManager: runnable does not exist";
        }
        return false;
    }
    if (!runnableNameInfo.isExecutable()) {
        if (isDebugMode) {
            qDebug() << "TonometerManager: runnable is not executable";
        }
        return false;
    }

    QFileInfo databaseFileInfo(databasePath);
    if (!databaseFileInfo.isFile()) {
        if (isDebugMode) {
            qDebug() << "TonometerManager: database is not a file";
        }
        return false;
    }
    if (!databaseFileInfo.isReadable()) {
        if (isDebugMode) {
            qDebug() << "TonometerManager: database is not readable";
        }
        return false;
    }

    QDir backupDir(temporaryPath);
    if (!backupDir.exists()) {
        if (isDebugMode) {
            qDebug() << "TonometerManager: backup dir does not exist";
        }
        return false;
    }

    return true;
}

void TonometerManager::measure()
{
    if (m_debug)
        qDebug() << "TonometerManager::measure";

    clearData();

    if (m_sim)
    {
        m_test->simulate({});
        return;
    }

    if (m_process.state() != QProcess::NotRunning) {
        emit error("ORA is already running");
        return;
    }

    m_process.start();

    if (!m_process.waitForStarted()) {
        emit error("Could not start the ORA application");
        return;
    }
}

void TonometerManager::readOutput()
{
    if (m_debug)
        qDebug() << "TonometerManager::readOutput";

    QVariantMap leftResults = extractMeasures(m_session->getBarcode().toInt(), "Left");
    QVariantMap rightResults = extractMeasures(m_session->getBarcode().toInt(), "Right");

    QList<QVariantMap> results { leftResults, rightResults };

    QSharedPointer<TonometerTest> test = qSharedPointerCast<TonometerTest>(m_test);
    test->fromVariantMapList(results);

    finish();
}

void TonometerManager::configureProcess()
{
    if (m_debug)
        qDebug() << "TonometerManager::configureProcess";

    // connect signals and slots to QProcess one time only
    //
    connect(&m_process, &QProcess::started,
        this, [this]() {
            qDebug() << "process started: " << m_process.arguments().join(" ");
        });

    connect(&m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        this, &TonometerManager::readOutput);

    connect(&m_process, &QProcess::errorOccurred,
        this, [](QProcess::ProcessError error)
        {
            QStringList s = QVariant::fromValue(error).toString().split(QRegExp("(?=[A-Z])"), Qt::SkipEmptyParts);
            qDebug() << "ERROR: process error occured: " << s.join(" ").toLower();
        });

    connect(&m_process, &QProcess::stateChanged,
        this, [](QProcess::ProcessState state) {
            QStringList s = QVariant::fromValue(state).toString().split(QRegExp("(?=[A-Z])"), Qt::SkipEmptyParts);
            qDebug() << "process state: " << s.join(" ").toLower();
        });
}



bool TonometerManager::clearData()
{
    if (m_debug)
        qDebug() << "TonometerManager::clearData";

    m_test->reset();
    restoreDatabase();

    return false;
}

bool TonometerManager::backupData() {
    QFileInfo databasePath(m_databasePath);
    QDir backupPath(m_temporaryPath);

    if (!QFile::exists(databasePath.absoluteFilePath()) ) {
        qDebug() << "tonometer database file does not exist at path" << databasePath.absoluteFilePath();
        return false;
    }

    QString backupDatabasePath = backupPath.absoluteFilePath(databasePath.fileName());
    if (QFileInfo::exists(backupDatabasePath)) {
        if (!QFile::remove(backupDatabasePath)) {
            qDebug() << "tonometer backup exists but cannot be removed..";
            return false;
        }
    }

    if (!QFile::copy(databasePath.absoluteFilePath(), backupDatabasePath)) {
        qDebug() << "could not copy database file at " << databasePath.absoluteFilePath() << "to" << backupDatabasePath;
        return false;
    }

    return true;
}

bool TonometerManager::restoreData() {
    QDir backupPath(m_temporaryPath);
    QFileInfo databasePath(m_databasePath);

    QString backupDatabasePath = backupPath.absoluteFilePath(databasePath.fileName());
    if (!backupPath.exists() ) {
        return false;
    }

    if (!QFile::remove(m_databasePath)) {
        return false;
    }

    if(!QFile::copy(backupDatabasePath, databasePath.absoluteFilePath())) {
        return false;
    }

    return true;
}

bool TonometerManager::setUp()
{
    if (m_debug)
        qDebug() << "TonometerManager::setUp";

    if (!backupData()) {
        qDebug() << "could not backup data";
        return false;
    }

    configureProcess();

    bool ok = insertPatient(
        m_session->getBarcode(),
        m_session->getInputData()["date_of_birth"].toString(),
        m_session->getInputData()["sex"].toString(),
        m_session->getBarcode().toInt()
    );

    if (!ok) {
        qDebug() << "could not insert patient";
        return false;
    }

    return true;
}

bool TonometerManager::cleanUp()
{
    if (m_debug)
        qDebug() << "TonometerManager::cleanUp";

    //if (!restoreDatabase()) {
    //    return false;
    //}

    if (!restoreData()) {
        qDebug() << "could not restore database";
        return false;
    }

    return true;
}

bool TonometerManager::insertPatient(
                                     const QString &name,
                                     const QString &birthDate,
                                     const QString &sex,
                                     const int id)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO Patients (PatientID, Name, BirthDate, Sex, GroupID, ID, RaceID) VALUES (?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(id);
    query.addBindValue(name);
    query.addBindValue(QDate::fromString(birthDate));
    query.addBindValue(sex.startsWith("m", Qt::CaseSensitivity::CaseInsensitive));
    query.addBindValue(2);
    query.addBindValue(id);
    query.addBindValue(1);

    if (!query.exec()) {
        qWarning() << "Database error:" << m_db.lastError().text();
        return false;
    }

    return true;

}

QVariantMap TonometerManager::extractMeasures(const int patientId, const QString &eye)
{
    QSqlQuery query(m_db);
    QVariantMap resultMap;

    query.prepare("SELECT * from Measures where PatientID = ? and Eye = ? ORDER BY MeasureDate desc");

    query.addBindValue(patientId);
    query.addBindValue(eye);

    if (!query.exec()) {
        qWarning() << "Database error:" << m_db.lastError().text();
        return resultMap;
    }

    while (query.next()) {
        for (int i = 0; i < query.record().count(); ++i) {
            resultMap.insert(query.record().fieldName(i), query.value(i));
        }
    }

    return resultMap;
}

bool TonometerManager::restoreDatabase()
{
    QSqlQuery query(m_db);

    query.prepare("DELETE FROM Patients WHERE PatientID = ?");
    query.addBindValue(m_session->getBarcode().toInt());

    if (!query.exec()) {
        qWarning() << "Error deleting data from table:" << query.lastError().text();
        return false;
    }

    query.prepare("DELETE FROM Measures WHERE PatientID = ?");
    query.addBindValue(m_session->getBarcode().toInt());

    if (!query.exec()) {
        qWarning() << "Error deleting data from table:" << query.lastError().text();
        return false;
    }

    return true;
}

#include "grip_strength_manager.h"
#include "data/grip_strength/tests/grip_strength_test.h"

#include "cypress_application.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQuery>

#include <QTemporaryDir>
#include <QFile>
#include <QtSql>

GripStrengthManager::GripStrengthManager(QSharedPointer<GripStrengthSession> session)
    : ManagerBase(session)
{
    m_test.reset(new GripStrengthTest);
    //QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");
    //m_workingDirPath = settings.value("instruments/grip_strength/working_dir").toString();
    //if (m_workingDirPath.isEmpty() || m_workingDirPath.isNull())
    //{
    //    const QString error = "working directory path for tracker 5 is not defined";
    //    qCritical() << error;
    //    throw std::runtime_error(error.toStdString());
    //}

    //m_executablePath = settings.value("instruments/grip_strength/executable").toString();
    //if (m_executablePath.isEmpty() || m_executablePath.isNull())
    //{
    //    const QString error = "executable path for Tracker 5 is not defined";
    //    qCritical() << error;
    //    throw std::runtime_error(error.toStdString());
    //}

    //m_databaseName = settings.value("instruments/grip_strength/databaseName").toString();
    //if (m_databaseName.isEmpty() || m_databaseName.isNull())
    //{
    //    const QString error = "database name is not defined";
    //    qCritical() << error;
    //    throw std::runtime_error(error.toStdString());
    //}
}

GripStrengthManager::~GripStrengthManager()
{
}

bool GripStrengthManager::isInstalled()
{
    return false;
}

void GripStrengthManager::start()
{
    emit started(m_test.get());
    emit canMeasure();
}

void GripStrengthManager::measure()
{
    m_test->reset();

    if (CypressSettings::isSimMode())
    {
        m_test->simulate();
        emit measured(m_test.get());
        emit canFinish();
    }
}

void GripStrengthManager::finish()
{
    QJsonObject responseJson {};

    int answer_id = m_session->getAnswerId();

    QJsonObject testJson = m_test->toJsonObject();
    testJson.insert("session", m_session->getJsonObject());

    responseJson.insert("value", testJson);

    QJsonDocument jsonDoc(responseJson);
    QByteArray serializedData = jsonDoc.toJson();

    QString answerUrl = CypressSettings::getAnswerUrl(answer_id);
    sendHTTPSRequest("PATCH", answerUrl, "application/json", serializedData);

    emit success("Measurements sent to Pine");
    //cleanUp();
}

void GripStrengthManager::addManualMeasurement()
{
    QSharedPointer<GripStrengthMeasurement> measurement(new GripStrengthMeasurement);
    m_test->addMeasurement(measurement);

    emit dataChanged(m_test.get());
}

bool GripStrengthManager::setUp() {
    //QString backupFolder = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/database_backup";
    //m_backupDir = QDir(backupFolder);
    //if (!m_backupDir.exists())
    //{
    //    if (!m_backupDir.mkpath(backupFolder))
    //    {
    //        throw std::runtime_error("Error creating temp directory");
    //    }
    //}

    //for (const QString &fileName : m_trackerDir.entryList(QDir::Files))
    //{
    //    if (!QFile::copy(m_trackerDir.filePath(fileName), m_backupDir.filePath(fileName)))
    //    {
    //        throw std::runtime_error("Could not backup file");
    //    }
    //}

    //m_process.setProgram(m_workingDirPath + m_executablePath);
    //m_process.start();

    //connect(&m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [this]
    //{
    //    finish();
    //});

    //m_process.waitForStarted();

    return true;
}

bool GripStrengthManager::cleanUp() {
    //if (m_database.isOpen())
    //{
    //    m_database.close();
    //}

    //if (!m_trackerDir.removeRecursively())
    //{
    //    throw std::runtime_error("Failed to remove trackerDir");
    //}

    //for (const QString &fileName : m_backupDir.entryList(QDir::Files))
    //{
    //    if (!QFile::copy(m_backupDir.filePath(fileName), m_trackerDir.filePath(fileName)))
    //    {
    //        throw std::runtime_error("Failed to open database");
    //    }
    //}

    //if (!m_backupDir.removeRecursively())
    //{
    //   throw std::runtime_error("Failed to remove backupDir");
    //}

    return true;
}

bool GripStrengthManager::clearData()
{
   //m_test.reset();
   //emit dataChanged();

   return true;
}


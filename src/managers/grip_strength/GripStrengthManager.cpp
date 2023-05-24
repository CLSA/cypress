#include "gripstrengthmanager.h"
//#include "auxiliary/tracker5util.h"
#include "auxiliary/JsonSettings.h"

#include "CypressApplication.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQuery>

#include <QTemporaryDir>
#include <QFile>
#include <QtSql>

GripStrengthManager::GripStrengthManager(QJsonObject inputData)
{

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

    m_inputData = jsonObjectToVariantMap(inputData);
}

GripStrengthManager::~GripStrengthManager()
{
}

void GripStrengthManager::start()
{
    setUp();
}

void GripStrengthManager::measure()
{
    //m_test.readMeasurements();
    //if (m_test.isValid())
    //{
    //    emit canFinish();
    //}

    if (CypressApplication::getInstance().isSimulation()) {
        sendJsonData("C:/work/clsa/cypress/src/tests/fixtures/grip_strength/output.json");
    }

    //results["cypress_session"] = m_uuid;
    //results["answer_id"] = m_answerId;
    //results["barcode"] = m_barcode;
    //results["interviewer"] = m_interviewer;

    //if (results.empty()) return;

    //bool ok = sendResultsToPine(results);
    //if (!ok)
    //{
    //    qDebug() << "Could not send results to Pine";
    //}
}

void GripStrengthManager::finish()
{
    cleanUp();
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

void GripStrengthManager::setInputData(const QVariantMap& inputData)
{
   //m_inputData = inputData;
}

bool GripStrengthManager::clearData()
{
   //m_test.reset();
   //emit dataChanged();

   return true;
}


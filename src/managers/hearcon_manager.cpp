#include "hearcon_manager.h"
#include "cypress_settings.h"
#include "data/hearing/tests/hearcon_test.h"

#include <QFile>
#include <QFileInfo>
#include <QProcess>

#include <QMessageBox>

#include <windows.h>
#include <TlHelp32.h>

#include "auxiliary/windows_util.h"


HearconManager::HearconManager(QSharedPointer<HearconSession> session): ManagerBase(session)
{
    m_processName = CypressSettings::readSetting("hearcon/processName").toString();

    m_processPath = CypressSettings::readSetting("hearcon/processPath").toString();
    m_processFile.setFileName(m_processPath);

    m_workingPath = CypressSettings::readSetting("hearcon/workingPath").toString();

    m_backupDatabasePath = CypressSettings::readSetting("hearcon/ra660/backupDatabasePath").toString();
    m_backupDatabaseFile.setFileName(m_backupDatabasePath);

    m_existingDatabasePath = CypressSettings::readSetting("hearcon/ra660/existingDatabasePath").toString();
    m_existingDatabase.setFileName(m_existingDatabasePath);

    m_readerPath = CypressSettings::readSetting("hearcon/plugin/path").toString();
    m_readerWorkingDirectory = CypressSettings::readSetting("hearcon/plugin/workingDirectory").toString();
    m_readerOutputPath = CypressSettings::readSetting("hearcon/plugin/outputDirectory").toString();

    m_test.reset(new HearconTest);
}

bool HearconManager::isInstalled()
{
    const QString processPath = CypressSettings::readSetting("hearcon/processPath").toString();
    const QString processName = CypressSettings::readSetting("hearcon/processName").toString();
    const QString workingPath = CypressSettings::readSetting("hearcon/workingPath").toString();

    const QString backupDatabasePath = CypressSettings::readSetting("hearcon/ra660/backupDatabasePath").toString();
    const QString existingDatabasePath = CypressSettings::readSetting("hearcon/ra660/existingDatabasePath").toString();


    const QString readerPath = CypressSettings::readSetting("hearcon/plugin/path").toString();
    const QString readerWorkingDirectory = CypressSettings::readSetting("hearcon/plugin/workingDirectory").toString();
    const QString readerOutputPath = CypressSettings::readSetting("hearcon/plugin/outputDirectory").toString();

    if (backupDatabasePath.isEmpty() || backupDatabasePath.isNull()) {
        qWarning() << "HearconManager::isInstalled - backupDatabasePath is not defined";
        return false;
    }

    QFileInfo backupDatabaseFile(backupDatabasePath);
    if (!backupDatabaseFile.exists() || !backupDatabaseFile.isReadable()) {
        qWarning() << "HearconManager::start - backupDatabaseFile does not exist";
        // If not, show error message
        return false;
    }

    if (existingDatabasePath.isEmpty() || existingDatabasePath.isNull()) {
        qWarning() << "HearconManager::isInstalled- existingDatabasePath is not defined";
        return false;
    }

    if (processPath.isEmpty() || processPath.isNull()) {
        qWarning() << "HearconManager::isInstalled - processPath is not defined";
        return false;
    }

    QFileInfo processFile(processPath);
    if (!processFile.exists() || !processFile.isExecutable()) {
        qWarning() << "HearconManager::isInstalled - process does not exist";
        return false;
    }

    if (processName.isEmpty() || processName.isNull()) {
        qWarning() << "HearconManager::isInstalled - processName is not defined";
        return false;
    }

    if (workingPath.isEmpty() || workingPath.isNull()) {
        qWarning() << "HearconManager::isInstalled - working path is not defined";
        return false;
    }

    if (readerPath.isNull() || readerPath.isEmpty()) {
        qWarning() << "HearconManager::isInstalled - hearcon/reader/path is not defined";
        return false;
    }

    if (readerWorkingDirectory.isNull() || readerWorkingDirectory.isEmpty()) {
        qWarning() << "HearconManager::isInstalled - hearcon/reader/workingDirectory is not defined";
        return false;
    }

    if (readerOutputPath.isNull() || readerOutputPath.isEmpty()) {
        qWarning() << "HearconManager::isInstalled - hearcon/reader/outputDirectory is not defined";
        return false;
    }

    return true;
}

bool HearconManager::start()
{
    qDebug() << "HearconManager::start";

    if (WindowsUtil::isProcessRunning(m_processName.toStdWString())) {
        qCritical() << "HearconManager::start - error, process is already running";
        QMessageBox::critical(nullptr, "Error", "HearCon is already running. Please close the application and try again.");
        return false;
    }

    QFile::remove(m_existingDatabasePath);

    /// Copy backup to the database location
    if (!m_backupDatabaseFile.copy(m_existingDatabasePath)) {
        qCritical() << "HearconManager::start - could not copy backup database to data folder";
        QMessageBox::critical(nullptr, "Error", "Could not start HearCon, please contact support");
        return false;
    }

    // Run initializer plugin
    QStringList pluginArguments;
    pluginArguments
        << m_existingDatabasePath
        << m_readerOutputPath
        << "initialize"
        << m_session->getBarcode()
        << m_session->getInputData()["dob"].toString()
        << m_session->getInputData()["sex"].toString();

    m_plugin.setProgram(m_readerPath);
    m_plugin.setArguments(pluginArguments);
    m_plugin.setWorkingDirectory(m_readerWorkingDirectory);
    m_plugin.setProcessChannelMode(QProcess::ForwardedChannels);

    m_plugin.start();
    m_plugin.waitForFinished();

    qDebug() << m_plugin.readAllStandardOutput();
    qDebug() << m_plugin.readAllStandardError();

    // Launch Hearcon app
    QStringList arguments;
    m_hearcon.setProgram(m_processPath);
    m_hearcon.setArguments(arguments);
    m_hearcon.setWorkingDirectory(m_workingPath);

    m_hearcon.start();
    m_hearcon.waitForStarted();

    // TODO add process error handling

    emit canMeasure();

    return true;
}

void HearconManager::measure()
{
    //if (WindowsUtil::isProcessRunning(m_processName.toStdWString())) {
    //    qCritical() << "HearconManager::start - error, process is already running";
    //    QMessageBox::warning(nullptr, "Application still running", "HearCon is still running. Please close the application and try again.");
    //    return;
    //}

    qDebug() << "HearconManager::measure";
    QStringList arguments;
    arguments
        << m_existingDatabasePath
        << m_readerOutputPath
        << "get_results"
        << m_session->getBarcode()
        << m_session->getInputData()["dob"].toString()
        << m_session->getInputData()["sex"].toString();

    m_plugin.setProgram(m_readerPath);
    m_plugin.setArguments(arguments);
    m_plugin.setWorkingDirectory(m_readerWorkingDirectory);
    m_plugin.setProcessChannelMode(QProcess::ForwardedChannels);

    qDebug() << "HearconManager::readOutput - starting readerProcess";

    m_plugin.start();
    m_plugin.waitForFinished();

    qDebug() << m_plugin.readAllStandardOutput();
    qDebug() << m_plugin.readAllStandardError();
    qDebug() << "Finished";

    // Read file from script
    //QFileInfo output(m_readerOutputPath);

    //if (!output.exists()) {
    //    qCritical() << "HearconManager::readOutput - no results found";
    //    emit error("No results found. Please contact support.");
    //    return;
    //}

    //if (!output.isReadable()) {
    //    qCritical() << "HearconManager::readOutput - cannot access results";
    //    emit error("Cannot access results. Please contact support.");
    //    return;
    //}

    //auto test = qSharedPointerCast<HearconTest>(m_test);
    //test->fromJsonFile(m_readerOutputPath);

    //if (test->isValid()) {
    //    emit canFinish();
    //}
    //else {
    //    qCritical() << "HearconManager::readOutput - results were invalid";
    //    emit error("Results were invalid. Please contact support.");
    //}
}

void HearconManager::readOutput() {
    qDebug() << "HearconManager::readOutput";
}

void HearconManager::finish()
{
    qDebug() << "HearconManager::finish";
}

void HearconManager::addManualMeasurement()
{
    qDebug() << "HearconManager::addManualMeasurement";
}

bool HearconManager::clearData()
{
    qDebug() << "HearconManager::clearData";
    return true;
}

bool HearconManager::cleanUp()
{
    qDebug() << "HearconManager::cleanUp";
    return true;
}

bool HearconManager::setUp()
{
    qDebug() << "HearconManager::setUp";
    return true;
}



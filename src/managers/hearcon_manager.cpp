#include "hearcon_manager.h"
#include "data/hearing/tests/hearcon_test.h"

#include <QFile>
#include <QFileInfo>
#include <QProcess>

#include <QMessageBox>

#include <windows.h>
#include <TlHelp32.h>

#include "auxiliary/windows_util.h"

DeviceConfig HearconManager::config {{
    { "processName",             { "hearcon/processName",                NonEmptyString }},
    { "processPath",             { "hearcon/processPath",                Exe            }},
    { "workingPath",             { "hearcon/workingPath",                Dir            }},
    { "backupDatabasePath",      { "hearcon/ra660/backupDatabasePath",   File           }},
    { "existingDatabasePath",    { "hearcon/ra660/existingDatabasePath", File           }},
    { "readerPath",              { "hearcon/plugin/path",                Exe            }},
    { "readerWorkingDirectory",  { "hearcon/plugin/workingDirectory",    Dir            }},
    { "readerOutputPath",        { "hearcon/plugin/outputDirectory",     NonEmptyString }},
}};

HearconManager::HearconManager(QSharedPointer<HearconSession> session): ManagerBase(session)
{
    m_processName = config.getSetting("processName");

    m_processPath = config.getSetting("processPath");
    m_processFile.setFileName(m_processPath);

    m_workingPath = config.getSetting("workingPath");

    m_backupDatabasePath = config.getSetting("backupDatabasePath");
    m_backupDatabaseFile.setFileName(m_backupDatabasePath);

    m_existingDatabasePath = config.getSetting("existingDatabasePath");
    m_existingDatabase.setFileName(m_existingDatabasePath);

    m_readerPath = config.getSetting("readerPath");
    m_readerWorkingDirectory = config.getSetting("readerWorkingDirectory");
    m_readerOutputPath = config.getSetting("readerOutputPath");

    m_test.reset(new HearconTest);
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
    m_test->reset();

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
    QFileInfo output(m_readerOutputPath);

    if (!output.exists()) {
        qCritical() << "HearconManager::readOutput - no results found";
        emit error("Test incomplete");
        return;
    }

    if (!output.isReadable()) {
        qCritical() << "HearconManager::readOutput - cannot access results";
        emit error("Cannot access results. Please contact support.");
        return;
    }

    auto test = qSharedPointerCast<HearconTest>(m_test);
    test->fromJsonFile(m_readerOutputPath);

    //QString data = JsonSettings::serializeJson(test->toJsonObject(), true);

    //QFile::remove("C:/Users/hoarea/cypress-builds/Cypress/hearcon/output.json");

    //qDebug() << "write" << data;
    //QFile file("C:/Users/hoarea/cypress-builds/Cypress/watch_bp/output.json");
    //if (file.open(QFile::WriteOnly | QFile::Text)) {
    //    QTextStream stream(&file);
    //    stream << data;
    //    file.close();
    //}

    //if (test->isValid()) {
    //    emit canFinish();
    //}
    //else {
    //    qCritical() << "HearconManager::readOutput - results were invalid";
    //    emit error("Results were invalid. Please contact support.");
    //}
}

void HearconManager::finish()
{
    qDebug() << "HearconManager::finish";
}

#include "ecg_manager.h"
#include "auxiliary/network_utils.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QFileDialog>
#include <QJsonObject>
#include <QSettings>
#include <QStandardItemModel>
#include <QJsonDocument>
#include <QMessageBox>

ECGManager::ECGManager(QSharedPointer<ECGSession> session)
    : ManagerBase(session)
{
    // absolute path to exe
    m_runnableName = CypressSettings::readSetting("ecg/runnableName").toString();

    // absolute path to working directory
    m_workingPath = CypressSettings::readSetting("ecg/workingPath").toString();

    // absolute path to export directory
    m_exportPath = CypressSettings::readSetting("ecg/exportPath").toString();

    // xml file output
    m_outputFile = m_exportPath + "/" + session->getBarcode() + ".xml";

    m_test.reset(new ECGTest);

    if (m_debug) {
        qDebug() << "ECGManager";
        qDebug() << session->getSessionId();
        qDebug() << session->getBarcode();
        qDebug() << session->getInterviewer();
        qDebug() << session->getInputData();
        qDebug() << m_runnableName;
        qDebug() << m_workingPath;
        qDebug() << m_exportPath;
        qDebug() << m_outputFile;
    }
}

bool ECGManager::isInstalled()
{
    bool isDebugMode = CypressSettings::isDebugMode();
    bool isSimMode = CypressSettings::isSimMode();

    if (isSimMode)
        return true;

    QString runnableName = CypressSettings::readSetting("ecg/runnableName").toString();
    QString workingPath = CypressSettings::readSetting("ecg/workingPath").toString();
    QString exportPath = CypressSettings::readSetting("ecg/exportPath").toString();

    if (runnableName.isNull() || runnableName.isEmpty()) {
        if (isDebugMode) {
            qDebug() << "ECGManager::isInstalled: runnableName is not defined";
        }

        return false;
    }

    if (workingPath.isNull() || workingPath.isEmpty()) {
        if (isDebugMode) {
            qDebug() << "ECGManager::isInstalled: workingPath is not defined";
        }

        return false;
    }

    if (exportPath.isNull() || exportPath.isEmpty()) {
        if (isDebugMode) {
            qDebug() << "ECGManager::isInstalled: exportPath is not defined";
        }

        return false;
    }

    QFileInfo runnableInfo(runnableName);
    if (!runnableInfo.exists()) {
        if (isDebugMode) {
            qDebug() << "ECGManager::isInstalled: runnableName does not exist";
        }

        return false;
    }

    if (!runnableInfo.isExecutable()) {
        if (isDebugMode) {
            qDebug() << "ECGManager::isInstalled: runnableName is not executable";
        }

        return false;
    }

    QDir workingInfo(workingPath);
    if (!workingInfo.exists()) {
        if (isDebugMode) {
            qDebug() << "ECGManager::isInstalled: working info does not exist";
        }

        return false;
    }

    QDir exportInfo(exportPath);
    if (!exportInfo.exists()) {
        if (isDebugMode) {
            qDebug() << "ECGManager::isInstalled: export info does not exist";
        }
        return false;
    }

    return true;
}

bool ECGManager::start()
{
    if (m_debug)
        qDebug() << "ECGManager::start" << m_session->getSessionId() << m_session->getStatus();

    clearData();

    if (!setUp()) {
        emit error("Could not setup ECG");
    }

    measure();

    return true;
}

void ECGManager::configureProcess()
{
    if (m_debug)
        qDebug() << "ECGManager::configureProcess";

    // connect signals and slots to QProcess one time only
    //
    connect(&m_process, &QProcess::started, this, [this]() {
        if (m_debug)
            qDebug() << "process started: " << m_process.arguments().join(" ");
    });

    connect(&m_process,
            QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this,
            &ECGManager::readOutput);

    connect(&m_process, &QProcess::errorOccurred, this, [this](QProcess::ProcessError error) {
        QStringList s = QVariant::fromValue(error).toString().split(QRegExp("(?=[A-Z])"),
                                                                    Qt::SkipEmptyParts);
        if (m_debug)
            qDebug() << "ERROR: process error occured: " << s.join(" ").toLower();
    });

    connect(&m_process, &QProcess::stateChanged, this, [this](QProcess::ProcessState state) {
        QStringList s = QVariant::fromValue(state).toString().split(QRegExp("(?=[A-Z])"),
                                                                    Qt::SkipEmptyParts);
        if (m_debug)
            qDebug() << "process state: " << s.join(" ").toLower();
    });

    QDir workingDir(m_workingPath);
    if (!workingDir.exists()) {
        if (m_debug) {
            qDebug() << "ECGManager::configureProcess -- "
                     << "workingDir does not exist";
        }

        return;
    }

    QDir exportDir(m_exportPath);
    if (!exportDir.exists()) {
        if (m_debug) {
            qDebug() << "ECGManager::configureProcess -- "
                     << "exportDir does not exist";
        }

        return;
    }

    QFileInfo exeInfo(m_runnableName);
    if (!exeInfo.exists()) {
        if (m_debug) {
            qDebug() << "ECGManager::configureProcess -- could not find exe at " << m_runnableName;
        }

        return;
    }

    if (!exeInfo.isExecutable()) {
        if (m_debug) {
            qDebug() << "ECGManager::configureProcess -- exe is not executable at "
                     << m_runnableName;
        }

        return;
    }

    QString backupPath = QDir::cleanPath(
        QString("%1%2%3").arg(m_workingPath, QDir::separator(), INIT_PATH));
    QDir backupDir(backupPath);
    if (backupDir.exists()) { // remove old backup
        if (!backupDir.removeRecursively()) {
            qDebug() << "failed to configure and remove backup directory" << backupPath;
        }
    }

    m_process.setProcessChannelMode(QProcess::ForwardedChannels);
    m_process.setProgram(m_runnableName);

    if (!backupData()) {
        QMessageBox::critical(nullptr, "Error", "Could not backup CardioSoft data");
    }

}

void ECGManager::measure()
{
    if (m_debug)
        qDebug() << "ECGManager::measure";

    clearData();

    if (CypressSettings::isSimMode()) {
        m_test->simulate();
        finish();
    }

    if (m_process.state() != QProcess::NotRunning) {
        emit error("Program is already running");
        return;
    }

    m_process.start();

    if (!m_process.waitForStarted()) {
        emit error("Could not start process");
        return;
    }
}

void ECGManager::readOutput()
{
    if (m_debug)
        qDebug() << "ECGManager::readOutput";

    QSharedPointer<ECGTest> test = qSharedPointerCast<ECGTest>(m_test);

    if (QProcess::NormalExit != m_process.exitStatus()) {
        emit error("CardioSoft error, cannot read output");
        return;
    }


    if(!QFileInfo::exists(m_outputFile))
    {
        emit error("Cannot find the measurement file");
        return;
    }

    if (m_debug)
        qDebug() << "found ecg output file" << m_outputFile;

    test->fromFile(m_outputFile);
    finish();
}

bool ECGManager::clearData()
{
    if (m_debug)
        qDebug() << "ECGManager::clearData";

    m_test->reset();

    return true;
}

bool ECGManager::backupData()
{
    if (m_debug)
        qDebug() << "ECGManager::deleteDeviceData";

    // get or create backup path
    QString backupPath = QDir::cleanPath(
        QString("%1%2%3").arg(m_workingPath, QDir::separator(), INIT_PATH));

    QDir backupDir(backupPath);

    if (!backupDir.exists()) {
        if (!backupDir.mkdir(backupPath)) {
            if (m_debug)
                qCritical() << "unable to create backup directory" << backupPath;

            return false;
        }
    }

    QString databasePath = QDir::cleanPath(
        QString("%1%2%3").arg(m_workingPath, QDir::separator(), DATABASE_PATH));
    QDir databaseDir(databasePath);

    databaseDir.setNameFilters(QStringList() << "*.BTR");
    databaseDir.setFilter(QDir::Files);

    // copy each database file to the backup folder
    QFileInfoList list = databaseDir.entryInfoList();
    if(!list.isEmpty())
    {
        foreach (const auto databaseFile, list) {
            QString toFile = QDir(backupPath).filePath(databaseFile.fileName());
            QFile::copy(databaseFile.absoluteFilePath(), toFile);
        }
    }

    // remove previous XML output from Cardiosoft export
    //
    QString xmlFile = QString("%1.xml").arg(m_session->getBarcode());
    QString outputFilePath = QDir(m_exportPath).filePath(xmlFile);

    QFile outputXmlFile(outputFilePath);
    if (outputXmlFile.exists() && !outputXmlFile.remove()) {
        if (m_debug) {
            qDebug() << "ECGManager::cleanUp - could not delete Cardiosoft XML output file"
                     << m_outputFile;
        }

        return false;
    }

    return true;
}

// Set up device
bool ECGManager::setUp()
{
    if (m_debug)
        qDebug() << "ECGManager::setUp";

    configureProcess();

    return backupData();
}

// Clean up the device for next time
bool ECGManager::cleanUp()
{
    if(QProcess::NotRunning != m_process.state())
        m_process.kill();

    m_test->reset();

    return restoreBackup();
}

bool ECGManager::restoreBackup()
{
    if (m_debug)
        qDebug() << "ECGManager::cleanUp";

    // get backup directory
    QString backupPath = QDir::cleanPath(
        QString("%1%2%3").arg(m_workingPath, QDir::separator(), INIT_PATH));
    QDir backupDir(backupPath);

    if (!backupDir.exists()) {
        if (m_debug)
            qDebug() << "ECGManager::cleanUp - could not find backup dir";

        return false;
    }

    if (m_debug)
        qDebug() << "ECGManager::cleanUp - " << backupDir.absolutePath();

    backupDir.setNameFilters(QStringList() << "*.BTR");
    backupDir.setFilter(QDir::Files);

    // get database directory
    //
    QString databasePath = QDir::cleanPath(
        QString("%1%2%3").arg(m_workingPath, QDir::separator(), DATABASE_PATH));
    QDir databaseDir(databasePath);

    if (!databaseDir.exists()) {
        if (m_debug)
            qDebug() << "ECGManager::cleanUp - could not find database dir";

        return false;
    }

    if (m_debug)
        qDebug() << "ECGManager::cleanUp - " << databasePath;

    // for each backup file, restore to the database folder
    //
    QFileInfoList list = backupDir.entryInfoList();
    if (!list.isEmpty()) {
        foreach (const auto backupFile, list) {
            QString toFile = QDir(databasePath).filePath(backupFile.fileName());
            if (!QFile::copy(backupFile.absoluteFilePath(), toFile)) {
                if (m_debug) {
                    qDebug() << "ECGManager::cleanUp - could not restore backup file "
                             << backupFile.absoluteFilePath();
                }
            }
        }
    }

    // remove xml output from Cardiosoft export
    //
    QString xmlFile = QString("%1.xml").arg(m_session->getBarcode());
    QString outputFilePath = QDir(m_exportPath).filePath(xmlFile);

    QFile outputXmlFile(outputFilePath);
    if (outputXmlFile.exists() && !outputXmlFile.remove()) {
        if (m_debug)
            qDebug() << "ECGManager::cleanUp - could not delete Cardiosoft XML output file"
                     << m_outputFile;

        return false;
    }

    return true;
}

#include "ecg_manager.h"
#include "auxiliary/network_utils.h"
#include "auxiliary/file_utils.h"

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
    const bool isDebugMode = CypressSettings::isDebugMode();
    const bool isSimMode = CypressSettings::isSimMode();

    if (isSimMode)
        return true;

    const QString runnableName = CypressSettings::readSetting("ecg/runnableName").toString();
    const QString workingPath = CypressSettings::readSetting("ecg/workingPath").toString();
    const QString exportPath = CypressSettings::readSetting("ecg/exportPath").toString();

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

    const QFileInfo runnableInfo(runnableName);
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

    const QDir workingInfo(workingPath);
    if (!workingInfo.exists()) {
        if (isDebugMode) {
            qDebug() << "ECGManager::isInstalled: working info does not exist";
        }

        return false;
    }

    const QDir exportInfo(exportPath);
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

    QString backupPath = QDir::cleanPath(
        QString("%1%2%3").arg(m_workingPath,
                              QDir::separator(),
                              INIT_PATH));
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


void ECGManager::finish() {
    const int answer_id = m_session->getAnswerId();
    const QString host = CypressSettings::getPineHost();
    const QString endpoint = CypressSettings::getPineEndpoint();

    // Save the xml and pdf files
    const QFileInfo xmlFile(m_outputFile);
    if (!xmlFile.exists()) {
        if (m_debug)
            qDebug() << "xml file does not exist at: " << m_outputFile;

        emit error("Ecg.xml file does not exist");
    }

    QJsonObject filesJson {};
    filesJson.insert("Ecg_xml", FileUtils::getHumanReadableFileSize(xmlFile.absoluteFilePath()));

    bool ok = NetworkUtils::sendHTTPSRequest(
        Poco::Net::HTTPRequest::HTTP_PATCH,
        (host + endpoint + QString::number(answer_id) + "?filename=" + "Ecg.xml").toStdString(),
        "application/octet-stream",
        FileUtils::readFile(xmlFile.absoluteFilePath())
    );

    if (!ok) {
        qDebug() << "could not send xmlFile" << xmlFile.absoluteFilePath();
        emit error("Could not submit results");
    }

    QJsonObject testJson = m_test->toJsonObject();
    testJson.insert("session", m_session->getJsonObject());
    testJson.insert("files", filesJson);

    QJsonObject responseJson {};
    responseJson.insert("value", testJson);

    QJsonDocument jsonDoc(responseJson);
    QByteArray serializedData = jsonDoc.toJson();

    QString answerUrl = CypressSettings::getAnswerUrl(answer_id);
    ok = NetworkUtils::sendHTTPSRequest(
        Poco::Net::HTTPRequest::HTTP_PATCH,
        answerUrl.toStdString(),
        "application/json",
        serializedData
    );

    if (!ok) {
        qDebug() << "could not send data";
        emit error("Could not submit results");
    }
    else {
        emit success("Success. You may now close this window");
    }
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

    const QString databasePath = QDir::cleanPath(
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
    const QString xmlFile = QString("%1.xml").arg(m_session->getBarcode());
    const QString outputFilePath = QDir(m_exportPath).filePath(xmlFile);

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
    const QString backupPath = QDir::cleanPath(
        QString("%1%2%3").arg(m_workingPath, QDir::separator(), INIT_PATH));

    QDir backupDir(backupPath);
    backupDir.setNameFilters(QStringList() << "*.BTR");
    backupDir.setFilter(QDir::Files);

    if (!backupDir.exists()) {
        if (m_debug)
            qDebug() << "ECGManager::cleanUp - could not find backup dir";

        return false;
    }

    if (m_debug)
        qDebug() << "ECGManager::cleanUp - " << backupDir.absolutePath();

    // get database directory
    //
    const QString databasePath = QDir::cleanPath(
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
    const QFileInfoList list = backupDir.entryInfoList();
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
    const QString xmlFile = QString("%1.xml").arg(m_session->getBarcode());
    const QString outputFilePath = QDir(m_exportPath).filePath(xmlFile);

    QFile outputXmlFile(outputFilePath);
    if (outputXmlFile.exists() && !outputXmlFile.remove()) {
        if (m_debug)
            qDebug() << "ECGManager::cleanUp - could not delete Cardiosoft XML output file"
                     << m_outputFile;

        return false;
    }

    return true;
}

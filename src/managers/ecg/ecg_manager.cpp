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


DeviceConfig ECGManager::config {{
    // absolute path to exe
    {"runnableName", {"ecg/runnableName", Exe }},

    // absolute path to working directory
    {"workingPath",  {"ecg/workingPath",  Dir }},

    // absolute path to export directory
    {"exportPath",   {"ecg/exportPath",   Dir }},
}};

ECGManager::ECGManager(QSharedPointer<ECGSession> session)
    : ManagerBase(session)
{
    m_runnableName = config.getSetting("runnableName");
    m_workingPath = config.getSetting("workingPath");
    m_exportPath = config.getSetting("exportPath");

    // xml file output
    m_outputFile = m_exportPath + "/" + session->getBarcode() + ".xml";

    m_test.reset(new ECGTest);
}

bool ECGManager::start()
{
    qInfo() << "ECGManager::start";

    clearData();

    if (!setUp())
        emit error("Could not setup ECG");

    measure();

    return true;
}

void ECGManager::configureProcess()
{
    qInfo() << "ECGManager::configureProcess";

    // connect signals and slots to QProcess one time only
    //
    connect(&m_process, &QProcess::started, this, [this]() {
        qDebug() << "process started: " << m_process.arguments().join(" ");
    });

    connect(&m_process,
            QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this,
            &ECGManager::readOutput);

    connect(&m_process, &QProcess::errorOccurred, this, [](QProcess::ProcessError error) {
        QStringList s = QVariant::fromValue(error).toString().split(QRegExp("(?=[A-Z])"),
                                                                    Qt::SkipEmptyParts);
        qWarning() << "ECGManager::configureProcess: process error occured: " << s.join(" ").toLower();
    });

    connect(&m_process, &QProcess::stateChanged, this, [](QProcess::ProcessState state) {
        QStringList s = QVariant::fromValue(state).toString().split(QRegExp("(?=[A-Z])"),
                                                                    Qt::SkipEmptyParts);
        qInfo() << "ECGManager::configureProcess: process state: " << s.join(" ").toLower();
    });

    QString backupPath = QDir::cleanPath(
        QString("%1%2%3").arg(m_workingPath,
                              QDir::separator(),
                              INIT_PATH));
    QDir backupDir(backupPath);
    if (backupDir.exists()) { // remove old backup
        if (!backupDir.removeRecursively()) {
            qWarning() << "ECGManager::configureProcess: failed to configure and remove backup directory" << backupPath;
        }
    }

    m_process.setProcessChannelMode(QProcess::ForwardedChannels);
    m_process.setProgram(m_runnableName);

    if (!backupData()) {
        qWarning() << "ECGManager::configureProcess: Could not backup CardioSoft data";
        QMessageBox::critical(nullptr, "Error", "Could not backup CardioSoft data");
    }
}

void ECGManager::measure()
{
    qInfo() << "ECGManager::measure";

    clearData();

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
    qInfo() << "ECGManager::readOutput";

    auto test = qSharedPointerCast<ECGTest>(m_test);
    if (QProcess::NormalExit != m_process.exitStatus()) {
        emit error("CardioSoft error, cannot read output");
        return;
    }

    if(!QFileInfo::exists(m_outputFile))
    {
        emit error("Cannot find the measurement file");
        return;
    }

    test->fromFile(m_outputFile);

    finish();
}


void ECGManager::finish() {
    const int answer_id = m_session->getAnswerId();
    const QString pineOrigin = m_session->getOrigin();
    const QString answerUrl = pineOrigin + "/answer/" + QString::number(answer_id);

    // Save the xml and pdf files
    const QFileInfo xmlFile(m_outputFile);
    if (!xmlFile.exists()) {
        qCritical() << "ECGManager::finish: xml file does not exist at: " << m_outputFile;
        emit error("Ecg.xml file does not exist");
    }

    QJsonObject filesJson {};
    filesJson.insert("Ecg_xml", FileUtils::getHumanReadableFileSize(xmlFile.absoluteFilePath()));

    bool ok = NetworkUtils::sendHTTPSRequest(
        Poco::Net::HTTPRequest::HTTP_PATCH,
        (answerUrl + "?filename=" + "Ecg.xml").toStdString(),
        "application/octet-stream",
        FileUtils::readFile(xmlFile.absoluteFilePath())
    );

    if (!ok) {
        qCritical() << "ECGManager::finish: could not send xmlFile" << xmlFile.absoluteFilePath();
        emit error("Could not submit results");
    }

    QJsonObject testJson = m_test->toJsonObject();
    testJson.insert("session", m_session->getJsonObject());
    testJson.insert("files", filesJson);

    QJsonObject responseJson {};
    responseJson.insert("value", testJson);

    const QJsonDocument jsonDoc(responseJson);
    const QByteArray serializedData = jsonDoc.toJson();

    ok = NetworkUtils::sendHTTPSRequest(
        Poco::Net::HTTPRequest::HTTP_PATCH,
        answerUrl.toStdString(),
        "application/json",
        serializedData
    );

    cleanUp();

    if (!ok) {
        qCritical() << "ECGManager::finish: could not send data";
        emit error("Could not submit results");
    }
    else {
        emit success("Success. You may now close this window");
    }
}


bool ECGManager::clearData()
{
    qInfo() << "ECGManager::clearData";

    m_test->reset();

    return true;
}

bool ECGManager::backupData()
{
    qInfo() << "ECGManager::backupData";

    // get or create backup path
    QString backupPath = QDir::cleanPath(
        QString("%1%2%3").arg(m_workingPath, QDir::separator(), INIT_PATH));

    QDir backupDir(backupPath);

    if (!backupDir.exists()) {
        if (!backupDir.mkdir(backupPath)) {
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
        qCritical() << "ECGManager::backupData - could not delete Cardiosoft XML output file"
             << m_outputFile;

        return false;
    }

    return true;
}

// Set up device
bool ECGManager::setUp()
{
    qInfo() << "ECGManager::setUp";
    configureProcess();
    return backupData();
}

// Clean up the device for next time
bool ECGManager::cleanUp()
{
    qInfo() << "ECGManager::cleanUp";
    if(QProcess::NotRunning != m_process.state())
        m_process.close();

    m_test->reset();

    return restoreBackup();
}

bool ECGManager::restoreBackup()
{
    qInfo() << "ECGManager::restoreBackup";

    // get backup directory
    const QString backupPath = QDir::cleanPath(
        QString("%1%2%3").arg(m_workingPath, QDir::separator(), INIT_PATH));

    QDir backupDir(backupPath);
    backupDir.setNameFilters(QStringList() << "*.BTR");
    backupDir.setFilter(QDir::Files);

    if (!backupDir.exists()) {
        qCritical() << "ECGManager::cleanUp: could not find backup dir";
        return false;
    }

    // get database directory
    //
    const QString databasePath = QDir::cleanPath(
        QString("%1%2%3").arg(m_workingPath, QDir::separator(), DATABASE_PATH));

    QDir databaseDir(databasePath);
    if (!databaseDir.exists()) {
        qCritical() << "ECGManager::cleanUp: could not find database dir";
        return false;
    }

    // for each backup file, restore to the database folder
    //
    const QFileInfoList list = backupDir.entryInfoList();
    if (!list.isEmpty()) {
        foreach (const auto backupFile, list) {
            QString toFile = QDir(databasePath).filePath(backupFile.fileName());
            if (!QFile::copy(backupFile.absoluteFilePath(), toFile)) {
                qCritical() << "ECGManager::cleanUp - could not restore backup file "
                     << backupFile.absoluteFilePath();
            }
        }
    }

    // remove xml output from Cardiosoft export
    //
    const QString xmlFile = QString("%1.xml").arg(m_session->getBarcode());
    const QString outputFilePath = QDir(m_exportPath).filePath(xmlFile);

    QFile outputXmlFile(outputFilePath);
    if (outputXmlFile.exists() && !outputXmlFile.remove()) {
        qCritical() << "ECGManager::cleanUp - could not delete Cardiosoft XML output file"
             << m_outputFile;

        return false;
    }

    return true;
}

#include "easyone_manager.h"

#include "data/spirometer/tests/easyone_test.h"

#include "managers/emr/emr_plugin_writer.h"
#include "auxiliary/windows_util.h"
#include "auxiliary/file_utils.h"

#include <QFile>
#include <QFileInfo>
#include <QDir>

#include <QMessageBox>

EasyoneConnectManager::EasyoneConnectManager(QSharedPointer<EasyoneConnectSession> session): ManagerBase(session)
{
    // Full path to EasyWarePro.exe
    m_processName = CypressSettings::readSetting("easyone/processName").toString();
    m_runnableName = CypressSettings::readSetting("easyone/runnableName").toString();
    m_runnablePath = CypressSettings::readSetting("easyone/runnablePath").toString(); // path to EasyWarePro.exe directory
    m_exchangePath = CypressSettings::readSetting("easyone/exchangePath").toString();
    m_databasePath = CypressSettings::readSetting("easyone/databasePath").toString();

    m_backupPath = CypressSettings::readSetting("easyone/backupPath").toString();

    m_inFileName = CypressSettings::readSetting("easyone/inFileName").toString();
    m_outFileName = CypressSettings::readSetting("easyone/outFileName").toString();

    qInfo() << "EasyoneConnectManager";
    qInfo() << session->getSessionId();
    qInfo() << session->getBarcode();
    qInfo() << session->getInterviewer();
    qInfo() << session->getInputData();

    qInfo() << m_processName;
    qInfo() << m_runnableName;
    qInfo() << m_runnablePath;
    qInfo() << m_exchangePath;
    qInfo() << m_databasePath;

    qInfo() << m_inFileName;
    qInfo() << m_outFileName;

    m_test.reset(new EasyoneTest);
}

bool EasyoneConnectManager::isInstalled()
{
    qInfo() << "EasyoneConnectManager::isInstalled";

    const QString processName = CypressSettings::readSetting("easyone/processName").toString();
    const QString runnableName = CypressSettings::readSetting("easyone/runnableName").toString();
    const QString runnablePath = CypressSettings::readSetting("easyone/runnablePath").toString();
    const QString exchangePath = CypressSettings::readSetting("easyone/exchangePath").toString();
    const QString databasePath = CypressSettings::readSetting("easyone/databasePath").toString();
    const QString backupPath = CypressSettings::readSetting("easyone/backupPath").toString();

    const QString inFileName = CypressSettings::readSetting("easyone/inFileName").toString();
    const QString outFileName = CypressSettings::readSetting("easyone/outFileName").toString();

    if (processName.isEmpty() || processName.isNull()) {
        qInfo() << "EasyoneConnectManager::isInstalled: processName is not defined";
        return false;
    }

    if (runnableName.isEmpty() || runnableName.isNull()) {
        qInfo() << "EasyoneConnectManager::isInstalled: runnableName is not defined";
        return false;
    }

    if (runnablePath.isEmpty() || runnablePath.isNull()) {
        qInfo() << "EasyoneConnectManager::isInstalled: runnablePath is not defined";
        return false;
    }

    if (exchangePath.isEmpty() || exchangePath.isNull()) {
        qInfo() << "EasyoneConnectManager::isInstalled: exchangePath is not defined";
        return false;
    }

    if (databasePath.isEmpty() || databasePath.isNull()) {
        qInfo() << "EasyoneConnectManager::isInstalled: databasePath is not defined";
        return false;
    }

    if (backupPath.isEmpty() || backupPath.isNull()) {
        qInfo() << "EasyoneConnectManager::isInstalled: backupPath is not defined";
        return false;
    }

    if (inFileName.isEmpty() || inFileName.isNull()) {
        qInfo() << "EasyoneConnectManager::isInstalled: inFileName is not defined";
        return false;
    }

    if (outFileName.isEmpty() || outFileName.isNull()) {
        qInfo() << "EasyoneConnectManager::isInstalled: outFileName is not defined";
        return false;
    }

    const QFileInfo runnableInfo(runnableName);
    if (!runnableInfo.exists()) {
        qInfo() << "EasyoneConnectManager::isInstalled: exe does not exist at" << runnableName;
        return false;
    }
    if (!runnableInfo.isExecutable()) {
        qInfo() << "EasyoneConnectManager::isInstalled: exe is not executable at" << runnableName;
        return false;
    }

    const QFileInfo runnableDir(runnablePath);
    if (!runnableDir.isDir()) {
        qInfo() << "EasyoneConnectManager::isInstalled: directory does not exist at" << runnablePath;
        return false;
    }
    if (!runnableDir.isReadable()) {
        qInfo() << "EasyoneConnectManager::isInstalled: directory is missing read permissions at" << runnableDir;
        return false;
    }

    return true;
}

bool EasyoneConnectManager::start()
{
    qDebug() << "EasyoneConnectManager::start";

    // Make sure application isn't running
    qDebug() << "EasyoneConnectManager::start - check if running";
    if (WindowsUtil::isProcessRunning(m_processName.toStdWString())) {
        qCritical() << "EasyoneConnectManager::start - error, process is already running";
        QMessageBox::critical(nullptr, "Error", "EasyOne Connect is already running. Please close the application and try again.");
        return false;
    }

    // Clear out database
    qDebug() << "EasyoneConnectManager::start - clearing database.sqlite";
    QFile::remove(m_databasePath + "/database.sqlite");

    qDebug() << "EasyoneConnectManager::start - clearing EasyOneConnectOptions.mdb";
    QFile::remove(m_databasePath + "/EasyOneConnectOptions.mdb");

    // Restore database from backup
    qDebug() << "EasyoneConnectManager::start - restoring database.sqlite backup";
    qDebug() << m_backupPath + "/database.sqlite" << " " << m_databasePath + "/database.sqlite";
    if (!FileUtils::copyFile(m_backupPath + "/database.sqlite", m_databasePath + "/database.sqlite")) {
        qCritical() << "EasyoneConnectManager::start - could not remove options.mdb";
        QMessageBox::critical(nullptr, "Error", "Something went wrong");
        return false;
    }

    qDebug() << "EasyoneConnectManager::start - restoring EasyOneConnectOptions.mdb backup";
    if (!FileUtils::copyFile(m_backupPath + "/EasyOneConnectOptions.mdb", m_databasePath + "/EasyOneConnectOptions.mdb")) {
        qCritical() << "EasyoneConnectManager::start - could not restore options.mdb";
        QMessageBox::critical(nullptr, "Error", "Something went wrong");
        return false;
    }

    // Write EMR plugin
    qDebug() << "EasyoneConnectManager::start - writing plugin";

    EMRPluginWriter writer;
    const QDir xmlPath(m_exchangePath);

    writer.setInputData(m_session->getInputData().toVariantMap());
    writer.write(xmlPath.filePath(m_inFileName), m_session->getBarcode());

    // Launch application
    qDebug() << "EasyoneConnectManager::start - launching EasyOne Connect";
    configureProcess();
    m_process.start();

    return true;
}

void EasyoneConnectManager::measure()
{
    qDebug() << "EasyoneConnectManager::measure";
}

void EasyoneConnectManager::finish()
{
    qDebug() << "EasyoneConnectManager::finish";
}

void EasyoneConnectManager::readOutput()
{
    qDebug() << "EasyoneConnectManager::readOutput";
}

QString EasyoneConnectManager::getEMRInXmlName() const { return QString("%1/%2").arg(m_exchangePath, m_inFileName); }

QString EasyoneConnectManager::getEMROutXmlName() const { return QString("%1/%2").arg(m_exchangePath, m_outFileName); }

bool EasyoneConnectManager::clearData()
{
    qDebug() << "EasyoneConnectManager::clearData";
    return true;
}

bool EasyoneConnectManager::setUp()
{
    qDebug() << "EasyoneConnectManager::setUp";
    return true;
}

bool EasyoneConnectManager::cleanUp()
{
    qDebug() << "EasyoneConnectManager::cleanUp";
    return true;
}

void EasyoneConnectManager::configureProcess()
{
    m_process.setProgram(m_runnableName);
    connect(&m_process, &QProcess::started, this, [this]() {
        qDebug() << "EasyoneConnectManager::process started: " << m_process.arguments().join(" ");
    });

    connect(&m_process,
            QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this,
            &EasyoneConnectManager::readOutput);

    connect(&m_process, &QProcess::errorOccurred, this, [=](QProcess::ProcessError error) {
        QStringList s = QVariant::fromValue(error).toString().split(QRegExp("(?=[A-Z])"),
                                                                    Qt::SkipEmptyParts);
        qDebug() << "EasyoneConnectManager::process error occured: " << s.join(" ").toLower();
    });

    connect(&m_process, &QProcess::stateChanged, this, [=](QProcess::ProcessState state) {
        QStringList s = QVariant::fromValue(state).toString().split(QRegExp("(?=[A-Z])"),
                                                                    Qt::SkipEmptyParts);
        qDebug() << "EasyoneConnectManager::process state: " << s.join(" ").toLower();
    });
}

QString EasyoneConnectManager::getOutputPdfPath() const
{
    qInfo() << "EasyoneConnectManager::getOutputPdfPath";

    auto test = qSharedPointerCast<EasyoneTest>(m_test);
    if (test->hasMetaData("pdf_report_path"))
        return test->getMetaDataAsString("pdf_report_path");

    return "";
}

bool EasyoneConnectManager::outputPdfExists() const
{
    qInfo() << "EasyoneConnectManager::outputPdfExists";

    const QString outPdfPath = getOutputPdfPath();
    if (outPdfPath.isEmpty() || outPdfPath.isNull())
        return false;

    return QFileInfo::exists(outPdfPath);
}

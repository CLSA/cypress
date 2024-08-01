#include "easyone_manager.h"

#include "data/spirometer/tests/easyone_test.h"

#include "managers/emr/emr_plugin_writer.h"
#include "auxiliary/windows_util.h"
#include "auxiliary/file_utils.h"

#include <QFile>
#include <QFileInfo>
#include <QDir>

#include <QMessageBox>

DeviceConfig EasyoneConnectManager::config {{
    {"processName",  {"easyone/processName",  NonEmptyString }},
    {"runnableName", {"easyone/runnableName", Exe  			 }},
    {"runnablePath", {"easyone/runnablePath", Dir            }},
    {"exchangePath", {"easyone/exchangePath", Dir            }},
    {"databasePath", {"easyone/databasePath", Dir            }},
    {"backupPath",   {"easyone/backupPath",   Dir            }},
    {"inFileName",   {"easyone/inFileName",   NonEmptyString }},
    {"outFileName",  {"easyone/outFileName",  NonEmptyString }},
}};

EasyoneConnectManager::EasyoneConnectManager(QSharedPointer<EasyoneConnectSession> session): ManagerBase(session)
{
    // Full path to EasyWarePro.exe
    m_processName = config.getSetting("processName");
    m_runnableName = config.getSetting("runnableName");
    m_runnablePath = config.getSetting("runnablePath"); // path to EasyWarePro.exe directory
    m_exchangePath = config.getSetting("exchangePath");
    m_databasePath = config.getSetting("databasePath");
    m_backupPath = config.getSetting("backupPath");
    m_inFileName = config.getSetting("inFileName");
    m_outFileName = config.getSetting("outFileName");

    m_test.reset(new EasyoneTest);
}

void EasyoneConnectManager::configureProcess()
{
    m_process.setProgram(m_runnableName);

    connect(&m_process, &QProcess::started, this, [this]() {
        qDebug() << "EasyoneConnectManager::process started: " << m_process.arguments().join(" ");
    });

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

    connect(&m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this,
            &EasyoneConnectManager::readOutput);
}

bool EasyoneConnectManager::start()
{
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

void EasyoneConnectManager::readOutput()
{
    qDebug() << "EasyoneConnectManager::readOutput";
}

void EasyoneConnectManager::finish()
{
    qDebug() << "EasyoneConnectManager::finish";
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

QString EasyoneConnectManager::getEMRInXmlName() const { return QString("%1/%2").arg(m_exchangePath, m_inFileName); }

QString EasyoneConnectManager::getEMROutXmlName() const { return QString("%1/%2").arg(m_exchangePath, m_outFileName); }

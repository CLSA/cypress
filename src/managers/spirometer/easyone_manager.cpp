#include "easyone_manager.h"
#include "managers/emr/emr_plugin_writer.h"
#include "data/spirometer/tests/easyone_test.h"

#include "auxiliary/windows_util.h"
#include "auxiliary/network_utils.h"
#include "auxiliary/file_utils.h"

#include "widgets/select_ethnicity_dialog.h"

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
    m_processName  = config.getSetting("processName");
    m_runnableName = config.getSetting("runnableName");
    m_runnablePath = config.getSetting("runnablePath"); // path to EasyWarePro.exe directory
    m_exchangePath = config.getSetting("exchangePath");
    m_databasePath = config.getSetting("databasePath");
    m_backupPath   = config.getSetting("backupPath");
    m_inFileName   = config.getSetting("inFileName");
    m_outFileName  = config.getSetting("outFileName");

    m_test.reset(new EasyoneTest);
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

    if (!restoreDatabase())
        return false;

    SelectEthnicityDialog dialog;
    if (dialog.exec() == QDialog::Accepted) {
        QString ethnicity = dialog.getSelectedValue();
        qDebug() << "ethnicity selected: " << ethnicity;
        m_test->addMetaData("ethnicity", ethnicity);
    }
    else {
        qDebug() << "No ethnicity selected";
        return false;
    }

    if (!setUp()) {
        emit error("Something went wrong during setup");
        return false;
    }

    // Write EMR plugin
    qDebug() << "EasyoneConnectManager::start - writing plugin";
    if (!writeEMRRequest())
        return false;

    // Launch application
    qDebug() << "EasyoneConnectManager::start - launching EasyOne Connect";
    if (!configureProcess())
        return false;

    m_process.start();

    return true;
}

void EasyoneConnectManager::readOutput()
{
    qDebug() << "EasyoneConnectManager::readOutput";

    if (QProcess::NormalExit != m_process.exitStatus()) {
        emit error("Process failed to finish correctly, cannot read output");
        return;
    }

    if (!QFileInfo::exists(getEMROutXmlName())) {
        emit error("Cannot find the output file");
        return;
    }

    auto test = qSharedPointerCast<EasyoneTest>(m_test);
    test->fromFile(getEMROutXmlName());

    finish();
}

void EasyoneConnectManager::finish()
{
    QList<QJsonObject> filePaths = {
        { { "path", getEMROutXmlName() }, { "name", "data.xml" } },
        { { "path", getOutputPdfPath() }, { "name", "report.pdf" } },
    };

    m_test->setFiles(filePaths);

    ManagerBase::finish();
}

bool EasyoneConnectManager::restoreDatabase()
{
    // Clear out database
    //QFile::remove(m_databasePath + "/EasyOneConnect.sqlite");
    //QFile::remove(m_databasePath + "/EasyOneConnectOptions.mdb");

    // Restore database from backup
    //if (!FileUtils::copyFile(m_backupPath + "/EasyOneConnect.sqlite", m_databasePath + "/EasyOneConnect.sqlite")) {
    //    qCritical() << "EasyoneConnectManager::restoreDatabase - could not copy EasyOneConnect.sqlite";
    //    QMessageBox::critical(nullptr, "Error", "Something went wrong");
    //    return false;
    //}

    //if (!FileUtils::copyFile(m_backupPath + "/EasyOneConnectOptions.mdb", m_databasePath + "/EasyOneConnectOptions.mdb")) {
    //    qCritical() << "EasyoneConnectManager::restoreDatabase - could not copy EasyOneConnectOptions.mdb";
    //    QMessageBox::critical(nullptr, "Error", "Something went wrong");
    //    return false;
    //}

    return true;
}

bool EasyoneConnectManager::writeEMRRequest()
{
    EMRPluginWriter writer;
    const QDir xmlPath(m_exchangePath);

    QJsonObject inputData = m_session->getInputData();
    inputData["Ethnicity"] = m_test->getMetaData("ethnicity").toString();

    writer.setInputData(inputData.toVariantMap());
    writer.write(xmlPath.filePath(m_inFileName), m_session->getBarcode());

    return true;
}

bool EasyoneConnectManager::configureProcess()
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

    return true;
}

QString EasyoneConnectManager::getOutputPdfPath() const
{
    return QString("%1/%2.pdf").arg(m_exchangePath, m_session->getBarcode());
}

QString EasyoneConnectManager::getEMRInXmlName() const
{
    return QString("%1/%2").arg(m_exchangePath, m_inFileName);
}

QString EasyoneConnectManager::getEMROutXmlName() const
{
    return QString("%1/%2").arg(m_exchangePath, m_outFileName);
}

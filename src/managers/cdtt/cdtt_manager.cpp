#include "cdtt_manager.h"
#include "cypress_settings.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>
#include <QSettings>
#include <QSqlDatabase>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QSqlError>

CDTTManager::CDTTManager(QSharedPointer<CDTTSession> session)
    : ManagerBase(session)
{
    m_runnableName = CypressSettings::readSetting("cdtt/runnableName").toString();
    m_runnablePath = CypressSettings::readSetting("cdtt/runnablePath").toString();
    m_outputPath = CypressSettings::readSetting("cdtt/outputPath").toString();

    QDir outputDir(m_outputPath);
    m_outputFile = outputDir.filePath(QString("Results-%0.xlsx").arg(m_session->getBarcode()));

    if (m_debug) {
        qDebug() << "CDTTManager";

        qDebug() << session->getSessionId();
        qDebug() << session->getBarcode();
        qDebug() << session->getInterviewer();

        qDebug() << session->getInputData();
    }

    m_test.reset(new CDTTTest);
    m_test->setMinimumMeasurementCount(1);
}

CDTTManager::~CDTTManager()
{
    QSqlDatabase::removeDatabase("xlsx_connection");
}

bool CDTTManager::isInstalled()
{
    QString runnableName = CypressSettings::readSetting("cdtt/runnableName").toString();
    QString runnablePath = CypressSettings::readSetting("cdtt/runnablePath").toString();
    QString outputPath = CypressSettings::readSetting("cdtt/outputPath").toString();

    if (runnableName.isNull() || runnableName.isEmpty())
        return false;

    if (runnablePath.isNull() || runnablePath.isEmpty())
        return false;

    if (outputPath.isNull() || outputPath.isEmpty())
        return false;

    QFileInfo runnableNameInfo(runnableName);
    if (!runnableNameInfo.isFile())
        return false;

    QDir runnableDir(runnablePath);
    if (!runnableDir.exists())
        return false;

    QDir outputDir(outputPath);
    if (!outputDir.exists())
        return false;

    return true;
}

bool CDTTManager::start()
{
    if (m_debug)
        qDebug() << "CDTT::start";

    if (!setUp())
        return false;

    measure();

    return true;
}

// Set up device
bool CDTTManager::setUp()
{
    if (m_debug)
        qDebug() << "CDTT::setUp";

    clearData();
    cleanUp();
    configureProcess();

    return true;
}

void CDTTManager::measure()
{
    if (m_debug)
        qDebug() << "CDTT::measure";

    clearData();

    if (m_process.state() != QProcess::NotRunning) {
        emit error("CDTT is already running");
        return;
    }

    m_process.start();
    if (!m_process.waitForStarted()) {
        emit error("Could not start CDTT application");
    }
}

bool CDTTManager::clearData()
{
    if (m_debug)
        qDebug() << "CDTTManager::clearData";

    m_test->reset();

    return true;
}

void CDTTManager::configureProcess()
{
    if (m_debug)
        qDebug() << "CDTTManager::configureProcess";

    QString command = "java";
    QStringList arguments;

    arguments << "-jar" << m_runnableName << m_session->getBarcode();

    m_process.setProgram(command);
    m_process.setArguments(arguments);
    m_process.setWorkingDirectory(m_runnablePath);
    m_process.setProcessChannelMode(QProcess::ForwardedChannels);

    if (m_debug) {
        qDebug() << "CDTTManager::configureProcess - config args: "
                 << m_process.arguments().join(" ");
        qDebug() << "CDTTManager::configureProces - working dir: "
                 << m_runnablePath;
    }

    // connect signals and slots to QProcess one time only
    //
    connect(&m_process, &QProcess::started, this,
        [this] () {
            qDebug() << "process started: " << m_process.arguments().join(" ");
        }
    );

    connect(
        &m_process,
        QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        this,
        &CDTTManager::readOutput
    );

    connect(&m_process, &QProcess::errorOccurred, this,
        [](QProcess::ProcessError error) {
            QStringList s = QVariant::fromValue(error).toString().split(QRegExp("(?=[A-Z])"), Qt::SkipEmptyParts);
            qDebug() << "ERROR: process error occured: " << s.join(" ").toLower();
        });

    connect(&m_process, &QProcess::stateChanged, this,
        [](QProcess::ProcessState state) {
            QStringList s = QVariant::fromValue(state).toString().split(QRegExp("(?=[A-Z])"), Qt::SkipEmptyParts);
            qDebug() << "process state: " << s.join(" ").toLower();
        });
}

void CDTTManager::readOutput()
{
    if (m_debug)
        qDebug() << "CDTTManager::readOutput";

    if(QProcess::NormalExit != m_process.exitStatus())
    {
        QMessageBox::critical(nullptr, "Error", "CDTT failed to finish correctly, cannot read data. Please contact support");
        return;
    }
    else
        qDebug() << "CDTTManager::readOutput - process finished successfully";

    CDTTTest* test = static_cast<CDTTTest*>(m_test.get());

    QDir dir(m_outputPath);
    QString fileName = dir.filePath(QString("Results-%0.xlsx").arg(m_session->getBarcode()));
    fileName = dir.toNativeSeparators(fileName);

    if (QFileInfo::exists(fileName)) {
        if (m_debug)
            qDebug() << "found output xlsx file " << fileName;

        QSqlDatabase db = QSqlDatabase::addDatabase("QODBC", "xlsx_connection");
        db.setDatabaseName(
            "DRIVER={Microsoft Excel Driver (*.xls, *.xlsx, *.xlsm, *.xlsb)};DBQ=" + fileName);

        if(db.isValid())
            db.open();
        else {
            emit error("ERROR: invalid database");
            return;
        }

        if(db.isOpen())
        {
            test->fromDatabase(db);
            finish();
            db.close();
        }
        else {
            qDebug() << "cannot find valid file";
            emit error("Something went wrong. Please contact support");
        }
    } else {
        qDebug() << "ERROR: no output xlsx file found" << fileName;
        emit error("Something went wrong. Please contact support");
    }
}



// Clean up the device for next time
bool CDTTManager::cleanUp()
{
    if (m_debug)
        qDebug() << "CDTTManager::cleanUp";

    m_test->reset();

    if(QProcess::NotRunning != m_process.state())
        m_process.kill();

    if(!m_outputFile.isEmpty() && QFileInfo::exists(m_outputFile))
    {
        QFile ofile(m_outputFile);
        ofile.remove();
        m_outputFile.clear();
    }
    return true;
}

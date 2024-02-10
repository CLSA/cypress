#include "choice_reaction_manager.h"
#include "cypress_session.h"

#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QJsonObject>
#include <QProcess>
#include <QSettings>
#include <QStandardItemModel>
#include <QJsonDocument>
#include <QMessageBox>

QString ChoiceReactionManager::CCB_PREFIX = "CLSA_ELCV";
QString ChoiceReactionManager::CCB_CLINIC = "CYPRESS";

ChoiceReactionManager::ChoiceReactionManager(QSharedPointer<ChoiceReactionSession> session)
    : ManagerBase(session)
{
    // absolute path to exe
    m_runnableName = CypressSettings::readSetting("choice_reaction/runnableName").toString();

    // absolute path to working directory
    m_runnablePath = CypressSettings::readSetting("choice_reaction/runnablePath").toString();

    // absolute path to output directory
    m_outputPath = CypressSettings::readSetting("choice_reaction/outputPath").toString();

    if (m_debug) {
        qDebug() << "ChoiceReactionManager";

        qDebug() << session->getSessionId();
        qDebug() << session->getBarcode();
        qDebug() << session->getInterviewer();

        qDebug() << session->getInputData();
    }

    m_test.reset(new ChoiceReactionTest);
}

bool ChoiceReactionManager::isInstalled()
{
    bool isDebugMode = CypressSettings::isDebugMode();

    QString runnableName = CypressSettings::readSetting("choice_reaction/runnableName").toString();
    QString runnablePath = CypressSettings::readSetting("choice_reaction/runnablePath").toString();
    QString outputPath = CypressSettings::readSetting("choice_reaction/outputPath").toString();

    if (runnableName.isNull() || runnableName.isEmpty())
    {
        if (isDebugMode)
            qDebug() << "ChoiceReactionManager::isInstalled: runnableName is not defined";

        return false;
    }

    if (runnablePath.isNull() || runnablePath.isEmpty())
    {
        if (isDebugMode)
            qDebug() << "ChoiceReactionManager::isInstalled: runnablePath is not defined";

        return false;
    }

    if (outputPath.isNull() || outputPath.isEmpty())
    {
        if (isDebugMode)
            qDebug() << "ChoiceReactionManager::isInstalled: outputPath is not defined";

        return false;
    }

    QFileInfo info(runnableName);
    if (!info.exists())
    {
        if (isDebugMode)
            qDebug() << "ChoiceReactionManager::isInstalled - CCB.exe does not exist at " << runnableName;

        return false;
    }

    if (!info.isExecutable())
    {
        if (isDebugMode)
            qDebug() << "ChoiceReactionManager::isInstalled - file is not executable at " << runnableName;

        return false;
    }

    QDir working(runnablePath);
    if (!working.exists())
    {
        if (isDebugMode)
            qDebug() << "ChoiceReactionManager::isInstalled - working directory does not exist";

        return false;
    }

    QDir out(outputPath);
    if (!out.exists())
    {
        if (isDebugMode)
            qDebug() << "ChoiceReactionManager::isInstalled - output path does not exist";

        return false;
    }

    return true;
}

bool ChoiceReactionManager::start()
{
    if (m_debug)
        qDebug() << "ChoiceReactionManager::start";

    if (!setUp()) {
        return false;
    }

    measure();
    return true;
}

void ChoiceReactionManager::measure()
{
    if (m_debug)
        qDebug() << "ChoiceReactionManager::measure";

    clearData();

    if (m_process.state() != QProcess::NotRunning) {
        emit error("Application is already running");
        return;
    }

    m_process.start();

    if (!m_process.waitForStarted()) {
        emit error("Could not start application");
        return;
    }
}

void ChoiceReactionManager::readOutput()
{
    // Output file name will be of the form
    // <CCB_PREFIX>_<CCB_CLINIC>_YYYYMMDD.csv
    // if CCB_CLINC is not specified at run time with /c option CCB_CLINIC = "Default"
    // YYYYMMDD is the current date.  Multiple runs of CCB.exe therefore overwrite
    // the output file.
    // user id and interviewer id are embedded in the csv file content.
    //
    if (m_debug)
        qDebug() << "ChoiceReactionManager::readOutput";

    if (QProcess::NormalExit != m_process.exitStatus()) {
        emit error("Process failed to finish correctly, cannot read output");
        return;
    }

    QStringList pattern;

    pattern << CCB_PREFIX << CCB_CLINIC << QDate().currentDate().toString("yyyyMMdd");

    QString outputFile = pattern.join("_") + ".csv";
    QDir dir(m_outputPath);

    QSharedPointer<ChoiceReactionTest> test = qSharedPointerCast<ChoiceReactionTest>(m_test);
    if(dir.exists(outputFile))
    {
        outputFile.prepend(QDir::separator());
        outputFile.prepend(m_outputPath);

        test->fromFile(outputFile);
        finish();
    }
    else
    {
        emit error("Something went wrong. Please contact support");
        m_test->reset();
    }
}



// Set up device
bool ChoiceReactionManager::setUp()
{
    if (m_debug)
        qDebug() << "ChoiceReactionManager::setUp";

    if (!cleanUp()) {
        return false;
    }

    configureProcess();

    return true;
}

// Clean up the device for for setup and next time
bool ChoiceReactionManager::cleanUp()
{
    if (m_debug)
        qDebug() << "ChoiceReactionManager::cleanUp";

    m_test->reset();

    if(QProcess::NotRunning != m_process.state())
    {
        m_process.close();
        m_process.waitForFinished();
    }

    // Clear all files
    QDir outputDir(m_outputPath);
    outputDir.setFilter(QDir::Files | QDir::NoDotAndDotDot);
    QStringList fileList = outputDir.entryList();

    foreach (const QString& file, fileList)
    {
        QString filePath = outputDir.absoluteFilePath(file);
        if (!QFile::remove(filePath)) {
            qDebug() << "could not remove " << filePath;
        }
    }

    return true;
}

void ChoiceReactionManager::configureProcess()
{
    if (m_debug)
        qDebug() << "ChoiceReactionManager::configureProcess";

    // connect signals and slots to QProcess one time only
    //
    connect(&m_process, &QProcess::started,
        this, [this]() {
            qDebug() << "process started: " << m_process.arguments().join(" ");
        });

    // read output from csv after process finishes
    connect(&m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        this, &ChoiceReactionManager::readOutput);

    // error occured,
    connect(&m_process, &QProcess::errorOccurred,
        this, [=](QProcess::ProcessError error)
        {
            QStringList s = QVariant::fromValue(error).toString().split(QRegExp("(?=[A-Z])"), Qt::SkipEmptyParts);
            if (m_debug)
                qDebug() << "ERROR: process error occured: " << s.join(" ").toLower();
        });

    connect(&m_process, &QProcess::stateChanged,
    this, [=](QProcess::ProcessState state) {
        QStringList s = QVariant::fromValue(state).toString().split(QRegExp("(?=[A-Z])"), Qt::SkipEmptyParts);
        if (m_debug)
            qDebug() << "process state: " << s.join(" ").toLower();
    });

    if (m_debug)
        qDebug() << "ChoiceReactionManager::configureProcess - ok, configuring command";

    // the inputs for command line args are present
    QStringList command;
    command << m_runnableName;

    if(!m_session->getInterviewer().isNull() && !m_session->getInterviewer().isEmpty())
        command << "/i" + m_session->getInterviewer();
    else
        command << "/iNone";

    // minimum required input to identify the file belonging to the participant
    //
    command << "/u" + m_session->getBarcode();

    // TODO: consider using upstream host "clinic" identifier
    //
    command << "/c" + CCB_CLINIC;

    // required language "en" or "fr" converted to E or F
    //
    QString s = m_session->getLanguage().toUpper();
    if (!s.isEmpty()) {
        command << "/l" + QString(s.at(0));
    }

    m_process.setProgram(m_runnableName);
    m_process.setArguments(command);
    m_process.setWorkingDirectory(m_runnablePath);
    m_process.setProcessChannelMode(QProcess::ForwardedChannels);

    if (m_debug) {
        qDebug() << "ChoiceReactionManager - process config args: " << m_process.arguments().join(" ");
        qDebug() << "ChoiceReactionManager - process working dir: " << m_runnablePath;
    }
}

bool ChoiceReactionManager::clearData()
{
    if (m_debug)
        qDebug() << "ChoiceReactionManager::clearData";

    m_test->reset();

    emit dataChanged(m_test);

    return true;
}

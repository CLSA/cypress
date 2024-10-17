#include "choice_reaction_manager.h"
#include "data/choice_reaction/tests/choice_reaction_test.h"

#include "auxiliary/file_utils.h"
#include "auxiliary/network_utils.h"

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

DeviceConfig ChoiceReactionManager::config {{
    {"runnableName", {"choice_reaction/runnableName",  Exe }},
    {"runnablePath", {"choice_reaction/runnablePath",  Dir }},
    {"outputPath",   {"choice_reaction/outputPath",    Dir }},
}};

ChoiceReactionManager::ChoiceReactionManager(QSharedPointer<ChoiceReactionSession> session)
    : ManagerBase(session)
{
    // absolute path to exe
    m_runnableName = config.getSetting("runnableName");

    // absolute path to working directory
    m_runnablePath = config.getSetting("runnablePath");

    // absolute path to output directory
    m_outputPath = config.getSetting("outputPath");

    m_test.reset(new ChoiceReactionTest);
}

bool ChoiceReactionManager::start()
{
    qDebug() << "ChoiceReactionManager::start";

    if (!setUp())
        return false;

    measure();
    return true;
}

void ChoiceReactionManager::measure()
{
    qDebug() << "ChoiceReactionManager::measure";

    clearData();

    if (m_process.state() != QProcess::NotRunning) {
        emit error("Application is already running");
        return;
    }

    m_process.start();

    if (!m_process.waitForStarted()) {
        emit error("Could not start application");
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
    qDebug() << "ChoiceReactionManager::readOutput";

    if (QProcess::NormalExit != m_process.exitStatus()) {
        emit error("Process failed to finish correctly, cannot read output");
        return;
    }

    QStringList pattern;

    pattern << CCB_PREFIX << CCB_CLINIC << QDate().currentDate().toString("yyyyMMdd");

    QString outputFile = pattern.join("_") + ".csv";
    const QDir dir(m_outputPath);

    auto test = qSharedPointerCast<ChoiceReactionTest>(m_test);

    if(dir.exists(outputFile)) {
        outputFile.prepend(QDir::separator());
        outputFile.prepend(m_outputPath);

        test->fromFile(outputFile);
        finish();
    }
    else {
        emit error("Something went wrong. Please contact support");
    }
}

void ChoiceReactionManager::finish() {
    qInfo() << "ChoiceReactionManager::finish";

    const int answer_id = m_session->getAnswerId();
    const QString pineOrigin = m_session->getOrigin();
    const QString answerUrl = pineOrigin + "/answer/" + QString::number(answer_id);

    QDir dir(m_outputPath);
    if (!dir.exists()) {
        qDebug() << "directory does not exist: " << m_outputPath;
        emit error("Output directory does not exist. Could not save measurements.");
        return;
    }

    QStringList pattern;
    pattern << CCB_PREFIX << CCB_CLINIC << QDate().currentDate().toString("yyyyMMdd");
    QString outputFile = pattern.join("_") + ".csv";

    QFileInfo csvFile(dir.absoluteFilePath(outputFile));
    if (!csvFile.exists()) {
        qDebug() << "file does not exist: " << csvFile.absoluteFilePath();
        emit error("Output excel file does not exist. Could not save measurements.");
        return;
    }

    QJsonObject filesJson {};
    filesJson.insert("data_csv", FileUtils::getHumanReadableFileSize(csvFile.absoluteFilePath()));

    QJsonObject testJson = m_test->toJsonObject();
    QJsonObject sessionJson = m_session->getJsonObject();
    testJson.insert("session", sessionJson);
    testJson.insert("files", filesJson);

    QJsonObject responseJson {};
    responseJson.insert("value", testJson);

    QJsonDocument jsonDoc(responseJson);
    QByteArray serializedData = jsonDoc.toJson();

    bool ok = NetworkUtils::sendHTTPSRequest(
        Poco::Net::HTTPRequest::HTTP_PATCH,
        (answerUrl  + "?filename=" + "data.csv").toStdString(),
        "application/octet-stream",
        FileUtils::readFile(csvFile.absoluteFilePath())
    );

    if (!ok) {
        emit error("Could not transfer CSV file to Pine.");
    }

    ok = NetworkUtils::sendHTTPSRequest(
        Poco::Net::HTTPRequest::HTTP_PATCH,
        answerUrl.toStdString(),
        "application/json",
        serializedData
    );

    cleanUp();

    if (ok)
        emit success("Save successful. You may close this window.");
    else
        emit error("Something went wrong");
}

// Set up device
bool ChoiceReactionManager::setUp()
{
    qInfo() << "ChoiceReactionManager::setUp";

    if (!cleanUp())
        return false;

    configureProcess();

    return true;
}

// Clean up the device for for setup and next time
bool ChoiceReactionManager::cleanUp()
{
    qInfo() << "ChoiceReactionManager::cleanUp";

    m_test->reset();

    // Close the process if it's already running
    if(QProcess::NotRunning != m_process.state()) {
        m_process.close();
        m_process.waitForFinished();
    }

    // Clear all output files
    QDir outputDir(m_outputPath);
    outputDir.setFilter(QDir::Files | QDir::NoDotAndDotDot);

    const QStringList fileList = outputDir.entryList();
    foreach (const QString& file, fileList)
    {
        const QString filePath = outputDir.absoluteFilePath(file);
        if (!QFile::remove(filePath)) {
            qDebug() << "could not remove " << filePath;
        }
    }

    return true;
}

void ChoiceReactionManager::configureProcess()
{
    qInfo() << "ChoiceReactionManager::configureProcess";

    // connect signals and slots to QProcess one time only
    //
    connect(&m_process, &QProcess::started,
        this, [this]() {
            qInfo() << "process started: " << m_process.arguments().join(" ");
        });

    // read output from csv after process finishes
    connect(&m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        this, &ChoiceReactionManager::readOutput);

    // error occured,
    connect(&m_process, &QProcess::errorOccurred,
        this, [=](QProcess::ProcessError error)
        {
            QStringList s = QVariant::fromValue(error).toString().split(QRegExp("(?=[A-Z])"), Qt::SkipEmptyParts);
            qWarning() << "process error occured: " << s.join(" ").toLower();
        });

    connect(&m_process, &QProcess::stateChanged,
    this, [=](QProcess::ProcessState state) {
        QStringList s = QVariant::fromValue(state).toString().split(QRegExp("(?=[A-Z])"), Qt::SkipEmptyParts);
        qInfo() << "process state: " << s.join(" ").toLower();
    });

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
    const QString s = m_session->getLanguage().toUpper();
    if (!s.isEmpty()) {
        command << "/l" + QString(s.at(0));
    }

    m_process.setProgram(m_runnableName);
    m_process.setArguments(command);
    m_process.setWorkingDirectory(m_runnablePath);
    m_process.setProcessChannelMode(QProcess::ForwardedChannels);

    qDebug() << "ChoiceReactionManager: process config args: " << m_process.arguments().join(" ");
    qDebug() << "ChoiceReactionManager: process working dir: " << m_runnablePath;
}

bool ChoiceReactionManager::clearData()
{
    qDebug() << "ChoiceReactionManager::clearData";

    m_test->reset();

    emit dataChanged(m_test);

    return true;
}

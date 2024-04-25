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

ChoiceReactionManager::ChoiceReactionManager(QSharedPointer<ChoiceReactionSession> session)
    : ManagerBase(session)
{
    // absolute path to exe
    m_runnableName = CypressSettings::readSetting("choice_reaction/runnableName").toString();

    // absolute path to working directory
    m_runnablePath = CypressSettings::readSetting("choice_reaction/runnablePath").toString();

    // absolute path to output directory
    m_outputPath = CypressSettings::readSetting("choice_reaction/outputPath").toString();

    m_test.reset(new ChoiceReactionTest);

    qInfo() << "ChoiceReactionManager";
    qInfo() << session->getSessionId();
    qInfo() << session->getBarcode();
    qInfo() << session->getInterviewer();
    qInfo() << session->getInputData();
}

bool ChoiceReactionManager::isInstalled()
{
    qInfo() << "ChoiceReactionManager::isInstalled";
    if (CypressSettings::isSimMode())
        return true;

    const QString runnableName = CypressSettings::readSetting("choice_reaction/runnableName").toString();
    const QString runnablePath = CypressSettings::readSetting("choice_reaction/runnablePath").toString();
    const QString outputPath = CypressSettings::readSetting("choice_reaction/outputPath").toString();

    if (runnableName.isNull() || runnableName.isEmpty()) {
        qInfo() << "runnableName is not defined";
        return false;
    }

    if (runnablePath.isNull() || runnablePath.isEmpty()) {
        qInfo() << "runnablePath is not defined";
        return false;
    }

    if (outputPath.isNull() || outputPath.isEmpty()) {
        qInfo() << "outputPath is not defined";
        return false;
    }

    const QFileInfo info(runnableName);
    if (!info.exists()) {
        qInfo() << "exe does not exist at " << runnableName;
        return false;
    }

    if (!info.isExecutable()) {
        qInfo() << "file is not executable at " << runnableName;
        return false;
    }

    const QDir working(runnablePath);
    if (!working.exists()) {
        qInfo() << "working directory does not exist";
        return false;
    }

    const QDir out(outputPath);
    if (!out.exists()) {
        qInfo() << "output path does not exist";
        return false;
    }

    return true;
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
    //const QString host = CypressSettings::getPineHost();
    //const QString endpoint = CypressSettings::getPineEndpoint();
    qInfo() << "ChoiceReactionManager::finish";

    const int answer_id = m_session->getAnswerId();
    const QString pineOrigin = m_session->getOrigin();
    const QString answerUrl = pineOrigin + "/answer/" + QString::number(answer_id);

    qDebug() << answerUrl;

    QDir dir(m_outputPath);
    if (!dir.exists()) {
        qDebug() << "directory does not exist: " << m_outputPath;
        emit error("Output directory does not exist. Could not save measurements.");
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
    qDebug() << "ChoiceReactionManager::setUp";

    if (!cleanUp())
        return false;

    configureProcess();

    return true;
}

// Clean up the device for for setup and next time
bool ChoiceReactionManager::cleanUp()
{
    qDebug() << "ChoiceReactionManager::cleanUp";

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
            qDebug() << "ERROR: process error occured: " << s.join(" ").toLower();
        });

    connect(&m_process, &QProcess::stateChanged,
    this, [=](QProcess::ProcessState state) {
        QStringList s = QVariant::fromValue(state).toString().split(QRegExp("(?=[A-Z])"), Qt::SkipEmptyParts);
        qDebug() << "process state: " << s.join(" ").toLower();
    });

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
    const QString s = m_session->getLanguage().toUpper();
    if (!s.isEmpty()) {
        command << "/l" + QString(s.at(0));
    }

    m_process.setProgram(m_runnableName);
    m_process.setArguments(command);
    m_process.setWorkingDirectory(m_runnablePath);
    m_process.setProcessChannelMode(QProcess::ForwardedChannels);

    qDebug() << "ChoiceReactionManager - process config args: " << m_process.arguments().join(" ");
    qDebug() << "ChoiceReactionManager - process working dir: " << m_runnablePath;
}

bool ChoiceReactionManager::clearData()
{
    qDebug() << "ChoiceReactionManager::clearData";

    m_test->reset();

    emit dataChanged(m_test);

    return true;
}

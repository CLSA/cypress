#include "choice_reaction_manager.h"

#include "cypress_application.h"
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
    m_test.reset(new ChoiceReactionTest);
}

bool ChoiceReactionManager::isAvailable()
{
    return false;
}

bool ChoiceReactionManager::isInstalled()
{
    return false;
}

void ChoiceReactionManager::start()
{
    setUp();

    emit started(m_test.get());
    emit canMeasure();
}

void ChoiceReactionManager::configureProcess()
{

    // connect signals and slots to QProcess one time only
    //
    connect(&m_process, &QProcess::started,
        this, [this]() {
            qDebug() << "process started: " << m_process.arguments().join(" ");
        });

    connect(&m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        this, &ChoiceReactionManager::readOutput);

    connect(&m_process, &QProcess::errorOccurred,
        this, [](QProcess::ProcessError error)
        {
            QStringList s = QVariant::fromValue(error).toString().split(QRegExp("(?=[A-Z])"), Qt::SkipEmptyParts);
            qDebug() << "ERROR: process error occured: " << s.join(" ").toLower();
        });

    connect(&m_process, &QProcess::stateChanged,
        this, [](QProcess::ProcessState state) {
            QStringList s = QVariant::fromValue(state).toString().split(QRegExp("(?=[A-Z])"), Qt::SkipEmptyParts);
            qDebug() << "process state: " << s.join(" ").toLower();
        });

    // CCB.exe is present
    //
    QFileInfo info(m_runnableName);
    QDir working(m_runnablePath);
    QDir out(m_outputPath);

    if(info.exists() && info.isExecutable() &&
        working.exists() && out.exists() &&
        !m_inputData.isEmpty())
    {
        qDebug() << "OK: configuring command";

        // the inputs for command line args are present
        QStringList command;
        command << m_runnableName;

        if(m_inputData.contains("interviewer_id"))
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
        if(!s.isEmpty())
        {
            command << "/l" + QString(s.at(0));
        }

        m_process.setProgram(m_runnableName);
        m_process.setArguments(command);
        m_process.setWorkingDirectory(m_runnablePath);
        m_process.setProcessChannelMode(QProcess::ForwardedChannels);

        qDebug() << "process config args: " << m_process.arguments().join(" ");
        qDebug() << "process working dir: " << m_runnablePath;

        emit canMeasure();
    }
    else
        qDebug() << "failed to configure process";
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
    QStringList pattern;

    pattern << CCB_PREFIX << CCB_CLINIC << QDate().currentDate().toString("yyyyMMdd");

    QString fileName = pattern.join("_") + ".csv";
    QDir dir(m_outputPath);

    ChoiceReactionTest* test = static_cast<ChoiceReactionTest*>(m_test.get());

    if(dir.exists(fileName))
    {
        qDebug() << "found output csv file " << fileName;

        fileName.prepend(QDir::separator());
        fileName.prepend(m_outputPath);

        test->fromFile(fileName);
        m_outputFile.clear();

        if(test->isValid())
        {
            emit measured(test);
            emit canFinish();

            m_outputFile = fileName;
        }
        else
        {
            // remove the file
            QFile::remove(fileName);
            QMessageBox::critical(nullptr, "Error", "ERROR: input from file produced invalid test results.");

            qDebug() << "ERROR: input from file produced invalid test results";
            qDebug() << "removing " << fileName;
        }
    }
    else
    {
        QMessageBox::critical(nullptr, "Error", "ERROR: no output csv file found");
    }
}

void ChoiceReactionManager::measure()
{
    m_test->reset();

    if (Cypress::getInstance().isSimulation())
    {
        m_test->simulate();
        emit measured(m_test.get());
        emit canFinish();
    }

    m_process.start();
}

void ChoiceReactionManager::finish()
{
    QJsonObject responseJson {};

    int answer_id = m_session->getAnswerId();

    QJsonObject testJson = m_test->toJsonObject();
    testJson.insert("session", m_session->getJsonObject());
    responseJson.insert("value", testJson);

    QJsonDocument jsonDoc(responseJson);
    QByteArray serializedData = jsonDoc.toJson();

    QString answerUrl = CypressSettings::getInstance().getAnswerUrl(answer_id);
    sendHTTPSRequest("PATCH", answerUrl, "application/json", serializedData);

    emit success("Measurements saved to Pine");

    cleanUp();
}

bool ChoiceReactionManager::clearData()
{
    return false;
}


// Set up device
bool ChoiceReactionManager::setUp()
{
    configureProcess();
    return true;
}

// Clean up the device for next time
bool ChoiceReactionManager::cleanUp()
{
    m_test.reset();
    if(QProcess::NotRunning != m_process.state())
    {
        m_process.close();
    }
    if(!m_outputFile.isEmpty() && QFileInfo::exists(m_outputFile))
    {
        QFile ofile(m_outputFile);
        ofile.remove();
    }
    m_outputFile.clear();

    return true;
}

void ChoiceReactionManager::setInputData(const QVariantMap& inputData)
{
    Q_UNUSED(inputData)
}

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
    m_runnableName = CypressSettings::readSetting("choice_reaction/runnableName").toString();
    m_runnablePath = CypressSettings::readSetting("choice_reaction/runnablePath").toString();
    m_outputPath = CypressSettings::readSetting("choice_reaction/outputPath").toString();

    m_test.reset(new ChoiceReactionTest);
}

bool ChoiceReactionManager::isInstalled()
{
    return false;
}

void ChoiceReactionManager::start()
{
    if (m_debug)
    {
        qDebug() << "ChoiceReactionManager::start";
    }

    cleanUp();
    setUp();

    emit started(m_test.get());
    emit canMeasure();
}

void ChoiceReactionManager::configureProcess()
{
    if (m_debug)
    {
        qDebug() << "ChoiceReactionManager::configureProcess";
    }

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
            {
                qDebug() << "ERROR: process error occured: " << s.join(" ").toLower();
            }

            cleanUp();
            setUp();
        });

    connect(&m_process, &QProcess::stateChanged,
    this, [=](QProcess::ProcessState state) {
        QStringList s = QVariant::fromValue(state).toString().split(QRegExp("(?=[A-Z])"), Qt::SkipEmptyParts);
        if (m_debug)
        {
            qDebug() << "process state: " << s.join(" ").toLower();
        }
    });

    // CCB.exe is present
    //
    QFileInfo info(m_runnableName);
    if (!info.exists())
    {
        qDebug() << "ChoiceReactionManager::configureProcess - CCB.exe does not exist at " << m_runnableName;
        return;
    }

    if (!info.isExecutable())
    {
        qDebug() << "ChoiceReactionManager::configureProcess - file is not executable at " << m_runnableName;
    }

    QDir working(m_runnablePath);
    if (!working.exists())
    {
        qDebug() << "ChoiceReactionManager::configureProcess - working does not exist";
    }

    QDir out(m_outputPath);
    if (!out.exists())
    {
        qDebug() << "ChoiceReactionManager::configureProcess - out does not exist";
    }

    if(info.exists() && info.isExecutable() &&
        working.exists() && out.exists())
    {
        if (m_debug)
        {
            qDebug() << "ChoiceReactionManager::configureProcess - ok, configuring command";
        }

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
        if(!s.isEmpty())
        {
            command << "/l" + QString(s.at(0));
        }

        m_process.setProgram(m_runnableName);
        m_process.setArguments(command);
        m_process.setWorkingDirectory(m_runnablePath);
        m_process.setProcessChannelMode(QProcess::ForwardedChannels);

        if (m_debug)
        {
            qDebug() << "ChoiceReactionManager - process config args: " << m_process.arguments().join(" ");
            qDebug() << "ChoiceReactionManager - process working dir: " << m_runnablePath;
        }

        emit canMeasure();
    }
    else
    {
        if (m_debug)
        {
            qDebug() << "failed to configure process";
        }

        QMessageBox::critical(nullptr, "Error", "The choice reaction test could not be configured. Please contact support");
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
    {
        qDebug() << "ChoiceReactionManager::readOutput";
    }

    QStringList pattern;

    pattern << CCB_PREFIX << CCB_CLINIC << QDate().currentDate().toString("yyyyMMdd");

    QString outputFile = pattern.join("_") + ".csv";
    QDir dir(m_outputPath);

    ChoiceReactionTest* test = static_cast<ChoiceReactionTest*>(m_test.get());

    if(dir.exists(outputFile))
    {
        outputFile.prepend(QDir::separator());
        outputFile.prepend(m_outputPath);

        test->fromFile(outputFile);

        if(test->isValid())
        {
            emit measured(test);
            emit dataChanged(test);
            emit canFinish();

            qDebug() << "test is valid";
        }
        else
        {
            QFile::remove(outputFile);
            QMessageBox::critical(nullptr, "Error", "ERROR: input from file produced invalid test results.");

            if (m_debug)
            {
                qDebug() << "ERROR: input from file produced invalid test results";
                qDebug() << "Removed" << outputFile;
            }
        }
    }
    else
    {
        QMessageBox::critical(nullptr, "Error", "ERROR: no output csv file found");
        m_test->reset();
        emit dataChanged(test);
    }
}

void ChoiceReactionManager::measure()
{
    if (m_debug)
    {
        qDebug() << "ChoiceReactionManager::measure";
    }

    m_test->reset();

    if (CypressSettings::isSimMode())
    {
        m_test->simulate();
        emit measured(m_test.get());
        emit canFinish();

        return;
    }

    m_process.start();
}

void ChoiceReactionManager::finish()
{
    if (m_debug)
    {
        qDebug() << "ChoiceReactionManager::finish";
    }

    QJsonObject responseJson {};

    int answer_id = m_session->getAnswerId();

    QJsonObject testJson = m_test->toJsonObject();
    testJson.insert("session", m_session->getJsonObject());
    responseJson.insert("value", testJson);

    QJsonDocument jsonDoc(responseJson);
    QByteArray serializedData = jsonDoc.toJson();

    QString answerUrl = CypressSettings::getAnswerUrl(answer_id);
    sendHTTPSRequest("PATCH", answerUrl, "application/json", serializedData);

    emit success("Measurements saved to Pine");

    cleanUp();
}

bool ChoiceReactionManager::clearData()
{
    if (m_debug)
    {
        qDebug() << "ChoiceReactionManager::clearData";
    }

    return false;
}


// Set up device
bool ChoiceReactionManager::setUp()
{
    if (m_debug)
    {
        qDebug() << "ChoiceReactionManager::setUp";
    }

    configureProcess();
    return true;
}

// Clean up the device for next time
bool ChoiceReactionManager::cleanUp()
{
    if (m_debug)
    {
        qDebug() << "ChoiceReactionManager::cleanUp";
    }

    m_test->reset();

    if(QProcess::NotRunning != m_process.state())
    {
        m_process.close();
    }

    //if(!m_outputFile.isEmpty() && QFileInfo::exists(m_outputFile))
    //{
    //    QFile ofile(m_outputFile);
    //    ofile.remove();
    //}

    // Clear all files
    QDir outputDir(m_outputPath);
    outputDir.setFilter(QDir::Files | QDir::NoDotAndDotDot);
    QStringList fileList = outputDir.entryList();

    foreach (const QString& file, fileList)
    {
        QFile::remove(outputDir.absoluteFilePath(file));
    }

    emit dataChanged(m_test.get());

    return true;
}

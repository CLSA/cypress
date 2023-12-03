#include "grip_strength_manager.h"
#include "data/grip_strength/tests/grip_strength_test.h"

#include "cypress_settings.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQuery>

#include <QTemporaryDir>
#include <QFile>
#include <QtSql>

GripStrengthManager::GripStrengthManager(QSharedPointer<GripStrengthSession> session)
    : ManagerBase(session)
{
    m_runnableName = CypressSettings::readSetting("grip_strength/executablePath").toString();
    m_runnablePath = CypressSettings::readSetting("grip_strength/workingDirectoryPath").toString();

    m_databasePath = CypressSettings::readSetting("grip_strength/databasePath").toString();
    m_backupPath = CypressSettings::readSetting("grip_strength/backupPath").toString();

    m_test.reset(new GripStrengthTest);
}

bool GripStrengthManager::isInstalled()
{
    bool isDebugMode = CypressSettings::isDebugMode();

    QString runnableName = CypressSettings::readSetting("grip_strength/runnableName").toString();
    QString runnablePath = CypressSettings::readSetting("grip_strength/runnablePath").toString();

    QString databasePath = CypressSettings::readSetting("grip_strength/databasePath").toString();
    QString backupPath = CypressSettings::readSetting("grip_strength/backupPath").toString();

    if (runnableName.isEmpty() || runnableName.isNull())
    {
        if (isDebugMode)
        {
            qDebug() << "GripStrengthManager::isInstalled: runnableName is not defined";
        }

        return false;
    }

    if (runnablePath.isEmpty() || runnablePath.isNull())
    {
        if (isDebugMode)
        {
            qDebug() << "GripStrengthManager::isInstalled: runnablePath is not defined";
        }
        return false;
    }

    if (databasePath.isEmpty() || databasePath.isNull())
    {
        if (isDebugMode)
        {
            qDebug() << "GripStrengthManager::isInstalled: databasePath is not defined";
        }
        return false;
    }

    if (backupPath.isEmpty() || backupPath.isNull())
    {
        if (isDebugMode)
        {
            qDebug() << "GripStrengthManager::isInstalled: backupPath is not defined";
        }

        return false;
    }

    // check if exe is present and executable
    //
    QFileInfo executable(runnableName);
    if (!executable.exists())
    {
        if (isDebugMode)
        {
            qDebug() << "GripStrengthManager::isInstalled - file does not exist at " << runnableName;
        }
        return false;
    }
    if (!executable.isExecutable())
    {
        if (isDebugMode)
        {
            qDebug() << "GripStrengthManager::isInstalled - file is not executable at " << runnableName;
        }

        return false;
    }

    QDir databaseDir(databasePath);
    if (!databaseDir.exists())
    {
        if (isDebugMode)
        {
            qDebug() << "GripStrengthManager::isInstalled - runnable path directory does not exist";
        }

        return false;
    }

    return true;
}


void GripStrengthManager::start()
{
    if (m_debug)
    {
        qDebug() << "GripStrengthManager::start";
    }

    setUp();

    emit started(m_test.get());
    emit canMeasure();
}

void GripStrengthManager::readOutput()
{

}

void GripStrengthManager::measure()
{
    if (m_debug)
    {
        qDebug() << "GripStrengthManager::measure";
    }

    m_test->reset();

    if (CypressSettings::isSimMode())
    {
        m_test->simulate();

        emit measured(m_test.get());
        emit dataChanged(m_test.get());
        emit canFinish();

        return;
    }

    m_process.start();
}

void GripStrengthManager::finish()
{
    if (m_debug)
    {
        qDebug() << "GripStrengthManager::finish";
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

    emit success("Measurements sent to Pine");

    cleanUp();
}

void GripStrengthManager::addManualMeasurement()
{
    if (m_debug)
    {
        qDebug() << "GripStrengthManager::addManualMeasurement";
    }

    QSharedPointer<GripStrengthMeasurement> measurement(new GripStrengthMeasurement);
    m_test->addMeasurement(measurement);

    emit dataChanged(m_test.get());
}

bool GripStrengthManager::setUp() {
    if (m_debug)
    {
        qDebug() << "GripStrengthManager::setUp";
    }

    cleanUp();
    configureProcess();

    return true;
}

bool GripStrengthManager::cleanUp() {
    if (m_debug)
    {
        qDebug() << "GripStrengthManager::cleanUp";
    }

    m_test->reset();

    // close process
    if (m_process.state() != QProcess::NotRunning)
    {
        m_process.close();
        m_process.waitForFinished();
    }

    // close database
    if (m_database.isOpen())
    {
        m_database.close();
    }

    // check if backup folder exists, if not then return since we are finished
    QDir backupFolder(m_backupPath);
    if (!backupFolder.exists())
    {
        if (m_debug)
        {
            qDebug() << "GripStrengthManager::cleanUp - backup folder doesn't exist";
        }

        return true;
    }

    // remove database folder, will be restored from backup
    QDir databaseFolder(m_databasePath);
    if (!databaseFolder.removeRecursively())
    {
        if (m_debug)
        {
            qDebug() << "GripStrengthManager::cleanUp - could not remove database folder";
        }
        return false;
    }

    if (m_debug)
    {
        qDebug() << "GripStrengthManager::cleanUp - database folder removed, restoring backup";
    }

    // copy backup to database folder
    foreach (const QString &fileName, backupFolder.entryList(QDir::Files))
    {
        QFile::copy(backupFolder.filePath(fileName), databaseFolder.filePath(fileName));
    }

    if (m_debug)
    {
        qDebug() << "GripStrengthManager::cleanUp - database folder restored from backup";
    }

    // remove backup folder
    if (!backupFolder.removeRecursively())
    {
        if (m_debug)
        {
            qDebug() << "GripStrengthManager::cleanUp - could not remove backup folder";
        }
        return false;
    }

    if (m_debug)
    {
        qDebug() << "GripStrengthManager::cleanUp - backup folder removed";
    }

    emit dataChanged(m_test.get());

    return true;
}

void GripStrengthManager::configureProcess()
{
    if (m_debug)
    {
        qDebug() << "GripStrengthManager::configureProcess";
    }

    // connect signals and slots to QProcess one time only
    //
    connect(&m_process, &QProcess::started,
        this, [this]() {

            if (m_debug)
            {
                qDebug() << "GripStrengthManager::process started: " << m_process.arguments().join(" ");
            }

        });

    // error occured with grip strength process
    connect(&m_process, &QProcess::errorOccurred,
        this, [=](QProcess::ProcessError error)
        {
            QStringList s = QVariant::fromValue(error).toString().split(QRegExp("(?=[A-Z])"), Qt::SkipEmptyParts);
            if (m_debug)
            {
                qDebug() << "GripStrengthManager::process error: process error occured: " << s.join(" ").toLower();
            }

            cleanUp();
            setUp();
        });

    connect(&m_process, &QProcess::stateChanged,
    this, [=](QProcess::ProcessState state) {
        QStringList s = QVariant::fromValue(state).toString().split(QRegExp("(?=[A-Z])"), Qt::SkipEmptyParts);
        if (m_debug)
        {
            qDebug() << "GripStrengthManager::process state: " << s.join(" ").toLower();
        }

    });

    // read output after grip strength process ends
    connect(&m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        this, &GripStrengthManager::readOutput);

    m_process.setProgram(m_runnableName);
    m_process.setWorkingDirectory(m_runnablePath);
    m_process.setProcessChannelMode(QProcess::ForwardedChannels);

    if (m_debug)
    {
        qDebug() << "GripStrengthManager::configureProcess - process configured";
    }
}

bool GripStrengthManager::clearData()
{
    if (m_debug)
    {
        qDebug() << "GripStrengthManager::clearData";
    }

    m_test->reset();

    emit dataChanged(m_test.get());

    return true;
}


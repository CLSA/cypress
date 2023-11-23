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
    m_workingDirPath = CypressSettings::readSetting("grip_strength/workingDirectoryPath").toString();
    m_executablePath = CypressSettings::readSetting("grip_strength/executablePath").toString();

    m_databasePath = CypressSettings::readSetting("grip_strength/databasePath").toString();
    m_backupPath = CypressSettings::readSetting("grip_strength/backupPath").toString();

    m_test.reset(new GripStrengthTest);
}

GripStrengthManager::~GripStrengthManager()
{
}

bool GripStrengthManager::isInstalled()
{
    return false;
}

void GripStrengthManager::start()
{
    if (m_debug)
    {
        qDebug() << "GripStrengthManager::start";
    }

    emit started(m_test.get());
    emit canMeasure();
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
        emit canFinish();
    }
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

    QDir databaseFolder(m_databasePath);
    if (!databaseFolder.exists())
    {
        return false;
    }

    QDir backupFolder(m_backupPath);
    if (backupFolder.exists())
    {
        backupFolder.removeRecursively();
    }

    backupFolder.mkpath(m_backupPath);
    foreach (const QString &fileName, databaseFolder.entryList(QDir::Files))
    {
        QFile::copy(databaseFolder.filePath(fileName), backupFolder.filePath(fileName));
    }

    return true;
}


void GripStrengthManager::configureProcess()
{
    if (m_debug)
    {
        qDebug() << "GripStrengthManager::configureProcess";
    }

    m_process.setProgram(m_executablePath);
    connect(&m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [this]
    {
        finish();
    });
}

bool GripStrengthManager::cleanUp() {
    if (m_debug)
    {
        qDebug() << "GripStrengthManager::cleanUp";
    }

    QDir backupFolder(m_backupPath);
    QDir databaseFolder(m_databasePath);

    if (m_database.isOpen())
    {
        m_database.close();
    }

    if (!databaseFolder.exists())
    {
        return false;
    }

    if (!backupFolder.exists())
    {
        return false;
    }

    if (!databaseFolder.removeRecursively())
    {

    }

    foreach (const QString &fileName, backupFolder.entryList(QDir::Files))
    {
        QFile::copy(backupFolder.filePath(fileName), databaseFolder.filePath(fileName));
    }

    if (!backupFolder.removeRecursively())
    {
    }

    return true;
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


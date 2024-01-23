#include "grip_strength_manager.h"
#include "data/grip_strength/tests/grip_strength_test.h"
#include "paradox_reader.h"

#include "cypress_settings.h"

#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QTemporaryDir>

GripStrengthManager::GripStrengthManager(QSharedPointer<GripStrengthSession> session)
    : ManagerBase(session)
{
    m_runnableName = CypressSettings::readSetting("grip_strength/runnableName").toString();
    m_runnablePath = CypressSettings::readSetting("grip_strength/runnablePath").toString();

    m_gripTestDBPath = CypressSettings::readSetting("grip_strength/gripTestDB").toString();
    m_gripTestDataDBPath = CypressSettings::readSetting("grip_strength/gripTestDataDB").toString();

    m_backupPath = CypressSettings::readSetting("grip_strength/backupPath").toString();
    m_databasePath = CypressSettings::readSetting("grip_strength/databasePath").toString();

    m_test.reset(new GripStrengthTest);
}

bool GripStrengthManager::isInstalled()
{
    bool isDebugMode = CypressSettings::isDebugMode();
    bool isSimMode = CypressSettings::isSimMode();

    if (isSimMode)
        return true;

    QString runnableName = CypressSettings::readSetting("grip_strength/runnableName").toString();
    QString runnablePath = CypressSettings::readSetting("grip_strength/runnablePath").toString();

    QString gripTestDBPath = CypressSettings::readSetting("grip_strength/gripTestDB").toString();
    QString gripTestDataDBPath = CypressSettings::readSetting("grip_strength/gripTestDataDB").toString();

    QString databasePath = CypressSettings::readSetting("grip_strength/databasePath").toString();
    QString backupPath = CypressSettings::readSetting("grip_strength/backupPath").toString();

    if (runnableName.isEmpty() || runnableName.isNull())
    {
        if (isDebugMode)
            qDebug() << "GripStrengthManager::isInstalled: runnableName is not defined";

        return false;
    }

    if (runnablePath.isEmpty() || runnablePath.isNull())
    {
        if (isDebugMode)
            qDebug() << "GripStrengthManager::isInstalled: runnablePath is not defined";

        return false;
    }

    if (databasePath.isEmpty() || databasePath.isNull())
    {
        if (isDebugMode)
            qDebug() << "GripStrengthManager::isInstalled: databasePath is not defined";

        return false;
    }

    if (gripTestDBPath.isEmpty() || gripTestDBPath.isNull())
    {
        if (isDebugMode)
            qDebug() << "GripStrengthManager::isInstalled: gripTestDBPath is not defined";

        return false;
    }

    if (gripTestDataDBPath.isEmpty() || gripTestDataDBPath.isNull())
    {
        if (isDebugMode)
            qDebug() << "GripStrengthManager::isInstalled: gripTestDataDBPath is not defined";

        return false;
    }

    if (backupPath.isEmpty() || backupPath.isNull())
    {
        if (isDebugMode)
            qDebug() << "GripStrengthManager::isInstalled: backupPath is not defined";

        return false;
    }

    // check if exe is present and executable
    //
    QFileInfo executable(runnableName);
    if (!executable.exists())
    {
        if (isDebugMode)
            qDebug() << "GripStrengthManager::isInstalled - file does not exist at " << runnableName;

        return false;
    }

    if (!executable.isExecutable()) {
        if (isDebugMode)
            qDebug() << "GripStrengthManager::isInstalled - file is not executable at " << runnableName;

        return false;
    }

    return true;
}


bool GripStrengthManager::start()
{
    if (m_debug)
        qDebug() << "GripStrengthManager::start";

    if (m_sim)
    {
        emit started(m_test);
        emit dataChanged(m_test);
        emit canMeasure();

        return true;
    }

    if (!setUp()) {
        return false;
    }

    return true;
}

void GripStrengthManager::readOutput()
{
    if (QProcess::NormalExit != m_process.exitStatus()) {
        emit error("Process failed to finish correctly, cannot read output");
        return;
    }

    ParadoxReader readerResults(m_gripTestDBPath);
    q_paradoxRecords results2 = readerResults.Read();

    QSharedPointer<GripStrengthTest> test = qSharedPointerCast<GripStrengthTest>(m_test);

    for (int i = 0; i < results2.length(); i++)
    {
        test->addMetaData("ExamID", results2[i]["ExamID"].toInt());
        test->addMetaData("TestID", results2[i]["TestID"].toInt());
        test->addMetaData("Test", results2[i]["Test"].toString());
        test->addMetaData("Rung", results2[i]["Rung"].toInt());
        test->addMetaData("Units", results2[i]["Units"].toString());
        test->addMetaData("MaxReps", results2[i]["MaxReps"].toInt());
        test->addMetaData("Sequence", results2[i]["Sequence"].toString());
        test->addMetaData("RestTime", results2[i]["RestTime"].toInt());
        test->addMetaData("Rate", results2[i]["Rate"].toInt());
        test->addMetaData("Threshold", results2[i]["Threshold"].toInt());
        test->addMetaData("PrimaryStat", results2[i]["PrimaryStat"].toString());
        test->addMetaData("NormType", results2[i]["NormType"].toInt());
        test->addMetaData("Comparison", results2[i]["Comparison"].toInt());
    }

    ParadoxReader readerTest(m_gripTestDataDBPath);
    q_paradoxRecords results = readerTest.Read();

    for (int i = 0; i < results.count(); i++)
    {
        QSharedPointer<GripStrengthMeasurement> measurement(new GripStrengthMeasurement);

        measurement->setAttribute("ExamID", results[i]["ExamID"].toInt());
        measurement->setAttribute("TestID", results[i]["TestID"].toInt());
        measurement->setAttribute("Position", results[i]["Position"].toInt());
        measurement->setAttribute("Side", results[i]["Side"].toInt());
        measurement->setAttribute("Rep1", results[i]["Rep1"].toInt());
        measurement->setAttribute("Rep2", results[i]["Rep2"].toInt());
        measurement->setAttribute("Rep3", results[i]["Rep3"].toInt());
        measurement->setAttribute("Rep4", results[i]["Rep4"].toInt());
        measurement->setAttribute("Rep5", results[i]["Rep5"].toInt());
        measurement->setAttribute("Rep6", results[i]["Rep6"].toInt());
        measurement->setAttribute("Rep7", results[i]["Rep7"].toInt());
        measurement->setAttribute("Rep8", results[i]["Rep8"].toInt());
        measurement->setAttribute("Rep9", results[i]["Rep9"].toInt());
        measurement->setAttribute("Rep10", results[i]["Rep10"].toInt());

        measurement->setAttribute("Rep1Exclude", results[i]["Rep1Exclude"].toBool());
        measurement->setAttribute("Rep2Exclude", results[i]["Rep2Exclude"].toBool());
        measurement->setAttribute("Rep3Exclude", results[i]["Rep3Exclude"].toBool());
        measurement->setAttribute("Rep4Exclude", results[i]["Rep4Exclude"].toBool());
        measurement->setAttribute("Rep5Exclude", results[i]["Rep5Exclude"].toBool());
        measurement->setAttribute("Rep6Exclude", results[i]["Rep6Exclude"].toBool());
        measurement->setAttribute("Rep7Exclude", results[i]["Rep7Exclude"].toBool());
        measurement->setAttribute("Rep8Exclude", results[i]["Rep8Exclude"].toBool());
        measurement->setAttribute("Rep9Exclude", results[i]["Rep9Exclude"].toBool());
        measurement->setAttribute("Rep10Exclude", results[i]["Rep10Exclude"].toBool());

        measurement->setAttribute("Average", results[i]["Average"].toInt());
        measurement->setAttribute("Maximum", results[i]["Maximum"].toInt());
        measurement->setAttribute("CV", results[i]["CV"].toInt());

        m_test->addMeasurement(measurement);
    }

    if (m_debug)
        qDebug() << "GripStrengthManager::readOutput: " << m_test->toJsonObject();
}


void GripStrengthManager::measure()
{
    if (m_debug)
        qDebug() << "GripStrengthManager::measure";

    if (m_sim) {
        clearData();
        m_test->simulate();

        emit dataChanged(m_test);
        emit canFinish();

        return;
    }

    if (m_process.state() != QProcess::NotRunning) {
        QMessageBox::critical(nullptr, "Error", "Program is already running");
        return;
    }

    m_process.start();
    if (!m_process.waitForStarted()) {
        QMessageBox::critical(nullptr, "Error", "Could not start application");
        return;
    }
}

void GripStrengthManager::addManualMeasurement()
{
    if (m_debug)
        qDebug() << "GripStrengthManager::addManualMeasurement";

    QSharedPointer<GripStrengthMeasurement> measurement(new GripStrengthMeasurement);
    m_test->addMeasurement(measurement);

    emit dataChanged(m_test);
}

bool GripStrengthManager::setUp() {
    if (m_debug)
        qDebug() << "GripStrengthManager::setUp";

    cleanUp();
    configureProcess();

    return true;
}

bool GripStrengthManager::cleanUp() {
    if (m_debug)
        qDebug() << "GripStrengthManager::cleanUp";

    m_test->reset();

    // close process
    if (m_process.state() != QProcess::NotRunning) {
        m_process.close();
        m_process.waitForFinished();
    }

    // close database
    if (m_database.isOpen())
        m_database.close();

    // check if backup folder exists, if not then return since we are finished
    QDir backupFolder(m_backupPath);
    if (!backupFolder.exists()) {
        if (m_debug)
            qDebug() << "GripStrengthManager::cleanUp - backup folder doesn't exist";

        return true;
    }

    // remove database folder, will be restored from backup
    QDir databaseFolder(m_databasePath);
    if (!databaseFolder.removeRecursively()) {
        if (m_debug)
            qDebug() << "GripStrengthManager::cleanUp - could not remove database folder";

        return false;
    }

    if (m_debug)
        qDebug() << "GripStrengthManager::cleanUp - database folder removed, restoring backup";

    // copy backup to database folder
    foreach (const QString &fileName, backupFolder.entryList(QDir::Files)) {
        QFile::copy(backupFolder.filePath(fileName), databaseFolder.filePath(fileName));
    }

    if (m_debug)
        qDebug() << "GripStrengthManager::cleanUp - database folder restored from backup";

    // remove backup folder
    if (!backupFolder.removeRecursively())
    {
        if (m_debug)
            qDebug() << "GripStrengthManager::cleanUp - could not remove backup folder";

        return false;
    }

    if (m_debug)
        qDebug() << "GripStrengthManager::cleanUp - backup folder removed";

    return true;
}

void GripStrengthManager::configureProcess()
{
    if (m_debug)
        qDebug() << "GripStrengthManager::configureProcess";

    // connect signals and slots to QProcess one time only
    //
    connect(&m_process, &QProcess::started,
        this, [this]() {

            if (m_debug)
                qDebug() << "GripStrengthManager::process started: " << m_process.arguments().join(" ");

        });

    // error occured with grip strength process
    connect(&m_process, &QProcess::errorOccurred,
        this, [=](QProcess::ProcessError error)
        {
            QStringList s = QVariant::fromValue(error).toString().split(QRegExp("(?=[A-Z])"), Qt::SkipEmptyParts);
            if (m_debug)
                qDebug() << "GripStrengthManager::process error: process error occured: " << s.join(" ").toLower();

            cleanUp();
            setUp();
        });

    connect(&m_process, &QProcess::stateChanged,
    this, [=](QProcess::ProcessState state) {
        QStringList s = QVariant::fromValue(state).toString().split(QRegExp("(?=[A-Z])"), Qt::SkipEmptyParts);
        if (m_debug)
            qDebug() << "GripStrengthManager::process state: " << s.join(" ").toLower();

    });

    // read output after grip strength process ends
    connect(&m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        this, &GripStrengthManager::readOutput);

    m_process.setProgram(m_runnableName);
    m_process.setWorkingDirectory(m_runnablePath);
    m_process.setProcessChannelMode(QProcess::ForwardedChannels);

    if (m_debug)
        qDebug() << "GripStrengthManager::configureProcess - process configured";
}

bool GripStrengthManager::clearData()
{
    if (m_debug)
        qDebug() << "GripStrengthManager::clearData";

    m_test->reset();

    emit dataChanged(m_test);

    return true;
}


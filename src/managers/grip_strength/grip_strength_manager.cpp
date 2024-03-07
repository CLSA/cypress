#include "grip_strength_manager.h"
#include "data/grip_strength/tests/grip_strength_test.h"
#include "auxiliary/file_utils.h"
#include "paradox_reader.h"
#include "auxiliary/tracker5_util.h"

#include "cypress_settings.h"

#include <QThread>
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
    const bool isDebugMode = CypressSettings::isDebugMode();
    const bool isSimMode = CypressSettings::isSimMode();

    if (isSimMode)
        return true;

    const QString runnableName = CypressSettings::readSetting("grip_strength/runnableName").toString();
    const QString runnablePath = CypressSettings::readSetting("grip_strength/runnablePath").toString();

    const QString gripTestDBPath = CypressSettings::readSetting("grip_strength/gripTestDB").toString();
    const QString gripTestDataDBPath = CypressSettings::readSetting("grip_strength/gripTestDataDB").toString();

    const QString databasePath = CypressSettings::readSetting("grip_strength/databasePath").toString();
    const QString backupPath = CypressSettings::readSetting("grip_strength/backupPath").toString();

    if (runnableName.isEmpty() || runnableName.isNull())
    {
        if (isDebugMode)
            qDebug() << "GripStrength: runnableName is not defined";

        return false;
    }

    if (runnablePath.isEmpty() || runnablePath.isNull())
    {
        if (isDebugMode)
            qDebug() << "GripStrength: runnablePath is not defined";

        return false;
    }

    if (databasePath.isEmpty() || databasePath.isNull())
    {
        if (isDebugMode)
            qDebug() << "GripStrength: databasePath is not defined";

        return false;
    }

    if (gripTestDBPath.isEmpty() || gripTestDBPath.isNull())
    {
        if (isDebugMode)
            qDebug() << "GripStrengt: gripTestDBPath is not defined";

        return false;
    }

    if (gripTestDataDBPath.isEmpty() || gripTestDataDBPath.isNull())
    {
        if (isDebugMode)
            qDebug() << "GripStrength: gripTestDataDBPath is not defined";

        return false;
    }

    if (backupPath.isEmpty() || backupPath.isNull())
    {
        if (isDebugMode)
            qDebug() << "GripStrength: backupPath is not defined";

        return false;
    }

    // check if exe is present and executable
    //
    const QFileInfo executable(runnableName);
    if (!executable.exists())
    {
        if (isDebugMode)
            qDebug() << "GripStrength: file does not exist at " << runnableName;

        return false;
    }

    if (!executable.isExecutable()) {
        if (isDebugMode)
            qDebug() << "GripStrength: file is not executable at " << runnableName;

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

    emit started(m_test);
    emit dataChanged(m_test);

    measure();

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

    ParadoxReader readerTest(m_gripTestDataDBPath);
    q_paradoxRecords results = readerTest.Read();

    auto test = qSharedPointerCast<GripStrengthTest>(m_test);

    for (int i = 0; i < results2.length(); i++)
    {
        test->addMetaData("exam_id",      results2[i]["ExamID"].toInt());
        test->addMetaData("test_id",      results2[i]["TestID"].toInt());
        test->addMetaData("test",         results2[i]["Test"].toString());
        test->addMetaData("rung",         results2[i]["Rung"].toInt());
        test->addMetaData("units",        results2[i]["Units"].toString());
        test->addMetaData("max_reps",     results2[i]["MaxReps"].toInt());
        test->addMetaData("sequence",     results2[i]["Sequence"].toString());
        test->addMetaData("rest",         results2[i]["RestTime"].toInt());
        test->addMetaData("rate",         results2[i]["Rate"].toInt());
        test->addMetaData("threshold",    Tracker5Util::asKg(results2[i]["Threshold"].toInt()));
        test->addMetaData("primary_stat", results2[i]["PrimaryStat"].toString());
        test->addMetaData("norm_type",    results2[i]["NormType"].toInt());
        test->addMetaData("comparison",   results2[i]["Comparison"].toInt());

        test->addMetaData("average", Tracker5Util::asKg(results[0]["Average"].toInt()));
        test->addMetaData("maximum", Tracker5Util::asKg(results[0]["Maximum"].toInt()));
        test->addMetaData("cv",      results[0]["CV"].toInt());
    }

    for (int i = 0; i < results.count(); i++)
    {
        QSharedPointer<GripStrengthMeasurement> measurement(new GripStrengthMeasurement);
        measurement->setAttribute("position", results[i]["Position"].toInt());
        measurement->setAttribute("side",     results[i]["Side"].toInt());

        bool rep1Exclude = results[i]["Rep1Exclude"].toBool();
        bool rep2Exclude = results[i]["Rep2Exclude"].toBool();
        bool rep3Exclude = results[i]["Rep3Exclude"].toBool();
        bool rep4Exclude = results[i]["Rep4Exclude"].toBool();
        bool rep5Exclude = results[i]["Rep5Exclude"].toBool();
        bool rep6Exclude = results[i]["Rep6Exclude"].toBool();
        bool rep7Exclude = results[i]["Rep7Exclude"].toBool();
        bool rep8Exclude = results[i]["Rep8Exclude"].toBool();
        bool rep9Exclude = results[i]["Rep9Exclude"].toBool();
        bool rep10Exclude = results[i]["Rep10Exclude"].toBool();

        double rep1 = Tracker5Util::asKg(results[i]["Rep1"].toInt());
        double rep2 = Tracker5Util::asKg(results[i]["Rep2"].toInt());
        double rep3 = Tracker5Util::asKg(results[i]["Rep3"].toInt());
        double rep4 = Tracker5Util::asKg(results[i]["Rep4"].toInt());
        double rep5 = Tracker5Util::asKg(results[i]["Rep5"].toInt());
        double rep6 = Tracker5Util::asKg(results[i]["Rep6"].toInt());
        double rep7 = Tracker5Util::asKg(results[i]["Rep7"].toInt());
        double rep8 = Tracker5Util::asKg(results[i]["Rep8"].toInt());
        double rep9 = Tracker5Util::asKg(results[i]["Rep9"].toInt());
        double rep10 = Tracker5Util::asKg(results[i]["Rep10"].toInt());

        if (!rep1Exclude && rep1 > 0)
            measurement->setAttribute("rep1", rep1, "kg", 2);
        if (!rep2Exclude && rep2 > 0)
            measurement->setAttribute("rep2", rep2, "kg", 2);
        if (!rep3Exclude && rep3 > 0)
            measurement->setAttribute("rep3", rep3, "kg", 2);
        if (!rep4Exclude && rep4 > 0)
            measurement->setAttribute("rep4", rep4, "kg", 2);
        if (!rep5Exclude && rep5 > 0)
            measurement->setAttribute("rep5", rep5, "kg", 2);
        if (!rep6Exclude && rep6 > 0)
            measurement->setAttribute("rep6", rep6, "kg", 2);
        if (!rep7Exclude && rep7 > 0)
            measurement->setAttribute("rep7", rep7, "kg", 2);
        if (!rep8Exclude && rep8 > 0)
            measurement->setAttribute("rep8", rep8, "kg", 2);
        if (!rep9Exclude && rep9 > 0)
            measurement->setAttribute("rep9", rep9, "kg", 2);
        if (!rep10Exclude && rep10 > 0)
            measurement->setAttribute("rep10", rep10, "kg", 2);

        m_test->addMeasurement(measurement);
    }

    if (m_debug)
        qDebug() << "GripStrengthManager::readOutput: " << m_test->toJsonObject();

    QThread::sleep(5);
    cleanUp();
    finish();
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
        emit error("Program is already running");
        return;
    }

    m_process.start();

    if (!m_process.waitForStarted()) {
        emit error("Could not start application");
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

    if (!backupData()) {
        return false;
    }

    configureProcess();

    return true;
}

bool GripStrengthManager::backupData() {
    if (m_debug)
        qDebug() << "GripStrengthManager::backupData";

    if (!FileUtils::copyDirectory(QDir(m_databasePath), QDir(m_backupPath), true, false)) {
        qDebug() << "failed to backup data";
        return false;
    }

    return true;
}

bool GripStrengthManager::restoreData() {
    if (m_debug)
        qDebug() << "GripStrengthManager::restoreData";

    if (!FileUtils::copyDirectory(QDir(m_backupPath), QDir(m_databasePath), true, false)) {
        qDebug() << "failed to restore data";
        return false;
    }

    return true;
}


bool GripStrengthManager::cleanUp() {

    if (QProcess::NotRunning != m_process.state())
        m_process.close();

    if (m_debug)
        qDebug() << "GripStrengthManager::cleanUp";

    if (!restoreData())
        return false;

    return true;
}

void GripStrengthManager::configureProcess()
{
    if (m_debug)
        qDebug() << "GripStrengthManager::configureProcess" << m_runnableName << m_runnablePath;

    m_process.setProgram(m_runnableName);
    m_process.setWorkingDirectory(m_runnablePath);
    m_process.setProcessChannelMode(QProcess::ForwardedChannels);

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
}

bool GripStrengthManager::clearData()
{
    if (m_debug)
        qDebug() << "GripStrengthManager::clearData";

    m_test->reset();

    return true;
}


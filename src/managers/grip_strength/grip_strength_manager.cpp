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

DeviceConfig GripStrengthManager::config {{
   { "runnableName",     { "grip_strength/runnableName",     Exe  }},
   { "runnablePath",     { "grip_strength/runnablePath",     Dir  }},
   { "gripTestDB",       { "grip_strength/gripTestDB",       File }},
   { "gripTestDataDB",   { "grip_strength/gripTestDataDB",   File }},
   { "backupPath",   	 { "grip_strength/backupPath",   	 Dir  }},
   { "databasePath",     { "grip_strength/databasePath",     Dir  }},
}};

GripStrengthManager::GripStrengthManager(QSharedPointer<GripStrengthSession> session)
    : ManagerBase(session) {
    m_runnableName = config.getSetting("runnableName");
    m_runnablePath = config.getSetting("runnablePath");

    m_gripTestDBPath = config.getSetting("gripTestDB");
    m_gripTestDataDBPath = config.getSetting("gripTestDataDB");

    m_backupPath = config.getSetting("backupPath");
    m_databasePath = config.getSetting("databasePath");

    m_test.reset(new GripStrengthTest);
}

bool GripStrengthManager::start() {
    qInfo() << "GripStrengthManager::start";

    if (!setUp())
        return false;

    measure();

    return true;
}

void GripStrengthManager::readOutput() {
    qInfo() << "GripStrengthManager::readOutput";

    if (QProcess::NormalExit != m_process.exitStatus()) {
        emit error("Process failed to finish correctly, cannot read output");
        return;
    }

    ParadoxReader readerResults(m_gripTestDBPath);
    q_paradoxRecords results2 = readerResults.Read();

    ParadoxReader readerTest(m_gripTestDataDBPath);
    q_paradoxRecords results = readerTest.Read();

    auto test = qSharedPointerCast<GripStrengthTest>(m_test);

    for (int i = 0; i < results2.length(); i++) {
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

    for (int i = 0; i < results.count(); i++) {
        QSharedPointer<GripStrengthMeasurement> measurement(new GripStrengthMeasurement);
        measurement->setAttribute("position", results[i]["Position"].toInt());
        measurement->setAttribute("side",     results[i]["Side"].toInt());

        const bool rep1Exclude = results[i]["Rep1Exclude"].toBool();
        const bool rep2Exclude = results[i]["Rep2Exclude"].toBool();
        const bool rep3Exclude = results[i]["Rep3Exclude"].toBool();
        const bool rep4Exclude = results[i]["Rep4Exclude"].toBool();
        const bool rep5Exclude = results[i]["Rep5Exclude"].toBool();
        const bool rep6Exclude = results[i]["Rep6Exclude"].toBool();
        const bool rep7Exclude = results[i]["Rep7Exclude"].toBool();
        const bool rep8Exclude = results[i]["Rep8Exclude"].toBool();
        const bool rep9Exclude = results[i]["Rep9Exclude"].toBool();
        const bool rep10Exclude = results[i]["Rep10Exclude"].toBool();

        const double rep1 = Tracker5Util::asKg(results[i]["Rep1"].toInt());
        const double rep2 = Tracker5Util::asKg(results[i]["Rep2"].toInt());
        const double rep3 = Tracker5Util::asKg(results[i]["Rep3"].toInt());
        const double rep4 = Tracker5Util::asKg(results[i]["Rep4"].toInt());
        const double rep5 = Tracker5Util::asKg(results[i]["Rep5"].toInt());
        const double rep6 = Tracker5Util::asKg(results[i]["Rep6"].toInt());
        const double rep7 = Tracker5Util::asKg(results[i]["Rep7"].toInt());
        const double rep8 = Tracker5Util::asKg(results[i]["Rep8"].toInt());
        const double rep9 = Tracker5Util::asKg(results[i]["Rep9"].toInt());
        const double rep10 = Tracker5Util::asKg(results[i]["Rep10"].toInt());

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

    qDebug() << m_test->toJsonObject();

    QThread::sleep(5); // give time for tracker exe to cleanup
    finish();
}

void GripStrengthManager::measure() {
    qDebug() << "GripStrengthManager::measure";

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

bool GripStrengthManager::setUp() {
    qInfo() << "GripStrengthManager::setUp";

    if (!backupData())
        return false;

    configureProcess();

    return true;
}

bool GripStrengthManager::backupData() {
    qInfo() << "GripStrengthManager::backupData";

    if (!FileUtils::copyDirectory(QDir(m_databasePath), QDir(m_backupPath), true, false)) {
        qDebug() << "failed to backup data";
        return false;
    }

    return true;
}

bool GripStrengthManager::restoreData() {
    qInfo() << "GripStrengthManager::restoreData";

    if (!FileUtils::copyDirectory(QDir(m_backupPath), QDir(m_databasePath), true, false)) {
        qWarning() << "failed to restore data";
        return false;
    }

    return true;
}


bool GripStrengthManager::cleanUp() {
    qInfo() << "GripStrengthManager::cleanUp";

    if (QProcess::NotRunning != m_process.state())
        m_process.close();

    if (!restoreData())
        return false;

    return true;
}

void GripStrengthManager::configureProcess() {
    qInfo() << "GripStrengthManager::configureProcess" << m_runnableName << m_runnablePath;

    m_process.setProgram(m_runnableName);
    m_process.setWorkingDirectory(m_runnablePath);
    m_process.setProcessChannelMode(QProcess::ForwardedChannels);

    // connect signals and slots to QProcess one time only
    //
    connect(&m_process, &QProcess::started,
        this, [this]() {
            qInfo() << "GripStrengthManager::process started: " << m_process.arguments().join(" ");
        });

    // error occured with grip strength process
    connect(&m_process, &QProcess::errorOccurred,
        this, [=](QProcess::ProcessError error)
        {
            QStringList s = QVariant::fromValue(error).toString().split(QRegExp("(?=[A-Z])"), Qt::SkipEmptyParts);
            qWarning() << "GripStrengthManager::process error: process error occured: " << s.join(" ").toLower();
        });

    connect(&m_process, &QProcess::stateChanged,
    this, [=](QProcess::ProcessState state) {
        QStringList s = QVariant::fromValue(state).toString().split(QRegExp("(?=[A-Z])"), Qt::SkipEmptyParts);
        qInfo() << "GripStrengthManager::process state: " << s.join(" ").toLower();

    });

    // read output after grip strength process ends
    connect(&m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        this, &GripStrengthManager::readOutput);
}

bool GripStrengthManager::clearData() {
    qInfo() << "GripStrengthManager::clearData";
    m_test->reset();
    return true;
}


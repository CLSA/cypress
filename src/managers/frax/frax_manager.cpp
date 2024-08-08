#include "frax_manager.h"
#include "data/frax/tests/frax_test.h"
#include "auxiliary/Utilities.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>
#include <QStandardItemModel>
#include <QMessageBox>

DeviceConfig FraxManager::config {{
    { "runnableName",       { "frax/runnableName",       Exe }},
    { "runnablePath",       { "frax/runnablePath",       Dir }},
    { "outputFilePath",     { "frax/outputFilePath",     NonEmptyString }},
    { "inputFilePath",      { "frax/inputFilePath",      NonEmptyString }},
    { "countryCode",        { "frax/countryCode",        NonEmptyString }},
    { "typeCode",           { "frax/typeCode",           NonEmptyString }},
}};

FraxManager::FraxManager(QSharedPointer<FraxSession> session)
    : ManagerBase(session)
{
    m_runnableName = config.getSetting("runnableName");
    m_runnablePath = config.getSetting("runnablePath");
    m_outputFilePath = config.getSetting("outputFilePath");
    m_inputFilePath = config.getSetting("inputFilePath");

    m_country_code = config.getSetting("countryCode");
    m_type_code = config.getSetting("typeCode");

    m_test.reset(new FraxTest);

    // key/value pairs needed to build input.txt
    //

}

bool FraxManager::start()
{
    qDebug() << "FraxManager::start";

    if (!setUp()) {
        emit error("Could not setup FRAX device, please contact support.");
        return false;
    }

    measure();
    return true;
}

void FraxManager::measure()
{
    qDebug() << "FraxManager::measure";

    clearData();

    if (m_process.state() != QProcess::NotRunning) {
        QMessageBox::critical(nullptr, "Error", "Program is already running");
        return;
    }

    m_process.start();

    if (!m_process.waitForStarted()) {
        QMessageBox::critical(nullptr, "Error", "Could not launch FRAX");
        return;
    }
}

void FraxManager::readOutput()
{
    qDebug() << "FraxManager::readOutput";

    auto test = qSharedPointerCast<FraxTest>(m_test);

    if (!QFileInfo::exists(m_outputFilePath)) {
        emit error("FRAX results not found");
        return;
    }

    test->fromFile(m_outputFilePath);

    qDebug() << test->toJsonObject();

    finish();
}

void FraxManager::configureProcess()
{
    qDebug() << "FraxManager::configureProcess";

    // connect signals and slots to QProcess one time only
    //
    connect(&m_process, &QProcess::started, this, [this]() {
        qDebug() << "process started: " << m_process.arguments().join(" ");
    });

    // read output from csv after process finishes
    connect(&m_process,
            QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this,
            &FraxManager::readOutput);

    // error occured,
    connect(&m_process, &QProcess::errorOccurred, this, [=](QProcess::ProcessError error) {
        QStringList s = QVariant::fromValue(error).toString().split(QRegExp("(?=[A-Z])"),
                                                                    Qt::SkipEmptyParts);
        qDebug() << "ERROR: process error occured: " << s.join(" ").toLower();
    });

    connect(&m_process, &QProcess::stateChanged, this, [=](QProcess::ProcessState state) {
        QStringList s = QVariant::fromValue(state).toString().split(QRegExp("(?=[A-Z])"),
                                                                    Qt::SkipEmptyParts);
        qDebug() << "process state: " << s.join(" ").toLower();
    });

    // blackbox.exe and input.txt file are present
    //
    QDir workingDirectory(m_runnablePath);
    QFileInfo fraxExecutableInfo(m_runnableName);

    if (!workingDirectory.exists()) {
        emit error("working directory does not exist");
        return;
    }

    if (!fraxExecutableInfo.exists()) {
        emit error("frax exe does not exist");
        return;
    }

    m_process.setProgram(m_runnableName);
    m_process.setWorkingDirectory(m_runnablePath);

    QStringList list;
    QJsonObject inputData = m_session->getInputData();

    list << inputData.value("type").toString();
    list << inputData.value("country_code").toString();
    list << inputData.value("age").toString();
    list << (inputData.value("sex").toString().toLower() == 'm' ? "0" : "1");
    list << inputData.value("body_mass_index").toString();
    list << inputData.value("previous_fracture").toString();
    list << inputData.value("parent_hip_fracture").toString();
    list << inputData.value("current_smoker").toString();
    list << inputData.value("glucocorticoid").toString();
    list << inputData.value("rheumatoid_arthritis").toString();
    list << inputData.value("secondary_osteoporosis").toString();
    list << inputData.value("alcohol").toString();
    list << inputData.value("femoral_neck_bmd").toString();

    // Write input.txt file
    QString line = list.join(",");
    qDebug() << line;

    QFile input_file(m_inputFilePath);

    if (input_file.exists()) {
        if (!input_file.remove()) {
            qDebug() << "could not remove file";
        }
    }

    if(input_file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(&input_file);
        stream << line << Qt::endl;
        input_file.close();

        qDebug() << "populated input.txt file " << m_inputFilePath;
        qDebug() << "content = " << line;
    }
    else
        emit error("Failed writing to input file");
}

bool FraxManager::clearData()
{
    qDebug() << "FraxManager::clearData";

    m_test->reset();
    return true;
}


// Set up device
bool FraxManager::setUp()
{
    qDebug() << "FraxManager::setUp";

    clearData();

    if (!cleanUp())
        return false;

    configureProcess();

    return true;
}

// Clean up the device for next time
bool FraxManager::cleanUp()
{
    // remove blackbox.exe generated output.txt file
    //
    qDebug() << "FraxManager::cleanUp";

    if(QProcess::NotRunning != m_process.state())
        m_process.close();

    if (QFileInfo::exists(m_outputFilePath)) {
        QFile outputFile(m_outputFilePath);
        if (!outputFile.remove())
            qDebug() << "could not remove output file";
        else
            qDebug() << "removed output file";
    }

    return true;
}

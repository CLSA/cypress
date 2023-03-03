#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QJsonObject>
#include <QProcess>
#include <QSettings>
#include <QStandardItemModel>
#include <QException>

#include "CypressApplication.h"
#include "auxiliary/JsonSettings.h"

#include "GripStrengthManager.h"

GripStrengthManager::GripStrengthManager()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");

    // all managers must check for barcode and language input values
    //
    m_workingDir   = 	 settings.value("instruments/grip_strength/workingDir").toString();
    m_runnableName = 	 settings.value("instruments/grip_strength/runnableName").toString();
    m_databaseDir  = 	 settings.value("instruments/grip_strength/databaseDir").toString();
    m_databaseName = 	 settings.value("instruments/grip_strength/databaseName").toString();
    m_databaseHost = 	 settings.value("instruments/grip_strength/databaseHost").toString();
    m_databasePort = 	 settings.value("instruments/grip_strength/databasePort").toString();
    m_databaseUser =     settings.value("instruments/grip_strength/databaseUser").toString();
    m_databasePassword = settings.value("instruments/grip_strength/databasePassword").toString();
}


bool GripStrengthManager::isAvailable()
{
    return false;
}


void GripStrengthManager::start()
{
    if (CypressApplication::mode == Mode::Sim) return;

    initializeConnections();
    //initializeModel();
    //configureProcess();
    bool ok = readOutput();

    if (!ok)
    {
        qDebug("Could not read output");
    }
}

void GripStrengthManager::measure()
{
    if (CypressApplication::mode == Mode::Sim) return;

    try {
        readOutput();
        qDebug() << m_test.toJsonObject();
        //updateModel();
    }
    catch (QException &e) {
        qDebug() << e.what();
    }
}

void GripStrengthManager::finish()
{
    if (CypressApplication::mode == Mode::Sim)
    {
        QJsonObject results = JsonSettings::readJsonFromFile(
            "C:/work/clsa/cypress/src/tests/fixtures/grip_strength/output.json"
        );
        if (results.empty()) return;

        bool ok = sendResultsToPine(results);
        if (!ok)
        {
            qDebug() << "Could not send results to Pine";
        }

        CypressApplication::status = Status::Waiting;
    }
}

bool GripStrengthManager::initializeConnections() {
    try {
        connect(&m_process, &QProcess::started, this, [this]() {
            qDebug() << "Tracker 5 started: " << m_process.arguments().join(" ");
        });

        connect(&m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &GripStrengthManager::canMeasure);

        connect(this, &GripStrengthManager::canMeasure, this, &GripStrengthManager::measureStart);

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

    }
    catch (QException& e)
    {
        qDebug() << e.what();
        return false;
    }

    return true;
}


bool GripStrengthManager::readOutput()
{
    bool ok = m_test.readGripTestResults();

    return ok;
}

bool GripStrengthManager::isDefined(const QString& value, const GripStrengthManager::FileType& fileType) const
{
    if (value.isEmpty())
        return false;

    QFileInfo info(value);
    bool ok = info.exists();
    if (fileType == GripStrengthManager::FileType::Tracker5Exe)
    {
        if ("exe" != info.completeSuffix())
        {
            ok = false;
        }
    }
    else if (fileType == GripStrengthManager::FileType::GripTestDbPath
        || fileType == GripStrengthManager::FileType::GripTestDataDbPath)
    {
        if ("DB" != info.completeSuffix())
        {
            ok = false;
        }
    }
    return ok;
}

bool GripStrengthManager::processStart()
{
    try {
        m_process.start();
        return m_process.waitForStarted();
    }
    catch (const std::exception& e) {
        qDebug() << e.what();
    }

    return false;
}

bool GripStrengthManager::clearData()
{
    //m_test.reset();
    return false;
    //updateModel();
}

bool GripStrengthManager::configureProcess() {
    QDir working(m_runnableName);
    if (!working.exists())
    {
        qDebug() << "working does not exist.";

        return false;
    }

    try {
        m_process.setProgram(m_runnableName);
        m_process.setWorkingDirectory(working.absolutePath());

        emit processInitialized();

        return true;
    }
    catch (QException& e)
    {
        qDebug() << e.what();
    }

    return false;
}

// Set up device
bool GripStrengthManager::setUp()
{
    return true;
}

// Clean up the device for next time
bool GripStrengthManager::cleanUp()
{
    return true;
}

void GripStrengthManager::setInputData(const QVariantMap& inputData)
{

}

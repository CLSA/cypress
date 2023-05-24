#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QJsonObject>
#include <QSettings>
#include <QStandardItemModel>

#include "CypressApplication.h"
#include "auxiliary/JsonSettings.h"

#include "FraxManager.h"

FraxManager::FraxManager(QJsonObject inputData)
{
    m_test.setExpectedMeasurementCount(4);
    m_inputData = jsonObjectToVariantMap(inputData);
}

bool FraxManager::isAvailable()
{
    return false;
}

void FraxManager::start()
{
    // connect signals and slots to QProcess one time only
    //
    //connect(&m_process, &QProcess::started,
    //    this, [this]() {
    //        qDebug() << "process started: " << m_process.arguments().join(" ");
    //    });

    //connect(&m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
    //    this, &FraxManager::readOutput);

    //connect(&m_process, &QProcess::errorOccurred,
    //    this, [](QProcess::ProcessError error)
    //    {
    //        QStringList s = QVariant::fromValue(error).toString().split(QRegExp("(?=[A-Z])"), Qt::SkipEmptyParts);
    //        qDebug() << "ERROR: process error occured: " << s.join(" ").toLower();
    //    });

    //connect(&m_process, &QProcess::stateChanged,
    //    this, [](QProcess::ProcessState state) {
    //        QStringList s = QVariant::fromValue(state).toString().split(QRegExp("(?=[A-Z])"), Qt::SkipEmptyParts);
    //        qDebug() << "process state: " << s.join(" ").toLower();
    //    });

    //m_process.setProcessChannelMode(QProcess::ForwardedChannels);

    //configureProcess();
    //emit dataChanged();
}


bool FraxManager::isDefined(const QString &exeName) const
{
    //bool ok = false;
    //if(!exeName.isEmpty())
    //{
    //    QFileInfo info(exeName);
    //    if(info.exists() && info.isExecutable())
    //    {
    //        ok = true;
    //    }
    //}
    return true;
}

void FraxManager::selectRunnable(const QString &runnableName)
{
    //if(isDefined(runnableName))
    //{
    //    QFileInfo info(runnableName);
    //    m_runnableName = runnableName;
    //    m_runnablePath = info.absolutePath();
    //    m_outputFile = QDir(m_runnablePath).filePath("output.txt");
    //    m_inputFile =  QDir(m_runnablePath).filePath("input.txt");
    //    m_temporaryFile = QDir(m_runnablePath).filePath("input_ORIG.txt");

    //    emit runnableSelected();
    //    configureProcess();
    //}
    //else
    //    emit canSelectRunnable();
}

void FraxManager::measure()
{
    //if (CypressApplication::getInstance().isSimulation()) return;
    //clearData();
    //// launch the process
    //m_process.start();

    if (CypressApplication::getInstance().isSimulation())
    {
        sendJsonData("C:/work/clsa/cypress/src/tests/fixtures/frax/output.json");
    }
    //QJsonObject results = JsonSettings::readJsonFromFile(
    //);

    //results["cypress_session"] = m_uuid;
    //results["answer_id"] = m_answerId;
    //results["barcode"] = m_barcode;
    //results["interviewer"] = m_interviewer;

    //if (results.empty()) return;

    //bool ok = sendResultsToPine(results);
    //if (!ok)
    //{
    //    qDebug() << "Could not send results to Pine";
    //}
}

void FraxManager::readOutput()
{
    //if (QProcess::NormalExit != m_process.exitStatus())
    //{
    //    qDebug() << "ERROR: process failed to finish correctly: cannot read output";
    //    return;
    //}
    //else
    //    qDebug() << "process finished successfully";

    //if(QFileInfo::exists(m_outputFile))
    //{
    //    qDebug() << "found output txt file " << m_outputFile;
    //    m_test.fromFile(m_outputFile);
    //    if(m_test.isValid())
    //    {
    //        emit canFinish();
    //    }
    //    else
    //        qDebug() << "ERROR: input from file produced invalid test results";
    //}
    //else
    //    qDebug() << "ERROR: no output.txt file found";
}

void FraxManager::configureProcess()
{
}

bool FraxManager::clearData()
{
    //m_test.reset();
    return false;
}

void FraxManager::finish()
{
    //if (CypressApplication::getInstance().isSimulation())
    //{
    //    QJsonObject results = JsonSettings::readJsonFromFile(
    //        "C:/work/clsa/cypress/src/tests/fixtures/frax/output.json"
    //    );
    //    if (results.empty()) return;

    //    bool ok = sendResultsToPine(results);
    //    if (!ok)
    //    {
    //        qDebug() << "Could not send results to Pine";
    //    }
    //}
}

// Set up device
bool FraxManager::setUp()
{
    return true;
}

// Clean up the device for next time
bool FraxManager::cleanUp()
{
    return true;
}

void FraxManager::setInputData(const QVariantMap& inputData)
{
    Q_UNUSED(inputData)
}

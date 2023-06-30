
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QFileDialog>
#include <QJsonObject>
#include <QSettings>
#include <QStandardItemModel>

#include "cypress_application.h"
#include "auxiliary/json_settings.h"

#include "ecg_manager.h"

ECGManager::ECGManager(QJsonObject inputData)
{
    m_test.setExpectedMeasurementCount(1);
    m_inputData = jsonObjectToVariantMap(inputData);
}

bool ECGManager::isAvailable()
{
    return false;
}

bool ECGManager::isInstalled()
{
    return false;
}

void ECGManager::start()
{
    // connect signals and slots to QProcess one time only
    //
    //connect(&m_process, &QProcess::started,
    //    this, [this]() {
    //        qDebug() << "process started: " << m_process.arguments().join(" ");
    //    });

    //connect(&m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
    //    this, &ECGManager::readOutput);

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

bool ECGManager::isDefined(const QString &fileName, const FileType &type) const
{
    Q_UNUSED(fileName)
    Q_UNUSED(type)

    return false;
}

void ECGManager::selectRunnable(const QString &runnableName)
{
    Q_UNUSED(runnableName)
}

void ECGManager::selectWorking(const QString& workingName)
{
    Q_UNUSED(workingName)
}

void ECGManager::select()
{

}

void ECGManager::measure()
{
    if (Cypress::getInstance().isSimulation()) {
        sendResultsToPine("C:/dev/clsa/cypress/src/tests/fixtures/ecg/output.json");
        return;
    }


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

    //if (CypressApplication::getInstance().isSimulation()) return;

    //clearData();
    //// launch the process
    //qDebug() << "starting process from measure";
    //m_process.start();
}

void ECGManager::readOutput()
{

}

void ECGManager::configureProcess()
{

}

bool ECGManager::clearData()
{
    m_test.reset();
    return false;
}

void ECGManager::finish()
{
    //QJsonObject results = JsonSettings::readJsonFromFile(
    //    "C:/dev/clsa/cypress/src/tests/fixtures/ecg/output.json"
    //);
    //if (results.empty()) return;

    //bool ok = sendResultsToPine(results);
    //if (!ok)
    //{
    //    qDebug() << "Could not send results to Pine";
    //}
}

bool ECGManager::deleteDeviceData()
{
   return false;
}

// Set up device
bool ECGManager::setUp()
{
    return true;
}

// Clean up the device for next time
bool ECGManager::cleanUp()
{
    return true;
}

void ECGManager::setInputData(const QVariantMap& inputData)
{
    Q_UNUSED(inputData)
}


#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QJsonObject>
#include <QProcess>
#include <QSettings>
#include <QSqlDatabase>
#include <QStandardItemModel>

#include "CypressApplication.h"
#include "auxiliary/JsonSettings.h"

#include "CDTTManager.h"

CDTTManager::CDTTManager(QJsonObject inputData)
{
    m_test.setMinimumMeasurementCount(1);
    m_inputData = jsonObjectToVariantMap(inputData);
    qDebug() << "CDTT input data: " << m_inputData;
}

CDTTManager::~CDTTManager()
{
    QSqlDatabase::removeDatabase("xlsx_connection");
}

bool CDTTManager::isAvailable()
{
    return false;
}

void CDTTManager::start()
{
    // connect signals and slots to QProcess one time only
    //
    //connect(&m_process, &QProcess::started,
    //    this, [this]() {
    //        qDebug() << "process started: " << m_process.arguments().join(" ");
    //    });

    //connect(&m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
    //    this, &CDTTManager::readOutput);

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

void CDTTManager::measure()
{
    if (CypressApplication::getInstance().isSimulation()) {
        sendJsonData("C:/work/clsa/cypress/src/tests/fixtures/cdtt/output.json");
    };

    //if (results.empty()) return;

    //results["cypress_session"] = m_uuid;
    //results["answer_id"] = m_answerId;
    //results["barcode"] = m_barcode;
    //results["interviewer"] = m_interviewer;

    //bool ok = sendResultsToPine(results);
    //if (!ok)
    //{
    //    qDebug() << "Could not send results to Pine";
    //}

    //clearData();

    //qDebug() << "starting process from measure";
    //m_process.start();
}

bool CDTTManager::clearData()
{
    //m_test.reset();
    return true;
}

void CDTTManager::finish()
{
    //if (CypressApplication::getInstance().isSimulation())
    //{
    //    QJsonObject results = JsonSettings::readJsonFromFile(
    //        "C:/work/clsa/cypress/src/tests/fixtures/cdtt/output.json"
    //    );
    //    if (results.empty()) return;

    //    bool ok = sendResultsToPine(results);
    //    if (!ok)
    //    {
    //        qDebug() << "Could not send results to Pine";
    //    }
    //}
}

void CDTTManager::configureProcess()
{

}

// Set up device
bool CDTTManager::setUp()
{
    return true;
}

// Clean up the device for next time
bool CDTTManager::cleanUp()
{
    return true;
}

void CDTTManager::setInputData(const QVariantMap& inputData)
{
    Q_UNUSED(inputData)
}


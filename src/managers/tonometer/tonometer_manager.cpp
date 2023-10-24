#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonObject>
#include <QSettings>
#include <QSqlDatabase>
#include <QStandardItemModel>

#include "cypress_application.h"
#include "auxiliary/json_settings.h"
//#include "data/AccessQueryHelper.h"

#include "tonometer_manager.h"

TonometerManager::TonometerManager(const CypressSession& session)
    : ManagerBase(session), m_test(new TonometerTest)
{
    m_test->setExpectedMeasurementCount(2);
}

TonometerManager::~TonometerManager()
{
  QSqlDatabase::removeDatabase("mdb_connection");
}

bool TonometerManager::isAvailable()
{
  return false;
}

bool TonometerManager::isInstalled()
{
  return false;
}

void TonometerManager::start()
{
    emit started(m_test);
    emit canMeasure();

    // connect signals and slots to QProcess one time only
    //
    //connect(&m_process, &QProcess::started,
    //    this, [this]() {
    //        qDebug() << "process started: " << m_process.arguments().join(" ");
    //    });

    //connect(&m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
    //    this, &TonometerManager::readOutput);

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

    //configureProcess();
    //emit dataChanged();
}


bool TonometerManager::isDefined(const QString& fileName, const TonometerManager::FileType& type) const
{
    bool ok = false;
    QFileInfo info(fileName);
    if(type == TonometerManager::FileType::ORAApplication)
    {
      ok = info.isExecutable() && info.exists();
    }
    else
    {
      ok = info.isFile() && info.exists();
    }
    return ok;
}

void TonometerManager::select()
{
}

void TonometerManager::selectRunnable(const QString &exeName)
{
    Q_UNUSED(exeName)
}

void TonometerManager::selectDatabase(const QString &dbName)
{
    Q_UNUSED(dbName)
}

void TonometerManager::measure()
{
    m_test->reset();
    m_test->simulate({});

    emit measured(m_test);
    emit canFinish();

    //QJsonObject response {
    //    {"uuid", m_uuid,},
    //    {"answer_id", m_answerId }
    //};

    //sendResultsToPine(response);
    //if (Cypress::getInstance().isSimulation()) {
    //  sendResultsToPine("C:/dev/clsa/cypress/src/tests/fixtures/tonometer/output.json");
    //}

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

void TonometerManager::readOutput()
{

}

void TonometerManager::configureProcess()
{

}

bool TonometerManager::clearData()
{
    m_test->reset();
    return false;
}

void TonometerManager::finish()
{
    QJsonObject responseJson {};

    int answer_id = m_session.getAnswerId();

    QJsonObject testJson = m_test->toJsonObject();
    testJson.insert("session", m_session.getJsonObject());

    responseJson.insert("value", testJson);

    QJsonDocument jsonDoc(responseJson);
    QByteArray serializedData = jsonDoc.toJson();

    QString host = CypressSettings::getInstance().getPineHost();
    QString endpoint = CypressSettings::getInstance().getPineEndpoint();

    sendHTTPSRequest("PATCH",
                     host + endpoint + QString::number(answer_id),
                     "application/json",
                     serializedData);

    emit success("sent");
}

// set input parameters for the test
void TonometerManager::setInputData(const QVariantMap& inputData)
{
    Q_UNUSED(inputData)
}

bool TonometerManager::setUp()
{
    return true;
}

bool TonometerManager::cleanUp()
{
    return true;
}

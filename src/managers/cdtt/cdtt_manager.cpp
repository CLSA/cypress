#include "cypress_application.h"
#include "cdtt_manager.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>
#include <QSettings>
#include <QSqlDatabase>
#include <QStandardItemModel>

CDTTManager::CDTTManager(QSharedPointer<CDTTSession> session)
    : ManagerBase(session)
    , m_test(new CDTTTest)
{
    m_test->setMinimumMeasurementCount(1);
}

CDTTManager::~CDTTManager()
{
    QSqlDatabase::removeDatabase("xlsx_connection");
}

bool CDTTManager::isAvailable()
{
    return false;
}

bool CDTTManager::isInstalled()
{
    return false;
}

void CDTTManager::start()
{
    m_test->reset();

    emit started(m_test);
    emit canMeasure();

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
    m_test->reset();

    m_test->simulate({});

    emit measured(m_test);
    emit canFinish();
    //if (Cypress::getInstance().isSimulation()) {
    //    sendResultsToPine("C:/dev/clsa/cypress/src/tests/fixtures/cdtt/output.json");
    //};



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
    QJsonObject responseJson {};

    int answer_id = m_session->getAnswerId();

    QJsonObject testJson = m_test->toJsonObject();
    testJson.insert("session", m_session->getJsonObject());

    responseJson.insert("value", testJson);

    QJsonDocument jsonDoc(responseJson);
    QByteArray serializedData = jsonDoc.toJson();

    QString answerUrl = CypressSettings::getInstance().getAnswerUrl(answer_id);
    sendHTTPSRequest("PATCH", answerUrl, "application/json", serializedData);

    emit success("Measurements saved to Pine");
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


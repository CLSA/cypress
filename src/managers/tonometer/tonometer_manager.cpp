//#include "data/AccessQueryHelper.h"
#include "cypress_application.h"

#include "tonometer_manager.h"
#include "server/sessions/tonometer_session.h"

#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>
#include <QSqlDatabase>
#include <QStandardItemModel>

TonometerManager::TonometerManager(QSharedPointer<TonometerSession> session)
    : ManagerBase(session)
{
    m_test.reset(new TonometerTest);
    m_test->setExpectedMeasurementCount(2);
}

TonometerManager::~TonometerManager()
{
  QSqlDatabase::removeDatabase("mdb_connection");
}

void TonometerManager::start()
{
    emit started(m_test.get());
    emit canMeasure();

    // connect signals and slots to QProcess one time only
    //
    connect(&m_process, &QProcess::started,
        this, [this]() {
            qDebug() << "process started: " << m_process.arguments().join(" ");
        });

    connect(&m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        this, &TonometerManager::readOutput);

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

    configureProcess();
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

    if (CypressSettings::isSimMode())
    {
        m_test->simulate({});
        emit measured(m_test.get());
        emit canFinish();
    }
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
    QJsonObject responseJson{};

    int answer_id = m_session->getAnswerId();

    QJsonObject testJson = m_test->toJsonObject();
    testJson.insert("session", m_session->getJsonObject());

    responseJson.insert("value", testJson);

    QJsonDocument jsonDoc(responseJson);
    QByteArray serializedData = jsonDoc.toJson();

    QString host = CypressSettings::getPineHost();
    QString endpoint = CypressSettings::getPineEndpoint();

    QString answerUrl = CypressSettings::getAnswerUrl(answer_id);
    sendHTTPSRequest("PATCH", answerUrl, "application/json", serializedData);

    emit success("sent");
}

bool TonometerManager::setUp()
{
    return true;
}

bool TonometerManager::cleanUp()
{
    return true;
}

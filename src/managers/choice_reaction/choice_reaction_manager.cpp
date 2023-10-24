#include "cypress_session.h"
#include "choice_reaction_manager.h"

#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QJsonObject>
#include <QProcess>
#include <QSettings>
#include <QStandardItemModel>
#include <QJsonDocument>

QString ChoiceReactionManager::CCB_PREFIX = "CLSA_ELCV";
QString ChoiceReactionManager::CCB_CLINIC = "CYPRESS";

ChoiceReactionManager::ChoiceReactionManager(QSharedPointer<ChoiceReactionSession> session)
    : ManagerBase(session)
    , m_test(new ChoiceReactionTest)
{
    qDebug() << "ChoiceReactionManager inputData: " << m_inputData;
}

bool ChoiceReactionManager::isAvailable()
{
    return false;
}

bool ChoiceReactionManager::isInstalled()
{
    return false;
}

void ChoiceReactionManager::start()
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
    //    this, &ChoiceReactionManager::readOutput);

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
    //emit dataChanged(_test);
}

void ChoiceReactionManager::configureProcess()
{

}

void ChoiceReactionManager::readOutput()
{

}

void ChoiceReactionManager::measure()
{
    m_test->reset();
    m_test->simulate({});

    emit measured(m_test);
    emit canFinish();
    //if (Cypress::getInstance().isSimulation()) {
    //    sendResultsToPine("C:/dev/clsa/cypress/src/tests/fixtures/choice_reaction/output.json");
    //    return;
    //}


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
}

void ChoiceReactionManager::finish()
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

bool ChoiceReactionManager::clearData()
{
    return false;
}


// Set up device
bool ChoiceReactionManager::setUp()
{
    return true;
}

// Clean up the device for next time
bool ChoiceReactionManager::cleanUp()
{
    return true;
}

void ChoiceReactionManager::setInputData(const QVariantMap& inputData)
{
    Q_UNUSED(inputData)
}

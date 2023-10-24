

#include "frax_manager.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>
#include <QStandardItemModel>

FraxManager::FraxManager(QSharedPointer<FraxSession> session)
    : ManagerBase(session)
    , m_test(new FraxTest)
{
}

bool FraxManager::isAvailable()
{
    return false;
}

bool FraxManager::isInstalled()
{
    return false;
}

void FraxManager::start()
{
    emit started(m_test);
    emit canMeasure();
}


bool FraxManager::isDefined(const QString &exeName) const
{
    Q_UNUSED(exeName)
    return true;
}

void FraxManager::selectRunnable(const QString &runnableName)
{
    Q_UNUSED(runnableName)
}

void FraxManager::measure()
{
    m_test->reset();

    QVariantMap map;
    QJsonObject inputData = m_session->getInputData();

    map.insert("type", 						inputData.value("type"));
    map.insert("country_code", 				inputData.value("country_code"));
    map.insert("age", 						inputData.value("age"));
    map.insert("sex", 						inputData.value("sex"));
    map.insert("bmi", 						inputData.value("bmi"));
    map.insert("previous_fracture", 		inputData.value("previous_fracture"));
    map.insert("parent_hip_fracture", 		inputData.value("parent_hip_fracture"));
    map.insert("current_smoker", 			inputData.value("current_smoker"));
    map.insert("gluccocorticoid", 			inputData.value("gluccocorticoid"));
    map.insert("rheumatoid_arthritis", 		inputData.value("rheumatoid_arthritis"));
    map.insert("secondary_osteoporosis", 	inputData.value("secondary_osteoporosis"));
    map.insert("t_score", 					inputData.value("t_score"));
    map.insert("z_score", 					inputData.value("z_score"));
    map.insert("alcohol", 					inputData.value("alcohol"));
    map.insert("birth_date", 				inputData.value("birth_date"));
    map.insert("interview_date", 			inputData.value("interview_date"));

    m_test->simulate(map);

    emit measured(m_test);
    emit canFinish();
}

void FraxManager::readOutput()
{
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
    QJsonObject responseJson{};

    int answer_id = m_session->getAnswerId();

    QJsonObject testJson = m_test->toJsonObject();
    testJson.insert("session", m_session->getJsonObject());

    responseJson.insert("value", testJson);

    QJsonDocument jsonDoc(responseJson);
    QByteArray serializedData = jsonDoc.toJson();

    QString answerUrl = CypressSettings::getInstance().getAnswerUrl(answer_id);
    sendHTTPSRequest("PATCH", answerUrl, "application/json", serializedData);

    emit success("sent");
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

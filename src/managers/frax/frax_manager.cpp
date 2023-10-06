#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QJsonObject>
#include <QSettings>
#include <QStandardItemModel>

#include "cypress_application.h"
#include "auxiliary/json_settings.h"

#include "frax_manager.h"

FraxManager::FraxManager(const CypressSession& session)
    : ManagerBase(session), m_test(new FraxTest)
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
    return true;
}

void FraxManager::selectRunnable(const QString &runnableName)
{

}

void FraxManager::measure()
{
    m_test->reset();

    QVariantMap map;
    QJsonObject inputData = m_session.getInputData();

    map.insert("TYPE", inputData.value("type"));
    map.insert("COUNTRY_CODE", inputData.value("country_code"));
    map.insert("AGE", inputData.value("age"));
    map.insert("SEX", inputData.value("sex"));
    map.insert("BMI", inputData.value("bmi"));
    map.insert("PREVIOUS_FRACTURE", inputData.value("previous_fracture"));
    map.insert("PARENT_HIP_FRACTURE", inputData.value("parent_hip_fracture"));
    map.insert("CURRENT_SMOKER", inputData.value("current_smoker"));
    map.insert("GLUCCOCORTICOID", inputData.value("gluccocorticoid"));
    map.insert("RHEUMATOID_ARTHRITIS", inputData.value("rheumatoid_arthritis"));
    map.insert("SECONDARY_OSTEOPOROSIS", inputData.value("secondary_osteoporosis"));
    map.insert("T_SCORE", inputData.value("t_score"));
    map.insert("Z_SCORE", inputData.value("z_score"));
    map.insert("ALCOHOL", inputData.value("alcohol"));
    map.insert("BIRTH_DATE", inputData.value("birth_date"));
    map.insert("INTERVIEW_DATE", inputData.value("interview_date"));

    m_test->simulate(map);

    emit measured(m_test);
    if (m_test->isValid())
    {
        emit canFinish();
    }
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
    int answer_id = m_session.getAnswerId();

    QJsonObject testJson = m_test->toJsonObject();
    QJsonDocument jsonDoc(testJson);
    QByteArray serializedData = jsonDoc.toJson();

    sendHTTPSRequest("PATCH", "https://blueberry.clsa-elcv.ca/qa/pine/api/answer/" + answer_id, "application/json", serializedData);

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

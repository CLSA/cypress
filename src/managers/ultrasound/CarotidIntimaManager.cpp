#include "CarotidIntimaManager.h"

#include "CypressApplication.h"
#include "auxiliary/JsonSettings.h"

CarotidIntimaManager::CarotidIntimaManager()
{


}

void CarotidIntimaManager::start()
{
    if (CypressApplication::mode == Mode::Sim) return;
}

void CarotidIntimaManager::measure()
{
    if (CypressApplication::mode == Mode::Sim) return;
}

void CarotidIntimaManager::finish()
{
    if (CypressApplication::mode == Mode::Sim)
    {
        QJsonObject results = JsonSettings::readJsonFromFile(
            "C:/work/clsa/cypress/src/tests/fixtures/ultrasound/output.json"
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

bool CarotidIntimaManager::clearData()
{
    return false;
}

// set input parameters for the test
void CarotidIntimaManager::setInputData(const QVariantMap& inputData)
{

}

bool CarotidIntimaManager::setUp()
{
    return true;
}

bool CarotidIntimaManager::cleanUp()
{
    return true;
}

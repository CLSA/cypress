#include "CarotidIntimaManager.h"

#include "CypressApplication.h"
#include "auxiliary/JsonSettings.h"

CarotidIntimaManager::CarotidIntimaManager(QWidget *parent)
    : ManagerBase{parent}
{


}

void CarotidIntimaManager::start()
{

}

void CarotidIntimaManager::measure()
{

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
    }
}

void CarotidIntimaManager::clearData()
{

}

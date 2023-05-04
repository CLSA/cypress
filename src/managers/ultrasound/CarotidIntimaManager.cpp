#include "CarotidIntimaManager.h"
#include "auxiliary/JsonSettings.h"

CarotidIntimaManager::CarotidIntimaManager(QObject *parent)
{
    Q_UNUSED(parent)

}

void CarotidIntimaManager::start()
{

}

void CarotidIntimaManager::measure()
{
    QJsonObject results = JsonSettings::readJsonFromFile(
        "C:/work/clsa/cypress/src/tests/fixtures/ultrasound/output.json"
    );

    results["cypress_session"] = m_uuid;
    results["answer_id"] = m_answerId;
    results["barcode"] = m_barcode;
    results["interviewer"] = m_interviewer;

    if (results.empty()) return;

    bool ok = sendResultsToPine(results);
    if (!ok)
    {
        qDebug() << "Could not send results to Pine";
    }
}

void CarotidIntimaManager::finish()
{

}

void CarotidIntimaManager::setInputData(const QVariantMap &)
{

}

bool CarotidIntimaManager::setUp()
{
    return true;
}

bool CarotidIntimaManager::clearData()
{
    return true;
}

bool CarotidIntimaManager::cleanUp()
{
    return true;
}

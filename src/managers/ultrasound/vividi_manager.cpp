#include "vividi_manager.h"
#include "auxiliary/json_settings.h"
#include "cypress_application.h"

VividiManager::VividiManager(QObject *parent)
{
    Q_UNUSED(parent)

}

void VividiManager::start()
{

}

void VividiManager::measure()
{
    if (CypressApplication::getInstance().isSimulation()) {
      sendResultsToPine("C:/dev/clsa/cypress/src/tests/fixtures/ultrasound/output.json");
    }

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

void VividiManager::finish()
{

}

void VividiManager::setInputData(const QVariantMap &)
{

}

bool VividiManager::setUp()
{
    return true;
}

bool VividiManager::clearData()
{
    return true;
}

bool VividiManager::cleanUp()
{
    return true;
}

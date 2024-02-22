#include "dxa_session.h"
#include "managers/dxa/dxa_manager.h"
#include "dialogs/dxa_dialog.h"

DXASession::DXASession(QObject *parent, const QJsonObject& inputData)
    : CypressSession{parent, inputData}
{
}

void DXASession::initializeDialog() {
    m_dialog = new DXADialog(nullptr, QSharedPointer<DXASession>(this));
}

QString DXASession::getWebpageContents() const {
    const QString webpageContents
        = "<!DOCTYPE html>"
          "<html lang=\"en\">"
          "<head>"
          "<meta charset=\"UTF-8\">"
          "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
          "<title>DXA 2 (" + m_inputData.value("barcode").toString() + ")</title>"
          "</head>"
          "<body>"
          "<h5>Participant ID</h5>"
          "<p>" + m_inputData.value("barcode").toString() + "</p>"
          "<h5>Scans required</h5>"
          "<p>" + m_inputData.value("scans").toString() + "</p>"
          "<h5>Weight</h5>"
          "<p>" + m_inputData.value("weight").toString() + "</p>"
          "<h5>Height</h5>"
          "<p>" + m_inputData.value("height").toString() + "</p>"
          "<h5>Sex</h5>"
          "<p>" + m_inputData.value("sex").toString() + "</p>"
          "</body>"
          "</html>";

    return webpageContents;
}

void DXASession::isInstalled() const {
    if (!DXAManager::isInstalled())
        throw NotInstalledError("DXA is not installed on this workstation");
}

void DXASession::isAvailable() const {
}

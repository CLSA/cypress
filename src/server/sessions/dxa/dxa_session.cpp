#include "dxa_session.h"
#include "managers/dxa/dxa_manager.h"
#include "dialogs/dxa_dialog.h"

DXASession::DXASession(QObject *parent, const QJsonObject& inputData)
    : CypressSession{parent, inputData}
{
}

void DXASession::initializeDialog()
{
    m_dialog = new DXADialog(nullptr, QSharedPointer<DXASession>(this));
}

QString DXASession::getWebpageContents()
{
    QString webpageContents
        = "<!DOCTYPE html>"
          "<html lang=\"en\">"
          "<head>"
          "<meta charset=\"UTF-8\">"
          "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
          "<title>Pine</title>"
          "</head>"
          "<body>"
          "<h1>ID</h1>"
          "<p></p>"
          "<h1>Scans required</h1>"
          "<p></p>"
          "<h1>Weight</h1>"
          "<p></p>"
          "<h1>Height</h1>"
          "<p></p>"
          "<h1>Sex</h1>"
          "<p></p>"
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

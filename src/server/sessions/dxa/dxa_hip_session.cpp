#include "dxa_hip_session.h"
#include "dialogs/dxa_hip_dialog.h"
#include "managers/dxa/dxa_hip_manager.h"

DxaHipSession::DxaHipSession(QObject *parent, const QJsonObject& inputData)
    : CypressSession{parent, inputData}
{
}

void DxaHipSession::initializeDialog()
{
    m_dialog = new DxaHipDialog(nullptr, QSharedPointer<DxaHipSession>(this));
}

QString DxaHipSession::getWebpageContents()
{
    QString id = getBarcode();
    QString scansRequired = "";
    QString weight = "";
    QString height = "";
    QString sex = "";

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
          + id + "<h1>Scan(s)</h1>" + scansRequired + "<h1>Weight</h1>" + weight + "<h1>Height</h1>"
          + height + "<h1>Sex</h1>" + sex
          + "</body>"
            "</html>";

    return webpageContents;
}

void DxaHipSession::isInstalled() const {
    if (!DxaHipManager::isInstalled())
        throw NotInstalledError("DXA is not installed on this workstation");
}

void DxaHipSession::isAvailable() const {
}

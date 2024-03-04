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

void DxaHipSession::validate() const {
    if (m_debug)
        qDebug() << m_inputData;

    if (!isValidString("sex"))
        throw ValidationError("Sex input variable is invalid");

    //if (!isValidString("scans"))
    //    throw ValidationError("Scans input variable is invalid");

    if (!isValidDouble("weight"))
        throw ValidationError("Weight input variable is invalid");

    if (!isValidDouble("height"))
        throw ValidationError("Height input variable is invalid");
}

QString DxaHipSession::getWebpageContents() const
{
    QString webpageContents
        = "<!DOCTYPE html>"
          "<html lang=\"en\">"
          "<head>"
          "<meta charset=\"UTF-8\">"
          "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
          "<title>DXA 1 (" + m_inputData.value("barcode").toString() + ")</title>"
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

void DxaHipSession::isInstalled() const {
    if (!DxaHipManager::isInstalled())
        throw NotInstalledError("DXA is not installed on this workstation");
}

void DxaHipSession::isAvailable() const {
}

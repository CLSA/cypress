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
    qDebug() << "DxaHipSession: inputData" << m_inputData;
    if (!isValidString("dob"))
        throw ValidationError("dob input is invalid");

    if (!isValidString("sex"))
        throw ValidationError("sex input is invalid");

    if (!isValidDouble("weight"))
        throw ValidationError("weight input is invalid");

    if (!isValidDouble("height"))
        throw ValidationError("height input is invalid");

    if (!isValidBool("include_lhip") && !m_inputData.value("include_lhip").isNull())
        throw ValidationError("include_lhip input is invalid");

    if (!isValidBool("include_rhip") && !m_inputData.value("include_rhip").isNull())
        throw ValidationError("include_rhip input is invalid");
}

QString DxaHipSession::getWebpageContents() const
{
    QString scansRequired = "";
    if (m_inputData.value("include_lhip").toBool())
        scansRequired += "<p>Left Hip</p>";
    if (m_inputData.value("include_rhip").toBool())
        scansRequired += "<p>Right Hip</p>";

    qDebug() << "scans required:" << scansRequired;

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
          "<h5>Date of birth</h5>"
          "<p>" + m_inputData.value("dob").toString() + "</p>"
          "<h5>Sex</h5>"
          "<p>" + m_inputData.value("sex").toString() + "</p>"
          "<h5>Weight</h5>"
          "<p>" + QString::number(m_inputData.value("weight").toDouble()) + "</p>"
          "<h5>Height</h5>"
          "<p>" + QString::number(m_inputData.value("height").toDouble()) + "</p>"
          "<h5>Scans required</h5>"  +
          scansRequired +
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

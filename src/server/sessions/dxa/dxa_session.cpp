#include "dxa_session.h"
#include "managers/dxa/dxa_manager.h"
#include "dialogs/dxa_dialog.h"

DXASession::DXASession(QObject *parent, const QJsonObject& inputData, const QString& origin)
    : CypressSession{parent, inputData, origin}
{
}

void DXASession::initializeDialog() {
    m_dialog = new DXADialog(nullptr, QSharedPointer<DXASession>(this));
}

void DXASession::validate() const {
    qDebug() << "DxaSession: inputData" << m_inputData;
    if (!isValidString("dob"))
        throw ValidationError("dob input is invalid");

    if (!isValidString("sex"))
        throw ValidationError("sex input is invalid");

    if (!isValidDouble("weight"))
        throw ValidationError("weight input is invalid");

    if (!isValidDouble("height"))
        throw ValidationError("Height input is invalid");

    if (!isValidBool("include_apspine") && !m_inputData.value("include_apspine").isNull())
        throw ValidationError("include_apspine input is invalid");

    if (!isValidBool("include_spine") && !m_inputData.value("include_spine").isNull())
        throw ValidationError("include_spine input is invalid");

    if (!isValidBool("include_wholebody") && !m_inputData.value("include_wholebody").isNull())
        throw ValidationError("include_wholebody input is invalid");

    if (!isValidBool("include_lfa") && !m_inputData.value("include_lfa").isNull())
        throw ValidationError("include_lfa input is invalid");

    if (!isValidBool("include_rfa") && !m_inputData.value("include_rfa").isNull())
        throw ValidationError("include_rfa input is invalid");
}

QString DXASession::getWebpageContents() const {

    QString scansRequired = "";
    if (m_inputData.value("include_apspine").toBool())
        scansRequired += "<p>AP Lumbar Spine</p>";
    if (m_inputData.value("include_spine").toBool())
        scansRequired += "<p>Lateral Spine</p>";
    if (m_inputData.value("include_wholebody").toBool())
        scansRequired += "<p>Whole Body</p>";
    if (m_inputData.value("include_lfa").toBool())
        scansRequired += "<p>Left Forearm</p>";
    if (m_inputData.value("include_rfa").toBool())
        scansRequired += "<p>Right Forearm";

    qDebug() << "scans required:" << scansRequired;

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
          "<h5>Date of birth</h5>"
          "<p>" + m_inputData.value("dob").toString() + "</p>"
          "<h5>Sex</h5>"
          "<p>" + m_inputData.value("sex").toString() + "</p>"
          "<h5>Weight</h5>"
          "<p>" + QString::number(m_inputData.value("weight").toDouble()) + "</p>"
          "<h5>Height</h5>"
          "<p>" + QString::number(m_inputData.value("height").toDouble()) + "</p>"
          "<h5>Scans required</h5>" +
          scansRequired +
          "</body>"
          "</html>";

    return webpageContents;
}

void DXASession::isInstalled() const {
    if (DXAManager::config.hasErrors())
        throw NotInstalledError("DXA is not installed on this workstation");
}

void DXASession::isAvailable() const {
}

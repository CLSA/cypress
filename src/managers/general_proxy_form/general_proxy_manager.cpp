#include "general_proxy_manager.h"
#include "auxiliary/pdf_form_filler.h"
#include "auxiliary/file_utils.h"
#include "auxiliary/network_utils.h"
#include "cypress_settings.h"

#include <QDir>
#include <QJsonDocument>

GeneralProxyManager::GeneralProxyManager(QSharedPointer<GenProxySession> session): ManagerBase(session)
{
    m_inputFilePath = CypressSettings::readSetting("general_proxy_form_path").toString();
    m_outputFilePath = QDir::currentPath() + "/" + m_session->getBarcode() + ".pdf";
}

bool GeneralProxyManager::start()
{
    measure();
    return true;
}


bool GeneralProxyManager::isInstalled()
{
    QString proxyPath = CypressSettings::readSetting("general_proxy_form_path").toString();
    if (proxyPath.isNull() || proxyPath.isEmpty()) {
        return false;
    }

    return true;
}

void GeneralProxyManager::measure()
{
    // copy general proxy consent
    if (!QFile::copy(m_inputFilePath, m_outputFilePath)) {
        emit error("Something went wrong");
    }

    // input any needed variables with pdftk
    // open adobe with the copied file
    // signing the page will autosave
    // once adobe closes, send the file
    // remove the file
    // close

    PDFFormFiller filler;
    QJsonObject inputData;

    inputData["Participant.enrollmentId"] = m_session->getBarcode();
    inputData["Participant.fullName"] = "John Smith";

    QString currentDir = QDir::currentPath();
    QString outputPath = filler.fillPDF(
        "C:/Users/Anthony/Documents/GitHub/cypress/src/tests/Tests/gen_proxy_v1-1.pdf",
        "C:/Users/Anthony/Documents/GitHub/cypress/src/tests/Tests/gen_proxy_v1_1.fdf",
        inputData,
        "C:/Users/Anthony/Documents/GitHub/cypress/src/tests/Tests/gen_proxy_v1_1-output.pdf"
    );

    //inputData["IPinformation_mandatoryField"] = "Yes";
    //inputData["AgreeGiveNumber_mandatoryField"] = "Yes";
    //inputData["DCScontinue_mandatoryField"] = "Yes";

    //inputData["ProxyFirstName"] = "John";
    //inputData["ProxyLastName"] = "Smith";
    //inputData["ProxyAddress"] = "123 Lane St.";
    //inputData["ProxyAddress2"] = "Apt 2";
    //inputData["ProxyCity"] = "Hamilton";
    //inputData["ProxyProvince"] = "ON";
    //inputData["ProxyPostalCode"] = "123 456";
    //inputData["ProxyTelephone"] = "(111) 111-1111";

    //inputData["DMalready_mandatoryField"] = "Yes";

    //inputData["InformantFirstName"] = "Jane";
    //inputData["InformantLastName"] = "Smith";
    //inputData["InformantAddress"] = "123 Lane St.";
    //inputData["InformantAddress2"] = "1";
    //inputData["InformantCity"] = "Hamilton";
    //inputData["InformantProvince"] = "ON";
    //inputData["InformantPostalCode"] = "123 456";
    //inputData["InformantTelephone"] = "(222) 222-2222";
    //inputData["informantIsProxy"] = "Yes";



}

void GeneralProxyManager::finish()
{
    int answer_id = m_session->getAnswerId();

    QJsonObject responseJson;
    QJsonObject testJson = {};

    testJson.insert("session", m_session->getJsonObject());
    //testJson.insert("results", inputData);
    testJson.insert("manual_entry", false);

    responseJson.insert("value", testJson);

    QJsonDocument jsonDoc(responseJson);
    QByteArray serializedData = jsonDoc.toJson();

    QString answerUrl = CypressSettings::getAnswerUrl(answer_id);
    bool ok = NetworkUtils::sendHTTPSRequest("PATCH", answerUrl.toStdString(), "application/json", serializedData);
    if (!ok) {
        emit error("Something went wrong");
    }

    ////QDesktopServices::openUrl(outputPath);
    //QByteArray responseData = FileUtils::readFile(outputPath);
    //ok = NetworkUtils::sendHTTPSRequest("PATCH", answerUrl.toStdString(), "application/octet-stream", responseData);
    //if (!ok) {
    //}

    //QString host = CypressSettings::getPineHost();
    //QString endpoint = CypressSettings::getPineEndpoint();

    //ok = NetworkUtils::sendHTTPSRequest("PATCH",
    //                      (host + endpoint + QString::number(answer_id)
    //                                     + "?filename=general_proxy_consent.pdf").toStdString(),
    //                      "application/octet-stream",
    //                      responseData);
    //if (!ok) {
    //}

    sendComplete(m_session->getSessionId());

    emit success("");
}

void GeneralProxyManager::setInputData(const QVariantMap &)
{

}

bool GeneralProxyManager::setUp()
{
    return true;
}

bool GeneralProxyManager::clearData()
{
    return true;
}

bool GeneralProxyManager::cleanUp()
{
    return true;
}

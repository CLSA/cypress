#include "general_proxy_manager.h"
#include "auxiliary/pdf_form_filler.h"
#include "auxiliary/file_utils.h"
#include "cypress_settings.h"

#include <QDir>
#include <QJsonDocument>

GeneralProxyManager::GeneralProxyManager(QSharedPointer<GenProxySession> session): ManagerBase(session)
{

}

void GeneralProxyManager::start()
{
    finish();
}

void GeneralProxyManager::measure()
{

}

void GeneralProxyManager::finish()
{
    PDFFormFiller filler;
    QJsonObject inputData;

    inputData["enrollmentId"] = m_session->getBarcode();
    inputData["fullName"] = "John Smith";

    inputData["IPinformation_mandatoryField"] = "Yes";
    inputData["AgreeGiveNumber_mandatoryField"] = "Yes";
    inputData["DCScontinue_mandatoryField"] = "Yes";

    inputData["Date"] = "2023-07-13";

    inputData["ProxyFirstName"] = "John";
    inputData["ProxyLastName"] = "Smith";
    inputData["ProxyAddress"] = "123 Lane St.";
    inputData["ProxyAddress2"] = "Apt 2";
    inputData["ProxyCity"] = "Hamilton";
    inputData["ProxyProvince"] = "ON";
    inputData["ProxyPostalCode"] = "123 456";
    inputData["ProxyTelephone"] = "(111) 111-1111";

    inputData["DMalready_mandatoryField"] = "Yes";

    inputData["InformantFirstName"] = "Jane";
    inputData["InformantLastName"] = "Smith";
    inputData["InformantAddress"] = "123 Lane St.";
    inputData["InformantAddress2"] = "1";
    inputData["InformantCity"] = "Hamilton";
    inputData["InformantProvince"] = "ON";
    inputData["InformantPostalCode"] = "123 456";
    inputData["InformantTelephone"] = "(222) 222-2222";
    inputData["informantIsProxy"] = "Yes";

    QString currentDir = QDir::currentPath();
    QString outputPath = filler.fillPDF(
        "C:/Users/Anthony/Documents/GitHub/cypress/src/tests/Tests/gen_proxy_v1-1.pdf",
        "C:/Users/Anthony/Documents/GitHub/cypress/src/tests/Tests/gen_proxy_v1_1.fdf",
        inputData,
        "C:/Users/Anthony/Documents/GitHub/cypress/src/tests/Tests/gen_proxy_v1_1-output.pdf"
    );

    int answer_id = m_session->getAnswerId();

    QJsonObject responseJson;
    QJsonObject testJson = {};

    testJson.insert("session", m_session->getJsonObject());
    testJson.insert("results", inputData);
    testJson.insert("manual_entry", false);

    responseJson.insert("value", testJson);

    QJsonDocument jsonDoc(responseJson);
    QByteArray serializedData = jsonDoc.toJson();

    QString answerUrl = CypressSettings::getAnswerUrl(answer_id);
    sendHTTPSRequest("PATCH", answerUrl, "application/json", serializedData);

    ////QDesktopServices::openUrl(outputPath);
    QByteArray responseData = FileUtils::readFile(outputPath);
    sendHTTPSRequest("PATCH", answerUrl, "application/octet-stream", responseData);

    QString host = CypressSettings::getPineHost();
    QString endpoint = CypressSettings::getPineEndpoint();

    sendHTTPSRequest("PATCH",
                     host + endpoint + QString::number(answer_id) + "?filename=general_proxy_consent.pdf",
                     "application/octet-stream",
                     responseData);

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

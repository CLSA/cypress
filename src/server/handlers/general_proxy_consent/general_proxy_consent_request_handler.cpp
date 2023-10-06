#include "general_proxy_consent_request_handler.h"
#include "auxiliary/pdf_form_filler.h"
#include "auxiliary/file_utils.h"

#include <QDesktopServices>
#include <QDir>

void GeneralProxyConsentRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    try {
        PDFFormFiller filler;
        QJsonObject inputData;
        QJsonObject requestData = getRequestData(request);

        inputData["enrollmentId"] = requestData["barcode"].toString();
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

        inputData["InformantFirstName"] = "Sally";
        inputData["InformantLastName"] = "Smith";
        inputData["InformantAddress"] = "123 Lane St.";
        inputData["InformantAddress2"] = "Apt 2";
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
            "C:/Users/Anthony/Documents/GitHub/cypress/src/tests/Tests/gen_proxy_v1_1-output.pdf");

        QDesktopServices::openUrl(outputPath);
        QByteArray responseData = FileUtils::readFileIntoByteArray(outputPath);

        FileUtils::sendHTTPSRequest(
            "PATCH",
            "https://blueberry.clsa-elcv.ca/qa/pine/api/answer/" + requestData["answer_id"].toString() + "?filename=general_proxy_consent_" + requestData["barcode"].toString() + ".pdf",
            "application/octet-stream",
            responseData);

        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentType("application/json");

        std::ostream& out = response.send();
        //out << responseData.toStdString();
        out.flush();
    }
    catch (const InvalidBarcodeException& exception)
    {
        response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
        response.setContentType("application/json");

        std::ostream& out = response.send();
        out << "invalid barcode";
        out.flush();
    }
    catch (const InvalidAnswerIdException& exception)
    {
        response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
        response.setContentType("application/json");

        std::ostream& out = response.send();
        out << "invalid answer id";
        out.flush();
    }
    catch (const InvalidInterviewerException& exception)
    {
        response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
        response.setContentType("application/json");

        std::ostream& out = response.send();
        out << "invalid interviewer option";
        out.flush();
    }
    catch (const InvalidLanguageException& exception)
    {
        response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
        response.setContentType("application/json");

        std::ostream& out = response.send();
        out << "invalid language option";
        out.flush();
    }
    catch (const InvalidInputDataException& exception)
    {
        response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
        response.setContentType("application/json");

        std::ostream& out = response.send();
        out << "invalid input data";
        out.flush();
    }

}

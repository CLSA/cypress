#include "participant_report_request_handler.h"
#include "cypress_application.h"
#include "auxiliary/json_settings.h"

#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

void ParticipantReportRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    try {
        QJsonObject requestData = getRequestData(request);
        qDebug() << "ParticipantReportRequestHandler::requestData()" << requestData;
        QString sessionId = Cypress::getInstance().httpServer->requestReport(Constants::ReportType::ParticipantReportEn, requestData);

        if (false)
        {
            response.setStatus(Poco::Net::HTTPResponse::HTTP_CONFLICT);
            response.setContentType("application/json");

            std::ostream& out = response.send();
            out << "workstation is busy";
            out.flush();

            return;
        }

        QJsonObject data = getResponseData(sessionId);
        QString responseData = JsonSettings::serializeJson(data);

        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentType("application/json");

        std::ostream& out = response.send();
        out << responseData.toStdString();
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
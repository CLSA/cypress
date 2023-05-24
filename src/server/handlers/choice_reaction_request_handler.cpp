#include "choice_reaction_request_handler.h".h"
#include "CypressApplication.h"

#include "auxiliary/JsonSettings.h"

void ChoiceReactionRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    try {
        QJsonObject requestData = getRequestData(request);
        QString responseData = JsonSettings::serializeJson(getResponseData());

        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentType("application/json");

        CypressApplication::getInstance().server -> requestTestStart(Constants::MeasureType::Choice_Reaction, requestData);

        std::ostream& out = response.send();
        out << responseData.toStdString();
        out.flush();
    }
    catch (std::exception& e)
    {
        qDebug() << e.what();
    }
}

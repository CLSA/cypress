#include <QDebug>

#include "cypress_application.h"
#include "auxiliary/json_settings.h"

#include "retinal_camera_right_request_handler.h"

void RetinalCameraRightRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    try {
        QJsonObject requestData = getRequestData(request);
        QString sessionId = Cypress::getInstance().httpServer->requestDevice(Constants::MeasureType::Retinal_Camera_Right, requestData);

        QJsonObject data = getResponseData(sessionId);
        QString responseData = JsonSettings::serializeJson(data);

        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentType("application/json");

        std::ostream& out = response.send();
        out << responseData.toStdString();
        out.flush();
    }
    catch (const ValidationError& exception)
    {
        response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
        response.setContentType("application/json");

        std::ostream& out = response.send();
        out << exception.what();
        out.flush();
    }
    catch (QException e)
    {
        qDebug() << e.what() ;
    }

    catch (std::exception e)
    {
        qDebug() << e.what();
    }
}

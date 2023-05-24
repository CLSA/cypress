#include <QDebug>

#include "CypressApplication.h"
#include "auxiliary/JsonSettings.h"
#include "retinal_camera_request_handler.h"

void RetinalCameraRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{

    try {
        QJsonObject requestData = getRequestData(request);

        // prepare response
        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentType("application/json");

        const Constants::MeasureType measureType = Constants::MeasureType::Retinal_Camera;
        const QJsonObject responseJSON = getResponseData();
        const QString sessionId = responseJSON.value("id").toString();
        const QString responseData = JsonSettings::serializeJson(responseJSON);

        // start test
        CypressApplication::getInstance().server -> requestTestStart(measureType, requestData);

        // send response with uuid body
        std::ostream& out = response.send();

        //out << "\"" << uuid.toStdString() << "\"";
        out.flush();
    }
    catch (std::exception& e)
    {
        qDebug() << e.what();
    }
}

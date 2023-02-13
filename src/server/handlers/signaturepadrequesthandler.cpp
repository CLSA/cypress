#include "signaturepadrequesthandler.h"

void SignaturePadRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    // buffer for reading request body
    char* buffer = new char[4096];

    try {
        std::string requestBody;
        std::istream &i = request.stream();

        // read request body
        while (i.read(buffer, sizeof(buffer)))
        {
            requestBody.append(buffer, sizeof(buffer));
        }
        requestBody.append(buffer, i.gcount());
        qInfo() << QString::fromStdString(requestBody);

        // prepare response
        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentType("application/json");

        const Constants::MeasureType measureType = Constants::MeasureType::typeSignature;
        const QJsonObject responseJSON = getResponseData();
        const QString uuid = responseJSON.value("id").toString();
        const QString responseData = JsonSettings::serializeJson(responseJSON);

        // start test
        CypressApplication::restApiServer -> requestTestStart(measureType, uuid);

        // send response with uuid body
        std::ostream& out = response.send();
        out << "\"" << uuid.toStdString() << "\"";
        out.flush();
    }
    catch (std::exception& e)
    {
        qDebug() << e.what();
    }

    delete[] buffer;
}




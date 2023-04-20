#include "signaturepadrequesthandler.h"

#include "Poco/StreamCopier.h"
#include <sstream>

void SignaturePadRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    // buffer for reading request body
    char* buffer = new char[4096];

    try {
        std::istream &i = request.stream();
        std::stringstream requestBodyStringStream;
        Poco::StreamCopier::copyStream(i, requestBodyStringStream);
        std::string requestBody = requestBodyStringStream.str();

        qDebug() << request.getContentLength();
        qDebug() << request.getContentType().c_str();

        // read request body
        //while (i.read(buffer, sizeof(buffer)))
        //{
        //    requestBody.append(buffer, sizeof(buffer));
        //}
        QByteArray inputDataRaw = QByteArray::fromStdString(requestBody);
        QJsonDocument jsonDocument = QJsonDocument::fromJson(inputDataRaw);
        QJsonObject inputData = jsonDocument.object();

        qDebug() << inputData;
        // prepare response
        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentType("application/json");

        const Constants::MeasureType measureType = Constants::MeasureType::Signature;
        const QJsonObject responseJSON = getResponseData();
        const QString sessionId = responseJSON.value("sessionId").toString();
        const QString responseData = JsonSettings::serializeJson(responseJSON);

        // start test
        CypressApplication::getInstance().server -> requestTestStart(measureType, inputData);

        // send response with uuid body
        std::ostream& out = response.send();
        out << "\"" << sessionId.toStdString() << "\"";
        out.flush();
    }
    catch (std::exception& e)
    {
        qDebug() << e.what();
    }

    delete[] buffer;
}




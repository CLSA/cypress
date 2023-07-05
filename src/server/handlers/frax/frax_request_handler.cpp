#include <QDebug>

#include "cypress_application.h"
#include "frax_request_handler.h"

#include "auxiliary/json_settings.h"

void FraxRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    try {
        QJsonObject requestData = getRequestData(request);
        QString sessionId = Cypress::getInstance().httpServer->requestSession(Constants::MeasureType::Frax, requestData);

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

bool isValidString(const QJsonObject& inputData, QString& variableName)
{
    return !(!inputData.contains(variableName) || inputData.value(variableName).isNull() || !inputData.value(variableName).isString() || inputData.value(variableName) == "");
}

bool isValidBool(const QJsonObject& inputData, QString& variableName)
{
    return !(!inputData.contains(variableName) || inputData.value(variableName).isNull() || !inputData.value(variableName).isBool());
}

bool isValidInt(const QJsonObject& inputData, QString& variableName)
{
    return !(!inputData.contains(variableName) || inputData.value(variableName).isNull());
}

bool isValidDouble(const QJsonObject& inputData, QString& variableName)
{
    return !(!inputData.contains(variableName) || inputData.value(variableName).isNull());
}

//bool FraxRequestHandler::isValidInputData(const QJsonObject& inputData)
//{
//    bool is_default_valid = DefaultRequestHandler::isValidInputData(inputData);
//
//    qDebug() << inputData;
//
//    if (!is_default_valid) {
//        return false;
//    }
//
//    if (!inputData.contains("age") || inputData.value("age").isNull() || !inputData.value("age").isString() || inputData.value("age") == "") {
//        qDebug() << "1";
//        return false;
//    }
//
//    if (!inputData.contains("alcohol") || inputData.value("alcohol").isNull() || !inputData.value("alcohol").isBool()) {
//        qDebug() << "2";
//        return false;
//    }
//
//    if (!inputData.contains("country_code") || inputData.value("country_code").isNull() || !inputData.value("country_code").isString()) {
//        qDebug() << "3";
//        return false;
//    }
//
//    if (!inputData.contains("current_smoker")) {
//        qDebug() << "4";
//        return false;
//    }
//
//    if (!inputData.contains("femoral_neck_bmd")) {
//        qDebug() << "5";
//        return false;
//    }
//
//    if (!inputData.contains("gluccocorticoid")) {
//        qDebug() << "6";
//        return false;
//    }
//
//    if (!inputData.contains("gluccocorticoid_month_number")) {
//        qDebug() << "6";
//        return false;
//    }
//
//    if (!inputData.contains("gluccocorticoid_year")) {
//        qDebug() << "6";
//        return false;
//    }
//
//    if (!inputData.contains("gluccocorticoid_age")) {
//        qDebug() << "6";
//        return false;
//    }
//    if (!inputData.contains("parent_hip_fracture")) {
//        qDebug() << "7";
//        return false;
//    }
//
//    if (!inputData.contains("previous_fracture")) {
//        qDebug() << "8";
//        return false;
//    }
//
//    if (!inputData.contains("rheumatoid_arthritis")) {
//        qDebug() << "9";
//        return false;
//    }
//
//    if (!inputData.contains("sex")) {
//        qDebug() << "11";
//        return false;
//    }
//
//    if (!inputData.contains("type")) {
//        qDebug() << "12";
//        return false;
//    }
//
//    if (!inputData.contains("mother_hip_fracture"))
//    {
//        qDebug() << "13";
//        return false;
//    }
//
//    if (!inputData.contains("father_hip_fracture"))
//    {
//        qDebug() << "14";
//        return false;
//    }
//
//    if (!inputData.contains("weight")) {
//        qDebug() << "15";
//        return false;
//    }
//
//    //bool ok = false;
//    //double weight = inputData.value("weight").toString().toDouble(&ok);
//    //if (!ok)
//    //{
//    //    qDebug()
//    //    return false;
//    //}
//
//    //qDebug() << "weight: " << weight;
//
//    return true;
//}

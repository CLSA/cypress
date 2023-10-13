#include "participant_report_request_handler.h"
//#include "cypress_application.h"
//#include "auxiliary/json_settings.h"

#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

#include "auxiliary/pdf_form_filler.h"

#include <QDesktopServices>
#include <QDir>

void ParticipantReportRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    try {
        QJsonObject requestData = getRequestData(request);
        //QString sessionId = Cypress::getInstance().httpServer->requestReport(Constants::ReportType::ParticipantReportEn, requestData);

        //QJsonObject data = getResponseData(sessionId);
        //QString responseData = JsonSettings::serializeJson(data);

        PDFFormFiller filler;
        QJsonObject inputData;

        inputData["DATE"] = QDate::currentDate().toString("yyyy-MM-dd");

        inputData["NAME"] = requestData.value("name").toString();
        inputData["AGE"] = requestData.value("age").toDouble();

        const QString& sex = requestData.value("sex").toString();

        inputData["HEIGHT"] = requestData.value("height").toDouble();
        inputData["WEIGHT"] = requestData.value("weight").toDouble();
        inputData["BMI"] = requestData.value("bmi").toDouble();

        inputData["BMI_NORMAL"] = "18.5 - 24.9";
        inputData["BMI_INTERP"] = inputData["BMI"].toDouble() < 18.5 ? "Low" : inputData["BMI"].toDouble() > 24.9 ? "High" : "Low";

        inputData["WAIST_CIRC"] = requestData.value("waist_circ").toDouble();
        inputData["HIP_CIRC"] = requestData.value("hip_circ").toDouble();
        inputData["WAIST_TO_HIP_RATIO"] = requestData.value("waist_to_hip_ratio").toDouble();

        if (sex == "F")
        {
            inputData["WAIST_TO_HIP_RATIO_NORMAL"] = "< 0.8";
        }

        else
        {
            inputData["WAIST_TO_HIP_RATIO_NORMAL"] = "< 1.0";
        }

        if (sex == "F")
        {
            inputData["WAIST_TO_HIP_RATIO_INTERP"] = "";
        }
        else {
            inputData["WAIST_TO_HIP_RATIO_INTERP"] = "";
        }

        inputData["BP_SYSTOLIC"] = requestData.value("bp_systolic").toDouble();
        inputData["BP_SYSTOLIC_NORMAL"] = "< 130";
        inputData["BP_SYSTOLIC_INTERP"] = requestData.value("bp_systolic").toDouble() < 130 ? "Normal" : requestData.value("bp_systolic").toDouble() >= 140 ? "High" : "Borderline";

        inputData["BP_DIASTOLIC"] = requestData.value("bp_diastolic").toDouble();

        inputData["BP_DIASTOLIC_NORMAL"] = "< 85";
        inputData["BP_DIASTOLIC_INTERP"] = requestData.value("bp_systolic").toDouble() < 85 ? "Normal" : requestData.value("bp_systolic").toDouble() >= 90 ? "High" : "Borderline";

        inputData["FVC"] = requestData.value("fvc").toDouble();
        inputData["FVC_NORMAL"] = "";
        inputData["FVC_INTERP"] = "";

        inputData["FEV1"] = requestData.value("fev1").toDouble();
        inputData["FEV1_NORMAL"] = "";
        inputData["FEV1_INTERP"] = "";

        inputData["FEV1_FVC"] = requestData.value("fev1_fvc");
        inputData["FEV1_FVC_NORMAL"] = "> 0.7";
        inputData["FEV1_FVC_INTERP"] = requestData.value("fev1_fvc").toDouble() > 0.7 ? "Normal" : "Low";

        inputData["FRAX"] = requestData.value("frax").toDouble();

        inputData["HEARING_RIGHT_500"] = requestData.value("hearing_right_500").toDouble() > 40 ? "Fail" : "Pass";
        inputData["HEARING_LEFT_500"] = requestData.value("hearing_left_500").toDouble() > 40 ? "Fail" : "Pass";

        inputData["HEARING_RIGHT_1000"] = requestData.value("hearing_right_1000").toDouble() > 40 ? "Fail" : "Pass";
        inputData["HEARING_LEFT_1000"] = requestData.value("hearing_left_1000").toDouble() > 40 ? "Fail" : "Pass";

        inputData["HEARING_RIGHT_2000"] = requestData.value("hearing_right_2000").toDouble() > 40 ? "Fail" : "Pass";
        inputData["HEARING_LEFT_2000"] = requestData.value("hearing_left_2000").toDouble() > 40 ? "Fail" : "Pass";

        inputData["HEARING_RIGHT_3000"] = requestData.value("hearing_right_3000").toDouble() > 40 ? "Fail" : "Pass";
        inputData["HEARING_LEFT_3000"] = requestData.value("hearing_left_3000").toDouble() > 40 ? "Fail" : "Pass";

        inputData["HEARING_RIGHT_4000"] = requestData.value("hearing_right_4000").toDouble() > 40 ? "Fail" : "Pass";
        inputData["HEARING_LEFT_4000"] = requestData.value("hearing_left_4000").toDouble() > 40 ? "Fail" : "Pass";

        inputData["HEARING_RIGHT_6000"] = requestData.value("hearing_right_6000").toDouble() > 40 ? "Fail" : "Pass";
        inputData["HEARING_LEFT_6000"] = requestData.value("hearing_left_6000").toDouble() > 40 ? "Fail" : "Pass";

        inputData["HEARING_RIGHT_8000"] = requestData.value("hearing_right_8000").toDouble() > 40 ? "Fail" : "Pass";
        inputData["HEARING_LEFT_8000"] = requestData.value("hearing_left_8000").toDouble() > 40 ? "Fail" : "Pass";

        inputData["VISUAL_ACUITY"] = requestData.value("visual_acuity").toString();

        QString currentDir = QDir::currentPath();
        QString outputPath = filler.fillPDF(
            "C:/Users/Anthony/Documents/GitHub/cypress/src/tests/Tests/participant_report_follow_up_3_site_en.pdf",
            "C:/Users/Anthony/Documents/GitHub/cypress/src/tests/Tests/participant_report_follow_up_3_site_en.fdf",
            inputData,
            "C:/Users/Anthony/Documents/GitHub/cypress/src/tests/Tests/participant_report_follow_up_3_site_en_output.pdf");

        QDesktopServices::openUrl(outputPath);

        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentType("application/json");

        std::ostream& out = response.send();
        //out << responseData.toStdString();
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
}

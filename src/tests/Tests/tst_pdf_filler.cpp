#include <QtTest>
#include <QCoreApplication>
#include <QDebug>
#include <QJsonObject>
#include <QDesktopServices>

#include "../../Auxiliary/pdf_form_filler.h"

class pdf_filler : public QObject
{
    Q_OBJECT

public:
    pdf_filler();
    ~pdf_filler();

private slots:
    void initTestCase();
    void cleanupTestCase();

    void test_gen_proxy_pdf();

    void test_participant_report_en_pdf();
    void test_participant_report_fr_pdf();

};

pdf_filler::pdf_filler()
{

}

pdf_filler::~pdf_filler()
{

}

void pdf_filler::initTestCase()
{

}

void pdf_filler::cleanupTestCase()
{

}

void pdf_filler::test_gen_proxy_pdf()
{
    PDFFormFiller filler;
    QJsonObject inputData;

    inputData["enrollmentId"] = "1234";
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
}

void pdf_filler::test_participant_report_en_pdf()
{
    PDFFormFiller filler;
    QJsonObject inputData;

    inputData["DATE"] = "";
    inputData["NAME"] = "";
    inputData["AGE"] = "";
    inputData["HEIGHT"] = "";
    inputData["WEIGHT"] = "";
    inputData["BMI"] = "";
    inputData["BMI_NORMAL"] = "";
    inputData["BMI_INTERP"] = "";

    inputData["WAIST_TO_HIP_RATIO"] = "";
    inputData["WAIST_CIRC"] = "";
    inputData["HIP_CIRC"] = "";
    inputData["WAIST_TO_HIP_RATIO_NORMAL"] = "";
    inputData["WAIST_TO_HIP_RATIO_INTERP"] = "";

    inputData["BP_SYSTOLIC"] = "";
    inputData["BP_SYSTOLIC_NORMAL"] = "";
    inputData["BP_SYSTOLIC_INTERP"] = "";
    inputData["BP_DIASTOLIC"] = "";
    inputData["BP_DIASTOLIC_NORMAL"] = "";
    inputData["BP_DIASTOLIC_INTERP"] = "";

    inputData["FVC"] = "";
    inputData["FVC_NORMAL"] = "";
    inputData["FVC_INTERP"] = "";
    inputData["FEV1"] = "";
    inputData["FEV1_NORMAL"] = "";
    inputData["FEV1_INTERP"] = "";
    inputData["FEV1_FVC"] = "";
    inputData["FEV1_FVC_NORMAL"] = "";
    inputData["FEV1_FVC_INTERP"] = "";

    inputData["FRAX"] = "";

    inputData["HEARING_RIGHT_500"] = "";
    inputData["HEARING_LEFT_500"] = "";

    inputData["HEARING_RIGHT_1000"] = "";
    inputData["HEARING_LEFT_1000"] = "";

    inputData["HEARING_RIGHT_2000"] = "";
    inputData["HEARING_LEFT_2000"] = "";

    inputData["HEARING_RIGHT_3000"] = "";
    inputData["HEARING_LEFT_3000"] = "";

    inputData["HEARING_RIGHT_4000"] = "";
    inputData["HEARING_LEFT_4000"] = "";

    inputData["HEARING_RIGHT_6000"] = "";
    inputData["HEARING_LEFT_6000"] = "";

    inputData["HEARING_RIGHT_8000"] = "";
    inputData["HEARING_LEFT_8000"] = "";

    inputData["VISUAL_ACUITY"] = "";

    QString currentDir = QDir::currentPath();
    QString outputPath = filler.fillPDF(
        "C:/Users/Anthony/Documents/GitHub/cypress/src/tests/Tests/participant_report_follow_up_3_site_en.pdf",
        "C:/Users/Anthony/Documents/GitHub/cypress/src/tests/Tests/participant_report_follow_up_3_site_en.fdf",
        inputData,
        "C:/Users/Anthony/Documents/GitHub/cypress/src/tests/Tests/participant_report_follow_up_3_site_en_output.pdf");

    QDesktopServices::openUrl(outputPath);

}

void pdf_filler::test_participant_report_fr_pdf()
{
    PDFFormFiller filler;
    QJsonObject inputData;

    inputData["DATE"] = "";
    inputData["NAME"] = "";
    inputData["AGE"] = "";
    inputData["HEIGHT"] = "";
    inputData["WEIGHT"] = "";
    inputData["BMI"] = "";
    inputData["BMI_NORMAL"] = "";
    inputData["BMI_INTERP"] = "";

    inputData["WAIST_TO_HIP_RATIO"] = "";
    inputData["WAIST_CIRC"] = "";
    inputData["HIP_CIRC"] = "";
    inputData["WAIST_TO_HIP_RATIO_NORMAL"] = "";
    inputData["WAIST_TO_HIP_RATIO_INTERP"] = "";

    inputData["BP_SYSTOLIC"] = "";
    inputData["BP_SYSTOLIC_NORMAL"] = "";
    inputData["BP_SYSTOLIC_INTERP"] = "";
    inputData["BP_DIASTOLIC"] = "";
    inputData["BP_DIASTOLIC_NORMAL"] = "";
    inputData["BP_DIASTOLIC_INTERP"] = "";

    inputData["FVC"] = "";
    inputData["FVC_NORMAL"] = "";
    inputData["FVC_INTERP"] = "";
    inputData["FEV1"] = "";
    inputData["FEV1_NORMAL"] = "";
    inputData["FEV1_INTERP"] = "";
    inputData["FEV1_FVC"] = "";
    inputData["FEV1_FVC_NORMAL"] = "";
    inputData["FEV1_FVC_INTERP"] = "";

    inputData["FRAX"] = "";

    inputData["HEARING_RIGHT_500"] = "";
    inputData["HEARING_LEFT_500"] = "";

    inputData["HEARING_RIGHT_1000"] = "";
    inputData["HEARING_LEFT_1000"] = "";

    inputData["HEARING_RIGHT_2000"] = "";
    inputData["HEARING_LEFT_2000"] = "";

    inputData["HEARING_RIGHT_3000"] = "";
    inputData["HEARING_LEFT_3000"] = "";

    inputData["HEARING_RIGHT_4000"] = "";
    inputData["HEARING_LEFT_4000"] = "";

    inputData["HEARING_RIGHT_6000"] = "";
    inputData["HEARING_LEFT_6000"] = "";

    inputData["HEARING_RIGHT_8000"] = "";
    inputData["HEARING_LEFT_8000"] = "";

    inputData["VISUAL_ACUITY"] = "";

    QString currentDir = QDir::currentPath();
    QString outputPath = filler.fillPDF(
        "C:/Users/Anthony/Documents/GitHub/cypress/src/tests/Tests/participant_report_follow_up_3_site_fr.pdf",
        "C:/Users/Anthony/Documents/GitHub/cypress/src/tests/Tests/participant_report_follow_up_3_site_fr.fdf",
        inputData,
        "C:/Users/Anthony/Documents/GitHub/cypress/src/tests/Tests/participant_report_follow_up_3_site_fr_output.pdf");

    QDesktopServices::openUrl(outputPath);
}

QTEST_MAIN(pdf_filler)

#include "tst_pdf_filler.moc"

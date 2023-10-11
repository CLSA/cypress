
#include "../../data/ecg/tests/ecg_test.h"
#include "../../data/blood_pressure/tests/blood_pressure_test.h"
#include "../../data/spirometer/tests/spirometer_test.h"

#include "../../data/tonometer/tests/tonometer_test.h"
#include "../../data/weigh_scale/tests/weigh_scale_test.h"

#include "../../data/cimt_vivid_i_test.h"
#include "../../data/grip_strength/tests/grip_strength_test.h"
#include "../../data/retinal_camera/retinal_camera_test.h"

#include "../../data/cdtt/tests/cdtt_test.h"
#include "../../data/choice_reaction/tests/choice_reaction_test.h"

#include "../../data/hearing/tests/hearing_test.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDesktopServices>
#include <QJsonObject>
#include <QtTest>

class DeviceSimTests : public QObject
{
    Q_OBJECT

public:
    DeviceSimTests();
    ~DeviceSimTests();

private slots:
    void initTestCase();
    void cleanupTestCase();

    void test_dexa_sim();
    void test_audiometer_sim();
    void test_grip_strength_sim();
    void test_choice_reaction_sim();
    void test_gen_proxy_sim();
    void test_participant_report_en_sim();
    void test_participant_report_fr_sim();

    void test_ecg_sim();
    void test_bpm_sim();
    void test_weight_scale_sim();
    void test_spirometer_sim();
    void test_tonometer_sim();
    void test_ultrasound_sim();
    void test_retinal_camera_sim();
    void test_cdtt_sim();
};

void DeviceSimTests::test_ecg_sim()
{
    ECGTest test;

    test.simulate();

    qDebug() << test.toJsonObject();
}

void DeviceSimTests::test_bpm_sim()
{
    BloodPressureTest test;
    test.simulate();
    qDebug() << test.toJsonObject();
}

void DeviceSimTests::test_weight_scale_sim()
{
    WeighScaleTest test;
    test.simulate();
    qDebug() << test.toJsonObject();
}

void DeviceSimTests::test_spirometer_sim()
{
    SpirometerTest test;
    test.simulate(QVariantMap({
                               {"barcode", "12345678"},
                               {"smoker", true 	},
                               {"gender", "M"},
                               {"height", 1.87},
                               {"weight", 80},
                               {"date_of_birth", "12.06.1960"},
    }));
    qDebug() << test.toJsonObject();
}

void DeviceSimTests::test_dexa_sim()
{

}

void DeviceSimTests::test_ultrasound_sim()
{
}

void DeviceSimTests::test_audiometer_sim()
{
    HearingTest test;
    test.simulate(QVariantMap({{"barcode", "12345678" }}));
    qDebug() << test.toJsonObject();
}

void DeviceSimTests::test_retinal_camera_sim()
{
    RetinalCameraTest test;
    test.simulate();
    qDebug() << test.toJsonObject();
}

void DeviceSimTests::test_tonometer_sim()
{
    TonometerTest test;
    test.simulate(
        QVariantMap({{"sex", "f"}, {"date_of_birth", "1960.12.06"}, {"barcode", "12345678"}}));
    qDebug() << test.toJsonObject();
}

void DeviceSimTests::test_grip_strength_sim()
{
    GripStrengthTest test;
    test.simulate();
    qDebug() << test.toJsonObject();
}

void DeviceSimTests::test_cdtt_sim()
{
    CDTTTest test;
    test.simulate(QVariantMap({{"barcode", "12345678"}}));
    qDebug() << test.toJsonObject();
}

void DeviceSimTests::test_choice_reaction_sim()
{
    ChoiceReactionTest test;
    test.simulate();
    qDebug() << test.toJsonObject();
}

DeviceSimTests::DeviceSimTests()
{

}

DeviceSimTests::~DeviceSimTests()
{

}

void DeviceSimTests::initTestCase()
{

}

void DeviceSimTests::cleanupTestCase()
{

}

void DeviceSimTests::test_gen_proxy_sim()
{
    //PDFFormFiller filler;
    //QJsonObject inputData;

    //inputData["enrollmentId"] = "1234";
    //inputData["fullName"] = "John Smith";

    //inputData["IPinformation_mandatoryField"] = "Yes";
    //inputData["AgreeGiveNumber_mandatoryField"] = "Yes";
    //inputData["DCScontinue_mandatoryField"] = "Yes";

    //inputData["Date"] = "2023-07-13";

    //inputData["ProxyFirstName"] = "John";
    //inputData["ProxyLastName"] = "Smith";
    //inputData["ProxyAddress"] = "123 Lane St.";
    //inputData["ProxyAddress2"] = "Apt 2";
    //inputData["ProxyCity"] = "Hamilton";
    //inputData["ProxyProvince"] = "ON";
    //inputData["ProxyPostalCode"] = "123 456";
    //inputData["ProxyTelephone"] = "(111) 111-1111";

    //inputData["DMalready_mandatoryField"] = "Yes";

    //inputData["InformantFirstName"] = "Sally";
    //inputData["InformantLastName"] = "Smith";
    //inputData["InformantAddress"] = "123 Lane St.";
    //inputData["InformantAddress2"] = "Apt 2";
    //inputData["InformantCity"] = "Hamilton";
    //inputData["InformantProvince"] = "ON";
    //inputData["InformantPostalCode"] = "123 456";
    //inputData["InformantTelephone"] = "(222) 222-2222";
    //inputData["informantIsProxy"] = "Yes";

    //QString currentDir = QDir::currentPath();
    //QString outputPath = filler.fillPDF(
    //    "C:/Users/Anthony/Documents/GitHub/cypress/src/tests/Tests/gen_proxy_v1-1.pdf",
    //    "C:/Users/Anthony/Documents/GitHub/cypress/src/tests/Tests/gen_proxy_v1_1.fdf",
    //    inputData,
    //    "C:/Users/Anthony/Documents/GitHub/cypress/src/tests/Tests/gen_proxy_v1_1-output.pdf");

    //QDesktopServices::openUrl(outputPath);
}

void DeviceSimTests::test_participant_report_en_sim()
{
    //PDFFormFiller filler;
    //QJsonObject inputData;

    //inputData["DATE"] = "";
    //inputData["NAME"] = "";
    //inputData["AGE"] = "";
    //inputData["HEIGHT"] = "";
    //inputData["WEIGHT"] = "";
    //inputData["BMI"] = "";
    //inputData["BMI_NORMAL"] = "";
    //inputData["BMI_INTERP"] = "";

    //inputData["WAIST_TO_HIP_RATIO"] = "";
    //inputData["WAIST_CIRC"] = "";
    //inputData["HIP_CIRC"] = "";
    //inputData["WAIST_TO_HIP_RATIO_NORMAL"] = "";
    //inputData["WAIST_TO_HIP_RATIO_INTERP"] = "";

    //inputData["BP_SYSTOLIC"] = "";
    //inputData["BP_SYSTOLIC_NORMAL"] = "";
    //inputData["BP_SYSTOLIC_INTERP"] = "";
    //inputData["BP_DIASTOLIC"] = "";
    //inputData["BP_DIASTOLIC_NORMAL"] = "";
    //inputData["BP_DIASTOLIC_INTERP"] = "";

    //inputData["FVC"] = "";
    //inputData["FVC_NORMAL"] = "";
    //inputData["FVC_INTERP"] = "";
    //inputData["FEV1"] = "";
    //inputData["FEV1_NORMAL"] = "";
    //inputData["FEV1_INTERP"] = "";
    //inputData["FEV1_FVC"] = "";
    //inputData["FEV1_FVC_NORMAL"] = "";
    //inputData["FEV1_FVC_INTERP"] = "";

    //inputData["FRAX"] = "";

    //inputData["HEARING_RIGHT_500"] = "";
    //inputData["HEARING_LEFT_500"] = "";

    //inputData["HEARING_RIGHT_1000"] = "";
    //inputData["HEARING_LEFT_1000"] = "";

    //inputData["HEARING_RIGHT_2000"] = "";
    //inputData["HEARING_LEFT_2000"] = "";

    //inputData["HEARING_RIGHT_3000"] = "";
    //inputData["HEARING_LEFT_3000"] = "";

    //inputData["HEARING_RIGHT_4000"] = "";
    //inputData["HEARING_LEFT_4000"] = "";

    //inputData["HEARING_RIGHT_6000"] = "";
    //inputData["HEARING_LEFT_6000"] = "";

    //inputData["HEARING_RIGHT_8000"] = "";
    //inputData["HEARING_LEFT_8000"] = "";

    //inputData["VISUAL_ACUITY"] = "";

    //QString currentDir = QDir::currentPath();
    //QString outputPath = filler.fillPDF(
    //    "C:/Users/Anthony/Documents/GitHub/cypress/src/tests/Tests/participant_report_follow_up_3_site_en.pdf",
    //    "C:/Users/Anthony/Documents/GitHub/cypress/src/tests/Tests/participant_report_follow_up_3_site_en.fdf",
    //    inputData,
    //    "C:/Users/Anthony/Documents/GitHub/cypress/src/tests/Tests/participant_report_follow_up_3_site_en_output.pdf");

    //QDesktopServices::openUrl(outputPath);
}

void DeviceSimTests::test_participant_report_fr_sim()
{
    //PDFFormFiller filler;
    //QJsonObject inputData;

    //inputData["DATE"] = "";
    //inputData["NAME"] = "";
    //inputData["AGE"] = "";
    //inputData["HEIGHT"] = "";
    //inputData["WEIGHT"] = "";
    //inputData["BMI"] = "";
    //inputData["BMI_NORMAL"] = "";
    //inputData["BMI_INTERP"] = "";

    //inputData["WAIST_TO_HIP_RATIO"] = "";
    //inputData["WAIST_CIRC"] = "";
    //inputData["HIP_CIRC"] = "";
    //inputData["WAIST_TO_HIP_RATIO_NORMAL"] = "";
    //inputData["WAIST_TO_HIP_RATIO_INTERP"] = "";

    //inputData["BP_SYSTOLIC"] = "";
    //inputData["BP_SYSTOLIC_NORMAL"] = "";
    //inputData["BP_SYSTOLIC_INTERP"] = "";
    //inputData["BP_DIASTOLIC"] = "";
    //inputData["BP_DIASTOLIC_NORMAL"] = "";
    //inputData["BP_DIASTOLIC_INTERP"] = "";

    //inputData["FVC"] = "";
    //inputData["FVC_NORMAL"] = "";
    //inputData["FVC_INTERP"] = "";
    //inputData["FEV1"] = "";
    //inputData["FEV1_NORMAL"] = "";
    //inputData["FEV1_INTERP"] = "";
    //inputData["FEV1_FVC"] = "";
    //inputData["FEV1_FVC_NORMAL"] = "";
    //inputData["FEV1_FVC_INTERP"] = "";

    //inputData["FRAX"] = "";

    //inputData["HEARING_RIGHT_500"] = "";
    //inputData["HEARING_LEFT_500"] = "";

    //inputData["HEARING_RIGHT_1000"] = "";
    //inputData["HEARING_LEFT_1000"] = "";

    //inputData["HEARING_RIGHT_2000"] = "";
    //inputData["HEARING_LEFT_2000"] = "";

    //inputData["HEARING_RIGHT_3000"] = "";
    //inputData["HEARING_LEFT_3000"] = "";

    //inputData["HEARING_RIGHT_4000"] = "";
    //inputData["HEARING_LEFT_4000"] = "";

    //inputData["HEARING_RIGHT_6000"] = "";
    //inputData["HEARING_LEFT_6000"] = "";

    //inputData["HEARING_RIGHT_8000"] = "";
    //inputData["HEARING_LEFT_8000"] = "";

    //inputData["VISUAL_ACUITY"] = "";

    //QString currentDir = QDir::currentPath();
    //QString outputPath = filler.fillPDF(
    //    "C:/Users/Anthony/Documents/GitHub/cypress/src/tests/Tests/participant_report_follow_up_3_site_fr.pdf",
    //    "C:/Users/Anthony/Documents/GitHub/cypress/src/tests/Tests/participant_report_follow_up_3_site_fr.fdf",
    //    inputData,
    //    "C:/Users/Anthony/Documents/GitHub/cypress/src/tests/Tests/participant_report_follow_up_3_site_fr_output.pdf");

    //QDesktopServices::openUrl(outputPath);
}

QTEST_MAIN(DeviceSimTests)

#include "tst_pdf_filler.moc"

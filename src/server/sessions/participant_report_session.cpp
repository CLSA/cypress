#include "participant_report_session.h"
#include "managers/participant_report/participant_report_manager.h"

ParticipantReportSession::ParticipantReportSession(QObject *parent, const QJsonObject& inputData)
    : CypressSession{parent, inputData}
{

}

void ParticipantReportSession::validate() const
{
    CypressSession::validate();


    //inputData["NAME"] = requestData.value("name").toString();
    if (!isValidString("name"))
        throw ValidationError("name");

    //inputData["AGE"] = requestData.value("age").toDouble();
    if (!isValidString("age"))
        throw ValidationError("age");

    //const QString& sex = requestData.value("sex").toString();
    if (!isValidString("sex"))
        throw ValidationError("sex");

    if (!isValidString("height"))
        throw ValidationError("height");

    if (!isValidString("weight"))
        throw ValidationError("weight");

    //inputData["WAIST_CIRC"] = requestData.value("waist_circ").toDouble();
    if (!isValidString("waist_circ"))
        throw ValidationError("waist_circ");

    //inputData["HIP_CIRC"] = requestData.value("hip_circ").toDouble();
    if (!isValidString("hip_circ"))
        throw ValidationError("hip_circ");

    //inputData["WAIST_TO_HIP_RATIO"] = requestData.value("waist_to_hip_ratio").toDouble();
    if (!isValidString("waist_to_hip_ratio"))
        throw ValidationError("waist_to_hip_ratio");

    //inputData["BP_SYSTOLIC"] = requestData.value("bp_systolic").toDouble();
    if (!isValidString("bp_systolic"))
        throw ValidationError("bp_systolic");

    //inputData["BP_DIASTOLIC"] = requestData.value("bp_diastolic").toDouble();
    if (!isValidString("bp_diastolic"))
        throw ValidationError("bp_diastolic");


    //inputData["FVC"] = requestData.value("fvc").toDouble();
    if (!isValidString("fvc"))
        throw ValidationError("fvc");

    //m_inputData["FEV1"] = requestData.value("fev1").toDouble();
    if (!isValidString("fev1"))
        throw ValidationError("fev1");

    //inputData["FEV1_FVC"] = requestData.value("fev1_fvc");
    if (!isValidString("fev1_fvc"))
        throw ValidationError("fev1_fvc");

    //inputData["FRAX"] = requestData.value("frax").toDouble();
    if (!isValidString("frax"))
        throw ValidationError("frax");

    if (!isValidString("hearing_right_500"))
        throw ValidationError("hearing_right_500");

    if (!isValidString("hearing_left_500"))
        throw ValidationError("hearing_left_500");

    if (!isValidString("hearing_right_1000"))
        throw ValidationError("hearing_right_1000");

    if (!isValidString("hearing_left_1000"))
        throw ValidationError("hearing_left_1000");

    if (!isValidString("hearing_right_2000"))
        throw ValidationError("hearing_right_2000");

    if (!isValidString("hearing_left_2000"))
        throw ValidationError("hearing_left_2000");

    if (!isValidString("hearing_left_3000"))
        throw ValidationError("hearing_left_3000");

    if (!isValidString("hearing_left_3000"))
        throw ValidationError("hearing_left_3000");

    if (!isValidString("hearing_right_4000"))
        throw ValidationError("hearing_right_4000");

    if (!isValidString("hearing_left_4000"))
        throw ValidationError("hearing_left_4000");

    if (!isValidString("hearing_right_6000"))
        throw ValidationError("hearing_right_6000");

    if (!isValidString("hearing_left_6000"))
        throw ValidationError("hearing_left_6000");

    if (!isValidString("hearing_right_8000"))
        throw ValidationError("hearing_right_8000");

    if (!isValidString("hearing_left_8000"))
        throw ValidationError("hearing_left_8000");

    if (!isValidString("visual_acuity"))
        throw ValidationError("visual_acuity");
}

void ParticipantReportSession::calculateInputs()
{
    m_inputData["date"] = QDate::currentDate().toString("yyyy-MM-dd");

    QString sex = m_inputData["sex"].toString();

    if (sex == "F")
    {
        m_inputData["WAIST_TO_HIP_RATIO_NORMAL"] = "< 0.8";
    }

    else
    {
        m_inputData["WAIST_TO_HIP_RATIO_NORMAL"] = "< 1.0";
    }

    if (sex == "F")
    {
        m_inputData["WAIST_TO_HIP_RATIO_INTERP"] = "";
    }
    else {
        m_inputData["WAIST_TO_HIP_RATIO_INTERP"] = "";
    }

    m_inputData["bp_systolic_normal"] = "< 130";
    m_inputData["bp_systolic_interp"] = m_inputData.value("bp_systolic").toDouble() < 130 ? "Normal" : m_inputData.value("bp_systolic").toDouble() >= 140 ? "High" : "Borderline";

    m_inputData["bp_diastolic_normal"] = "< 85";
    m_inputData["bp_diastolic_interp"] = m_inputData.value("bp_systolic").toDouble() < 85 ? "Normal" : m_inputData.value("bp_systolic").toDouble() >= 90 ? "High" : "Borderline";

    // bmi
    m_inputData["bmi"] = 0.00;
    m_inputData["bmi_normal"] = "18.5 - 24.9";
    m_inputData["bmi_interp"] = m_inputData["bmi"].toDouble() < 18.5 ? "Low" : m_inputData["bmi"].toDouble() > 24.9 ? "High" : "Low";

    m_inputData["fvc_interp"] = "";

    m_inputData["fvc_normal"] = "";
    m_inputData["fev1_normal"] = "";
    m_inputData["fev1_interp"] = "";

    m_inputData["fev1_fvc_normal"] = "> 0.7";
    m_inputData["fev1_fvc_interp"] = m_inputData.value("fev1_fvc").toDouble() > 0.7 ? "Normal" : "Low";

    m_inputData["hearing_right_500_pass"] = m_inputData.value("hearing_right_500").toDouble() > 40 ? "Fail" : "Pass";
    m_inputData["hearing_left_500_pass"] = m_inputData.value("hearing_left_500").toDouble() > 40 ? "Fail" : "Pass";
    m_inputData["hearing_right_1000_pass"] = m_inputData.value("hearing_right_1000").toDouble() > 40 ? "Fail" : "Pass";
    m_inputData["hearing_left_1000_pass"] = m_inputData.value("hearing_left_1000").toDouble() > 40 ? "Fail" : "Pass";
    m_inputData["hearing_right_2000_pass"] = m_inputData.value("hearing_right_2000").toDouble() > 40 ? "Fail" : "Pass";
    m_inputData["hearing_left_2000_pass"] = m_inputData.value("hearing_left_2000").toDouble() > 40 ? "Fail" : "Pass";
    m_inputData["hearing_right_3000_pass"] = m_inputData.value("hearing_right_3000").toDouble() > 40 ? "Fail" : "Pass";
    m_inputData["hearing_left_3000_pass"] = m_inputData.value("hearing_left_3000").toDouble() > 40 ? "Fail" : "Pass";
    m_inputData["hearing_right_4000_pass"] = m_inputData.value("hearing_right_4000").toDouble() > 40 ? "Fail" : "Pass";
    m_inputData["hearing_left_4000_pass"] = m_inputData.value("hearing_left_4000").toDouble() > 40 ? "Fail" : "Pass";
    m_inputData["hearing_right_6000_pass"] = m_inputData.value("hearing_right_6000").toDouble() > 40 ? "Fail" : "Pass";
    m_inputData["hearing_left_6000_pass"] = m_inputData.value("hearing_left_6000").toDouble() > 40 ? "Fail" : "Pass";
    m_inputData["hearing_right_8000_pass"] = m_inputData.value("hearing_right_8000").toDouble() > 40 ? "Fail" : "Pass";
    m_inputData["hearing_left_8000_pass"] = m_inputData.value("hearing_left_8000").toDouble() > 40 ? "Fail" : "Pass";

    m_inputData["visual_acuity"] = m_inputData.value("visual_acuity").toString();
}

void ParticipantReportSession::start()
{
    m_startDateTime = QDateTime::currentDateTimeUtc();
    m_status = SessionStatus::Started;

    ParticipantReportManager manager(QSharedPointer<ParticipantReportSession>(this));
    manager.start();

    qDebug() << "start session" << getSessionId() << m_startDateTime;
}

void ParticipantReportSession::end()
{

}

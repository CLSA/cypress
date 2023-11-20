#include "participant_report_manager.h"
#include "auxiliary/pdf_form_filler.h"
#include "auxiliary/file_utils.h"

#include <QDir>
#include <QDesktopServices>

ParticipantReportManager::ParticipantReportManager(QSharedPointer<ParticipantReportSession> session): ManagerBase(session)
{

}

void ParticipantReportManager::start()
{
    finish();
}

void ParticipantReportManager::measure()
{

}

void ParticipantReportManager::finish()
{
    PDFFormFiller filler;

    QString currentDir = QDir::currentPath();
    QString outputPath = filler.fillPDF(
        "C:/Users/Anthony/Documents/GitHub/cypress/src/tests/Tests/participant_report_follow_up_3_site_en.pdf",
        "C:/Users/Anthony/Documents/GitHub/cypress/src/tests/Tests/participant_report_follow_up_3_site_en.fdf",
        m_session->getInputData(),
        "C:/Users/Anthony/Documents/GitHub/cypress/src/tests/Tests/participant_report_follow_up_3_site_en_output.pdf");

    QDesktopServices::openUrl(outputPath);

    QString answerUrl = CypressSettings::getAnswerUrl(m_session->getAnswerId());
    QString host = CypressSettings::getPineHost();
    QString endpoint = CypressSettings::getPineEndpoint();

    QByteArray responseData = FileUtils::readFile(outputPath);

    QJsonObject responseJson;
    QJsonObject testJson = {};

    testJson.insert("session", m_session->getJsonObject());
    testJson.insert("results", m_session->getInputData());
    testJson.insert("manual_entry", false);

    responseJson.insert("value", testJson);

    QJsonDocument jsonDoc(responseJson);

    sendHTTPSRequest("PATCH", answerUrl, "application/json", jsonDoc.toJson());

    sendHTTPSRequest("PATCH",
                                host + endpoint + m_session->getAnswerId()
                                    + "?filename=participant_report.pdf",
                                "application/octet-stream",
                                responseData);

    sendComplete(m_session->getSessionId());
    emit success("");
}

void ParticipantReportManager::setInputData(const QVariantMap &)
{

}

bool ParticipantReportManager::setUp()
{
    return true;
}

bool ParticipantReportManager::clearData()
{
    return true;
}

bool ParticipantReportManager::cleanUp()
{
    return true;
}

#include "participant_report_manager.h"

#include "cypress_settings.h"
#include "cypress_session.h"
#include "auxiliary/utilities.h"

#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>

ParticipantReportManager::ParticipantReportManager(QSharedPointer<CypressSession> session)
    : ManagerBase(session)
{

}


void ParticipantReportManager::start()
{
    qDebug() << "ParticipantReportManager::start()";
}


void ParticipantReportManager::measure()
{
    qDebug() << "ParticipantReportManager::measure()";
    QString reportPath{m_session->getLanguage() == "en" ? "participant_report_en_path"
                                                        : "participant_report_fr_path"};

    QUrl url;
    url = QUrl::fromLocalFile(m_settings.readSetting(reportPath).toString());

    // Open the file
    Utilities::openUrl(url);
}

void ParticipantReportManager::finish()
{
    qDebug() << "ParticipantReportManager::finish()";
}

void ParticipantReportManager::setInputData(const QVariantMap& inputData)
{
    m_inputData = inputData;
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




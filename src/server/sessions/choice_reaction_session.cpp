#include "choice_reaction_session.h"
#include "cypress_application.h"

#include "dialogs/choice_reaction_dialog.h"
#include "managers/choice_reaction/choice_reaction_manager.h"

#include "auxiliary/json_settings.h"

ChoiceReactionSession::ChoiceReactionSession(QObject *parent, const QJsonObject& inputData, const QString& origin)
    : CypressSession{parent, inputData, origin}
{
    qDebug().noquote() << JsonSettings::prettyPrintJson(inputData);
}

void ChoiceReactionSession::initializeDialog()
{
    m_dialog = new ChoiceReactionDialog(nullptr, QSharedPointer<ChoiceReactionSession>(this));
}

void ChoiceReactionSession::isInstalled() const
{
    if (ChoiceReactionManager::config.hasErrors())
        throw NotInstalledError("Choice Reaction is not installed on this workstation");
}

void ChoiceReactionSession::isAvailable() const
{
    // throw a not available exception if this device is already in use

    QList<QSharedPointer<CypressSession>> activeSessions = Cypress::getInstance().getActiveSessions();

    for (auto activeSession : activeSessions)
    {
        try {
            ChoiceReactionSession* session = dynamic_cast<ChoiceReactionSession*>(activeSession.get());
            if (session == nullptr)
                continue;

            if (CypressSettings::isDebugMode())
                qDebug() << "found already active session for choice reaction: " << session->getSessionId();

            throw NotAvailableError("Device in use");
        }
        catch (const std::bad_cast& e)
        {
            if (CypressSettings::isDebugMode())
                qDebug() << "session downcast failed: " << activeSession->getSessionId() << e.what();

            continue;
        }
    }
}

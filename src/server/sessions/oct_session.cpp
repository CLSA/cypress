#include "oct_session.h"

#include "cypress_session.h"
#include "dialogs/oct_dialog.h"

OCTSession::OCTSession(QObject *parent, const QJsonObject& inputData, const QString& origin)
    : CypressSession{parent, inputData, origin}
{

}

void OCTSession::initializeDialog()
{
    m_dialog = new OCTDialog(nullptr, QSharedPointer<OCTSession>(this));
}

void OCTSession::isInstalled() const
{
    //throw NotInstalledError("Grip Strength is not installed on this workstation");
}

void OCTSession::isAvailable() const
{

}

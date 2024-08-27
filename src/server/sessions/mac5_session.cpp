#include "mac5_session.h"
#include "dialogs/mac5_dialog.h"
#include "managers/ecg/mac5_manager.h"

Mac5Session::Mac5Session(QObject *parent, const QJsonObject &inputData, const QString &origin)
    : CypressSession(parent, inputData, origin)
{

}

void Mac5Session::initializeDialog()
{
    m_dialog = new Mac5Dialog(nullptr, QSharedPointer<Mac5Session>(this));
}

void Mac5Session::isInstalled() const
{
    if (Mac5Manager::config.hasErrors())
        throw NotInstalledError("MAC 5 is not installed on this workstation");
}

void Mac5Session::isAvailable() const
{

}

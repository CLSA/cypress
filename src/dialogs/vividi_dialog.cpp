#include "vividi_dialog.h"

#include "cypress_application.h"
#include <QCloseEvent>

VividiDialog::VividiDialog(QJsonObject requestData)
{

}

void VividiDialog::userClose()
{
    DialogBase::userClose();
    CypressApplication::getInstance().forceSessionEnd();
}

void VividiDialog::closeEvent(QCloseEvent* event)
{
    qDebug() << "AudiometerDialog::handleClose";
    event->ignore();
    if (m_user_close)
    {
        m_manager->sendComplete("vividi", m_manager->m_uuid);
    }
    else
    {
        CypressApplication::getInstance().dialog = nullptr;
        m_manager->sendCancellation("vividi", m_manager->m_uuid);
    }
    event->accept();
}

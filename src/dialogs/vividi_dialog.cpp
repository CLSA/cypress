#include "vividi_dialog.h"

#include "cypress_application.h"
#include <QCloseEvent>

VividiDialog::VividiDialog(QJsonObject requestData)
{

}

void VividiDialog::userClose()
{
    DialogBase::userClose();
    if (m_user_close)
    {
        m_manager->sendComplete("vividi", m_manager->m_uuid);
    }
    else
    {
        m_manager->sendCancellation("vividi", m_manager->m_uuid);
    }
    Cypress::getInstance().forceSessionEnd();
}

void VividiDialog::closeEvent(QCloseEvent* event)
{
    qDebug() << "AudiometerDialog::handleClose";
    event->accept();
}

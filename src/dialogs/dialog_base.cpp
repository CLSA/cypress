#include "dialog_base.h"

#include "cypress_application.h"
#include "cypress_session.h"
#include "managers/manager_base.h"

#include <QCoreApplication>
#include <QCloseEvent>
#include <QDebug>
#include <QDir>
#include <QJsonObject>
#include <QJsonDocument>
#include <QSettings>
#include <QCloseEvent>
#include <QMessageBox>

DialogBase::DialogBase(QWidget *parent, QSharedPointer<CypressSession> session)
    : QDialog(parent)
    , m_session(session)
{
    setWindowFlags(Qt::WindowFullscreenButtonHint);
    setAttribute(Qt::WA_DeleteOnClose);

    m_debug = CypressSettings::isDebugMode();
    m_sim = CypressSettings::isSimMode();
}

DialogBase::~DialogBase() {}

bool DialogBase::run()
{
    return m_manager->start();
}


void DialogBase::closeEvent(QCloseEvent* event)
{
    if (m_session->getStatus() == CypressSession::SessionStatus::CriticalError) {
        QDialog::closeEvent(event);
        return;
    }

    if (m_session->getStatus() == CypressSession::SessionStatus::Cancelled) {
        m_manager->cleanUp();
        QDialog::closeEvent(event);
        return;
    }

    if (m_session->getStatus() == CypressSession::SessionStatus::Started) {
        QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Confirmation",
        "Are you sure you want to close? Any unsaved measurements will be lost.",
        QMessageBox::Yes|QMessageBox::No
    );
        if (reply != QMessageBox::Yes)
            event->ignore();
        else
            cancel("");
    } else {
        QDialog::closeEvent(event);
    }
}

void DialogBase::cancel(const QString& cancelMsg)
{
    Q_UNUSED(cancelMsg);
    qDebug() << "DialogBase::cancel";

    if (!m_sim)
        m_manager->sendCancellation(m_session->getSessionId());

    Cypress::getInstance().endSession(m_session->getSessionId(), CypressSession::Cancelled);

    close();
}

void DialogBase::error(const QString& errorMsg)
{
    QMessageBox::critical(nullptr, "Error", errorMsg);

    if (!m_sim)
        m_manager->sendCancellation(m_session->getSessionId());

    Cypress::getInstance().endSession(m_session->getSessionId(), CypressSession::CriticalError);

    close();
}

void DialogBase::success(const QString& successMsg)
{
    QMessageBox::information(nullptr, "Measurement complete", successMsg);

    if (!m_sim)
        m_manager->sendComplete(m_session->getSessionId());

    Cypress::getInstance().endSession(m_session->getSessionId(), CypressSession::Success);

    close();
}

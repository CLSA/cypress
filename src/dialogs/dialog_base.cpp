#include "cypress_application.h"
#include "cypress_session.h"

#include "dialog_base.h"
#include "managers/manager_base.h"

#include "auxiliary/json_settings.h"

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

    connect(this, SIGNAL(finished(int)), this, SLOT(deleteLater()));

    this->setAttribute(Qt::WA_DeleteOnClose);

    startTime = QDateTime::currentDateTimeUtc();
}

void DialogBase::initialize()
{
    initializeModel();
    initializeConnections();
}

DialogBase::~DialogBase()
{
    qDebug() << "destroy dialog";
}

void DialogBase::initializeModel()
{

}

void DialogBase::initializeConnections()
{

}

void DialogBase::run()
{
    initialize();
    m_manager->start();
    show();
}


void DialogBase::closeEvent(QCloseEvent* event)
{
    if (m_session->getStatus() != SessionStatus::Ended) {
        cancel("");
    }

    QDialog::closeEvent(event);
}

void DialogBase::cancel(const QString& cancelMsg)
{
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Confirmation",
        !cancelMsg.isEmpty() ? cancelMsg : "Are you sure you want to close? Any unsaved measurements will be lost.",
        QMessageBox::Yes|QMessageBox::No
    );

    if (reply != QMessageBox::Yes)
    {
        return;
    }

    m_manager->sendCancellation(m_session->getSessionId());

    Cypress::getInstance().endSession(m_session->getSessionId());

    close();
}

void DialogBase::error(const QString& errorMsg)
{
    QMessageBox::critical(this, "Error", !errorMsg.isEmpty() ? errorMsg : "Unknown error");

    m_manager->sendCancellation(m_session->getSessionId());

    Cypress::getInstance().endSession(m_session->getSessionId());

    close();
}

void DialogBase::success(const QString& successMsg)
{
    QMessageBox::information(this, "Complete", !successMsg.isEmpty() ? successMsg : "The data has been saved to Pine");

    m_manager->sendComplete(m_session->getSessionId());

    Cypress::getInstance().endSession(m_session->getSessionId());

    close();
}

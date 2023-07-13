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

DialogBase::DialogBase(QWidget* parent, const CypressSession& session)
    : QDialog(parent),
    m_session(session)
{
    connect(this, SIGNAL(finished(int)), this, SLOT(deleteLater()));

    this->setAttribute(Qt::WA_DeleteOnClose);

    if (Cypress::getInstance().isSimulation())
    {
        setWindowTitle(windowTitle() + "(SIM)");
    }
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
    bool sessionEnded = Cypress::getInstance().endSession(m_session.getSessionId());
    if (!sessionEnded)
    {
        event->ignore();
    }
    else
    {
        QDialog::closeEvent(event);
    }
}

void DialogBase::complete(QJsonObject& results)
{
    //results["id"] = m_uuid;
    emit sendResults(results);
}

#include "dialog_base.h"
#include "auxiliary/json_settings.h"
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

DialogBase::DialogBase(QWidget *parent, QString uuid)
    : QDialog(parent)
    , m_verbose(false)
{
    m_uuid = uuid;
    this->setAttribute(Qt::WA_DeleteOnClose);

}

void DialogBase::initialize()
{
  initializeModel();
  initializeConnections();
}

void DialogBase::run()
{
    initialize();
    m_manager->start();
}

void DialogBase::userClose()
{
    m_user_close = true;
}

void DialogBase::initializeModel()
{

}

void DialogBase::closeEvent(QCloseEvent *event)
{
    event->accept();
    //m_manager.get()->finish();
}

void DialogBase::complete(QJsonObject &results)
{
    results["id"] = m_uuid;
    emit sendResults(results);
}

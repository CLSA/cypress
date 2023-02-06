#include "DialogBase.h"
#include "auxiliary/JsonSettings.h"
#include "managers/ManagerBase.h"

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

void DialogBase::initializeModel()
{

}

void DialogBase::closeEvent(QCloseEvent *event)
{
    event->ignore();
    if (QMessageBox::No == QMessageBox::question(this, "Close Confirmation", "Are you sure you want to exit? Any unsaved changes will be lost.", QMessageBox::Yes | QMessageBox::No)) {
        return;
    }

    if(m_verbose)
        qDebug() << "Close event called";

    m_manager->finish();
    event->accept();
}

void DialogBase::complete(QJsonObject &results)
{
    results["id"] = m_uuid;
    emit sendResults(results);
}

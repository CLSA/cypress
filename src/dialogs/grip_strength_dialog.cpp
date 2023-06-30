#include "grip_strength_dialog.h"
#include "managers/grip_strength/grip_strength_manager.h"
#include "cypress_application.h"

#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>


GripStrengthDialog::GripStrengthDialog(QJsonObject inputData): ui(new Ui::GripStrengthDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowFullscreenButtonHint);
    m_manager.reset(new GripStrengthManager(inputData));

    ui->measureButton->setEnabled(true);

    connect(ui->measureButton, &QPushButton::clicked, m_manager.get(), &ManagerBase::measure);
}

GripStrengthDialog::~GripStrengthDialog()
{
    delete ui;
}

void GripStrengthDialog::initializeConnections()
{
    //m_manager.start();
}

bool GripStrengthDialog::handleClose() {
    return this -> close();
}

void GripStrengthDialog::enableMeasure() {
    //ui->measureWidget->enableMeasure();
}

void GripStrengthDialog::initializeModel()
{
}

void GripStrengthDialog::userClose()
{
    DialogBase::userClose();
    qDebug() << "GripStrengthDialog::handleClose";
    if (m_user_close)
    {
        m_manager->sendComplete("grip_strength", m_manager->m_uuid);
    }
    else
    {
        Cypress::getInstance().deviceDialog = nullptr;
        m_manager->sendCancellation("grip_strength", m_manager->m_uuid);
    }
    Cypress::getInstance().forceSessionEnd();
}

void GripStrengthDialog::closeEvent(QCloseEvent* event)
{
    event->accept();
}




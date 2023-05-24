#include "GripStrengthDialog.h"

#include <QDebug>
#include <QMessageBox>
#include "managers/grip_strength/GripStrengthManager.h"

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




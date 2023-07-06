#include "grip_strength_dialog.h"
#include "managers/grip_strength/grip_strength_manager.h"
#include "cypress_application.h"

#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>


GripStrengthDialog::GripStrengthDialog(QWidget* parent, const CypressSession& session):
    DialogBase(parent, session),
    ui(new Ui::GripStrengthDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowFullscreenButtonHint);

    m_manager.reset(new GripStrengthManager(session));

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

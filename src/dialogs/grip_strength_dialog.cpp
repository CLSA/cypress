#include "grip_strength_dialog.h"
#include "managers/grip_strength/grip_strength_manager.h"

#include <QCloseEvent>
#include <QDebug>
#include <QMessageBox>

GripStrengthDialog::GripStrengthDialog(QWidget *parent, QSharedPointer<GripStrengthSession> session)
    : DialogBase(parent, session)
    , ui(new Ui::GripStrengthDialog)
{
    ui->setupUi(this);

    //ui->measurementTable->disableMeasureButton();
    //ui->measurementTable->disableFinishButton();

    //if (!m_sim)
    //ui->measurementTable->hideMeasureButton();

    this->setWindowTitle("Grip Strength");
    this->setWindowFlags(Qt::WindowFullscreenButtonHint);

    m_manager.reset(new GripStrengthManager(session));

    QSharedPointer<GripStrengthManager> manager = qSharedPointerCast<GripStrengthManager>(m_manager);
    ui->testInfoWidget->setSessionInformation(*session);

    // finished
    connect(manager.get(), &GripStrengthManager::success, this, &GripStrengthDialog::success);

    // critical error
    connect(manager.get(), &GripStrengthManager::error, this, &GripStrengthDialog::error);
}

GripStrengthDialog::~GripStrengthDialog()
{
    delete ui;
}

#include "spirometer_dialog.h"
#include "managers/spirometer/spirometer_manager.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>

SpirometerDialog::SpirometerDialog(QWidget *parent, QSharedPointer<SpirometerSession> session)
    : DialogBase(parent, session)
    , ui(new Ui::SpirometerDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowFullscreenButtonHint);

    m_manager.reset(new SpirometerManager(session));

    QSharedPointer<SpirometerManager> manager = qSharedPointerCast<SpirometerManager>(m_manager);

    ui->testInfoWidget->setSessionInformation(*session);

    connect(manager.get(), &SpirometerManager::success, this, &SpirometerDialog::success);
    connect(manager.get(), &SpirometerManager::error, this, &SpirometerDialog::error);
}

SpirometerDialog::~SpirometerDialog()
{
    delete ui;
}

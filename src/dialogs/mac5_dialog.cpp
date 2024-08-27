#include "mac5_dialog.h"
#include "ui_mac5_dialog.h"

#include "managers/ecg/mac5_manager.h"

Mac5Dialog::Mac5Dialog(QWidget *parent, QSharedPointer<Mac5Session> session) : DialogBase(parent, session),
    ui(new Ui::Mac5Dialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowFullscreenButtonHint);

    m_manager.reset(new Mac5Manager(session));
    this->setWindowTitle("MAC 5");
    ui->testInfoWidget->setSessionInformation(*session);

    auto manager = qSharedPointerCast<Mac5Manager>(m_manager);

    // finished
    connect(manager.get(), &Mac5Manager::success, this, &Mac5Dialog::success);

    // critical error
    connect(manager.get(), &Mac5Manager::error, this, &Mac5Dialog::error);
}

Mac5Dialog::~Mac5Dialog()
{
    delete ui;
}



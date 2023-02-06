#include <QMessageBox>

#include "CypressApplication.h"
#include "RetinalCameraDialog.h"
#include "managers/retinal_camera/RetinalCameraManager.h"
#include "ui_RetinalCameraDialog.h"

RetinalCameraDialog::RetinalCameraDialog(QString uuid) : ui(new Ui::RetinalCameraDialog)
{
    m_uuid = uuid;

    ui->setupUi(this);
    setWindowFlags(Qt::WindowFullscreenButtonHint);

    if (CypressApplication::mode == Mode::Sim)
    {
        setWindowTitle("Retinal Camera (SIM)");
        ui->measureWidget->enableMeasure();
    }

    m_manager.reset(new RetinalCameraManager(m_uuid));
}

RetinalCameraDialog::~RetinalCameraDialog()
{
    delete ui;
}

void RetinalCameraDialog::initializeModel()
{
}

void RetinalCameraDialog::initializeConnections()
{
    QSharedPointer<RetinalCameraManager> derived = m_manager.staticCast<RetinalCameraManager>();
    connect(ui->measureWidget, &MeasureWidget::measure, derived.get(), &RetinalCameraManager::measure);

    //this->run();
   // connect(ui->barcodeWidget, &BarcodeWidget::validated,
   //       this,[this](const bool& valid)
   // {
   //   if(valid)
   //   {
   //       // launch the manager
   //       //
   //       this->run();
   //   }
   //   else
   //   {
   //       QMessageBox::critical(
   //         this, QApplication::applicationName(),
   //         tr("The input does not match the expected barcode for this participant."));
   //   }
    //});
}

bool RetinalCameraDialog::handleClose()
{
    return this -> close();
}

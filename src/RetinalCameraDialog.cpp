#include <QMessageBox>

#include "RetinalCameraDialog.h"
#include "managers/RetinalCameraManager.h"
#include "ui_RetinalCameraDialog.h"

RetinalCameraDialog::RetinalCameraDialog(QWidget *parent) :
    DialogBase(parent),
    ui(new Ui::RetinalCameraDialog)
{
    setWindowFlags(Qt::Window);
    ui->setupUi(this);

    m_manager.reset(new RetinalCameraManager(this));
}

RetinalCameraDialog::~RetinalCameraDialog()
{
    delete ui;
}


QString RetinalCameraDialog::getVerificationBarcode() const
{
    return ui->barcodeWidget->barcode();
}

void RetinalCameraDialog::setVerificationBarcode(const QString& barcode)
{
    ui->barcodeWidget->setBarcode(barcode);
}

void RetinalCameraDialog::initializeModel()
{
    setVerificationBarcode(Constants::DefaultBarcode);
}


void RetinalCameraDialog::initializeConnections()
{
    if (m_verbose)
        qDebug() << "initializeConnections";

    connect(ui->measureWidget, &MeasureWidget::closeApplication, this, &RetinalCameraDialog::handleClose);

    connect(ui->barcodeWidget, &BarcodeWidget::validated,
          this,[this](const bool& valid)
    {
      if(valid)
      {
          // launch the manager
          //
          this->run();
      }
      else
      {
          QMessageBox::critical(
            this, QApplication::applicationName(),
            tr("The input does not match the expected barcode for this participant."));
      }
  });
}

bool RetinalCameraDialog::handleClose()
{
    return this -> close();
}

#include "GripStrengthDialog.h"
#include "managers/grip_strength/GripStrengthManager.h"

#include <QDebug>
#include <QMessageBox>

GripStrengthDialog::GripStrengthDialog(QWidget *parent):
    DialogBase(parent),
    ui(new Ui::GripStrengthDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowFullscreenButtonHint);

    m_manager.reset(new GripStrengthManager(this));
}

GripStrengthDialog::~GripStrengthDialog()
{
    delete ui;
}

QString GripStrengthDialog::getVerificationBarcode() const
{
  return ui->barcodeWidget->barcode();
}

void GripStrengthDialog::setVerificationBarcode(const QString &barcode)
{
    ui->barcodeWidget->setBarcode(barcode);
}

void GripStrengthDialog::initializeConnections()
{
    setVerificationBarcode(Constants::DefaultBarcode);

    QSharedPointer<GripStrengthManager> derived = m_manager.staticCast<GripStrengthManager>();
    //connect(derived.get(), &GripStrengthManager::measureStart, this, &GripStrengthDialog::enableMeasure);
    //connect(derived.get(), &GripStrengthManager::dataChanged, ui->measureWidget, &MeasureWidget::updateModelView);

    connect(ui->measureWidget, &MeasureWidget::closeApplication, this, &GripStrengthDialog::handleClose);

    run();
    //connect(ui->measureWidget, &MeasureWidget::measure, derived.get(), &GripStrengthManager::measure);
    //connect(ui->measureWidget, &MeasureWidget::writeToFile, this, &GripStrengthDialog::writeOutput);

    // connect(ui->barcodeWidget, &BarcodeWidget::validated,
    //       this,[this](const bool& valid)
    // {
    //   if(valid)
    //   {
    //       // launch the manager
    //       //
    //       qDebug() << "run";
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

bool GripStrengthDialog::handleClose() {
    return this -> close();
}

void GripStrengthDialog::enableMeasure() {
    ui->measureWidget->enableMeasure();
}

void GripStrengthDialog::initializeModel()
{
    m_manager.get()->initializeModel();
    ui->measureWidget->initialize(m_manager.get()->getModel());
}




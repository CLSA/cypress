#include "signaturepaddialog.h"
#include "ui_signaturepaddialog.h"
#include "managers/signature_pad/signaturepadmanager.h"

SignaturePadDialog::SignaturePadDialog(QString uuid) : ui(new Ui::SignaturePadDialog)
{
    m_uuid = uuid;
    ui->setupUi(this);
    setWindowFlags(Qt::WindowFullscreenButtonHint);

    m_manager.reset(new SignaturePadManager());

    connect(m_manager.get(), &SignaturePadManager::displaySignature, this, &SignaturePadDialog::renderSignature);
    m_manager->start();
}

SignaturePadDialog::~SignaturePadDialog()
{
    delete ui;
}

void SignaturePadDialog::initializeConnections() {

}

void SignaturePadDialog::initializeModel() {

}

void SignaturePadDialog::on_ResetButton_clicked()
{
    m_manager->restart();
}

void SignaturePadDialog::on_SubmitButton_clicked()
{
   m_manager->measure();
}

void SignaturePadDialog::renderSignature(const QByteArray& bytes)
{
    QPixmap mpixmap;
    bool ok = mpixmap.loadFromData(bytes);
    if (!ok)
    {
        qDebug() << "can't create pixmap";
    }
    else {
        mpixmap = mpixmap.scaled(800, 160, Qt::KeepAspectRatio);
        ui->label->setPixmap(mpixmap);
    }
}


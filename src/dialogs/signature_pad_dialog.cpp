#include "signature_pad_dialog.h"
#include "ui_signature_pad_dialog.h"
#include "managers/signature_pad/signature_pad_manager.h"
#include "cypress_application.h"

#include <QCloseEvent>

SignaturePadDialog::SignaturePadDialog(QWidget* parent, const CypressSession& session) :
    DialogBase(parent, session),
    ui(new Ui::SignaturePadDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowFullscreenButtonHint);

    //m_manager.reset(new SignaturePadManager(inputData));
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
    ui->SubmitButton->setEnabled(true);
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

#include "signaturepaddialog.h"
#include "ui_signaturepaddialog.h"
#include "managers/signature_pad/signaturepadmanager.h"

SignaturePadDialog::SignaturePadDialog(QString uuid) : ui(new Ui::SignaturePadDialog)
{

    m_uuid = uuid;
    ui->setupUi(this);
    setWindowFlags(Qt::WindowFullscreenButtonHint);

    m_manager.reset(new SignaturePadManager());
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

void SignaturePadDialog::on_graphicsView_rubberBandChanged(const QRect &viewportRect, const QPointF &fromScenePoint, const QPointF &toScenePoint)
{
    qDebug() << "graphics view rubber band changed";
}

void SignaturePadDialog::closeEvent(QCloseEvent *event)
{
    m_manager->finish();
    this -> close();
}

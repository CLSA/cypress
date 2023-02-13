#include "signaturepadmanager.h"
#include "signaturepadcommunication.h"

SignaturePadManager::SignaturePadManager()
{
    SignaturePadCommunication* spc = new SignaturePadCommunication();

    spc->moveToThread(&captureThread);
    qDebug() << "move to thread";

    connect(&captureThread, &QThread::finished, spc, &QObject::deleteLater);
    qDebug() << "connect 1";

    connect(this, &SignaturePadManager::startCapture, spc, &SignaturePadCommunication::start);
    qDebug() << "connect 2";

    captureThread.start();
    qDebug() << "start";
}

SignaturePadManager::~SignaturePadManager()
{
    captureThread.quit();
    captureThread.wait();
}

void SignaturePadManager::start()
{
    emit startCapture();
}

void SignaturePadManager::measure()
{
}

void SignaturePadManager::finish()
{
}

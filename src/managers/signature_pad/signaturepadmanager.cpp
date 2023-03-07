#include "signaturepadmanager.h"
#include "signaturepadcommunication.h"

SignaturePadManager::SignaturePadManager()
{
    spc.reset(new SignaturePadCommunication());
    spc->moveToThread(&captureThread);

    connect(this, &SignaturePadManager::startCapture, spc.get(), &SignaturePadCommunication::start);
    connect(spc.get(), &SignaturePadCommunication::signatureOk, this, &SignaturePadManager::receiveSignature);
}

SignaturePadManager::~SignaturePadManager()
{
    captureThread.requestInterruption();
    captureThread.quit();
    captureThread.wait();

    qDebug() << "destroy sig pad manager";
}

void SignaturePadManager::start()
{
    captureThread.start();
    emit startCapture();
}

void SignaturePadManager::measure()
{
    QJsonObject response {{
        "value", QString(signature.toBase64())
    }};

    sendResultsToPine(response);
}

void SignaturePadManager::restart()
{
    disconnect(&captureThread, &QThread::finished, spc.get(), &QObject::deleteLater);
    disconnect(this, &SignaturePadManager::startCapture, spc.get(), &SignaturePadCommunication::start);

    captureThread.requestInterruption();
    captureThread.quit();
    captureThread.wait();

    spc->moveToThread(QThread::currentThread());
    spc.reset(new SignaturePadCommunication());
    spc->moveToThread(&captureThread);

    connect(&captureThread, &QThread::finished, spc.get(), &QObject::deleteLater);
    connect(this, &SignaturePadManager::startCapture, spc.get(), &SignaturePadCommunication::start);
    connect(spc.get(), &SignaturePadCommunication::signatureOk, this, &SignaturePadManager::receiveSignature);

    captureThread.start();

    emit startCapture();
}

void SignaturePadManager::finish()
{
    captureThread.requestInterruption();
    captureThread.quit();
    captureThread.wait();
}

void SignaturePadManager::receiveSignature(const QByteArray& bytes)
{
    signature = bytes;
    emit displaySignature(signature);
}

// Reset the session
bool SignaturePadManager::clearData() {
   return true;
}

// Set up device
bool SignaturePadManager::setUp() {
    return true;
}

// Clean up the device for next time
bool SignaturePadManager::cleanUp() {
    return true;
}

// set input parameters for the test
void SignaturePadManager::setInputData(const QVariantMap& inputData)
{

}

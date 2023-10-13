#include "signature_pad_manager.h"
#include "signature_pad_communication.h"

#include "cypress_session.h"

#include "auxiliary/json_settings.h"

SignaturePadManager::SignaturePadManager(const CypressSession& session):
    ManagerBase(session)
{
    spc.reset(new SignaturePadCommunication());
    spc->moveToThread(&captureThread);

    connect(this, &SignaturePadManager::startCapture, spc.get(), &SignaturePadCommunication::start);
    connect(spc.get(), &SignaturePadCommunication::signatureOk, this, &SignaturePadManager::receiveSignature);

    qDebug() << m_inputData.value("answer_id").toInt();
    qDebug() << m_inputData.value("barcode").toString();
}

SignaturePadManager::~SignaturePadManager()
{
    captureThread.requestInterruption();
    captureThread.quit();
    captureThread.wait();

    qDebug() << "destroy sig pad manager";
}

bool SignaturePadManager::isInstalled()
{
    return false;
}

bool SignaturePadManager::isAvailable()
{
    return false;
}

void SignaturePadManager::start()
{
    captureThread.start();
    emit startCapture();
}

void SignaturePadManager::measure()
{
    //QJsonObject response {
    //    { "answer_id", m_answerId },
    //    { "session_id", m_uuid },
    //    { "barcode", m_participantId },
    //};

    //response["cypress_session"] = m_uuid;
    //response["answer_id"] = m_answerId;
    //response["barcode"] = m_barcode;
    //response["interviewer"] = m_interviewer;

    //sendResultsToPine(response);

    //if (!signature.isEmpty()) {
    //    sendFileToPine(signature, "signature.bmp");
    //}
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
    Q_UNUSED(inputData)
    //m_answerId = inputData.value("answerId").toInt();
    // m_participantId = inputData.value("participantId").toString();
}

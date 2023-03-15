#include "signaturepadmanager.h"
#include "signaturepadcommunication.h"

#include "auxiliary/JsonSettings.h"

#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/Net/SecureStreamSocket.h"
#include "Poco/Net/AcceptCertificateHandler.h"

#include "Poco/StreamCopier.h"
#include "Poco/Path.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"
#include <sstream>

SignaturePadManager::SignaturePadManager(QJsonObject inputData)
{
    spc.reset(new SignaturePadCommunication());
    spc->moveToThread(&captureThread);

    connect(this, &SignaturePadManager::startCapture, spc.get(), &SignaturePadCommunication::start);
    connect(spc.get(), &SignaturePadCommunication::signatureOk, this, &SignaturePadManager::receiveSignature);

    m_answerId = inputData.value("answer_id").toString();
    m_participantId = inputData.value("barcode").toString();
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
    QJsonObject response {
        {"answerId", m_answerId},
        {"participantId", m_participantId}
    };

    //sendResultsToPine(response);
    sendRawDataToPine(signature);
}

void SignaturePadManager::sendRawDataToPine(QByteArray data)
{
    try
    {
        Poco::Net::initializeSSL();
        Poco::SharedPtr<Poco::Net::InvalidCertificateHandler> ptrHandler = new Poco::Net::AcceptCertificateHandler(false);
        Poco::Net::Context::Ptr ptrContext = new Poco::Net::Context(
            Poco::Net::Context::CLIENT_USE, "", "", "",
            Poco::Net::Context::VERIFY_RELAXED, 9, true,
            "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH"
        );
        Poco::Net::SSLManager::instance().initializeClient(0, ptrHandler, ptrContext);

        //Poco::Net::SocketAddress address("drummer.clsa-elcv.ca:443");
        //Poco::Net::SecureStreamSocket socket(address);
        //if (socket.havePeerCertificate())
        //{
        //    Poco::Net::X509Certificate cert = socket.peerCertificate();
        //}

        Poco::URI uri(QString("https://drummer.clsa-elcv.ca/patrick/pine/api/answer/" + m_answerId + "?filename=signature.bmp").toStdString());
        Poco::Net::HTTPSClientSession session(uri.getHost(), uri.getPort());

        qDebug() << QString("https://drummer.clsa-elcv.ca/patrick/pine/api/answer/" + m_answerId + "?filename=signature.bmp");

        std::string path(uri.getPathAndQuery());
        if (path.empty()) path = "/";

        Poco::Net::HTTPRequest req(Poco::Net::HTTPRequest::HTTP_PATCH, path, Poco::Net::HTTPMessage::HTTP_1_1);

        req.setContentType("application/octet-stream");
        req.setContentLength(data.length());
        req.setCredentials("Basic", QString("cypress:H9DqvCGjJdJE").toUtf8().toBase64().toStdString());

        std::ostream &os = session.sendRequest(req);
        os << data.constData();
        os.flush();

        Poco::Net::HTTPResponse res;

        std::istream &is = session.receiveResponse(res);
        std::stringstream ss;
        Poco::StreamCopier::copyStream(is, ss);
    }
    catch (Poco::Exception &e)
    {
        qDebug() << e.code();
        qDebug() << e.what();
    }

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
    m_answerId = inputData.value("answerId").toString();
    m_participantId = inputData.value("participantId").toString();
}

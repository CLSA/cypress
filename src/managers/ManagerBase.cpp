#include "ManagerBase.h"
#include <QStandardItemModel>
#include "server/utils.h"
#include <QException>
#include <QDebug>

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

#include "auxiliary/JsonSettings.h"

ManagerBase::ManagerBase()
{
}

ManagerBase::~ManagerBase()
{
}

bool ManagerBase::sendResultsToPine(const QJsonObject& results)
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
        const QString& pinePath = QString("https://drummer.clsa-elcv.ca/patrick/pine/api/answer/" + QString::number(m_answerId));
        qDebug() << pinePath;

        Poco::URI uri(pinePath.toStdString());
        Poco::Net::HTTPSClientSession session(uri.getHost(), uri.getPort());

        std::string path(uri.getPathAndQuery());
        if (path.empty()) path = "/";

        // Serialize and send data
        QString dataString = JsonSettings::serializeJson(results);

        Poco::Net::HTTPRequest req(Poco::Net::HTTPRequest::HTTP_PATCH, path, Poco::Net::HTTPMessage::HTTP_1_1);
        req.setContentType("application/json");
        req.setContentLength(dataString.length());
        std::ostream &os = session.sendRequest(req);
        os << dataString.toStdString();
        os.flush();

        // Get response
        Poco::Net::HTTPResponse res;
        std::istream &is = session.receiveResponse(res);
        std::stringstream ss;
        Poco::StreamCopier::copyStream(is, ss);
    }
    catch (Poco::Exception &e)
    {
        qDebug() << e.what();
    }

    return false;
}

bool ManagerBase::sendFileToPine(const QByteArray& rawData, const QString& fileName)
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

        const QString& pinePath = QString("https://drummer.clsa-elcv.ca/patrick/pine/api/answer/" + QString::number(m_answerId) + "?filename=" + fileName);
        qDebug() << pinePath;

        Poco::URI uri(pinePath.toStdString());
        Poco::Net::HTTPSClientSession session(uri.getHost(), uri.getPort());

        std::string path(uri.getPathAndQuery());
        if (path.empty()) path = "/";

        Poco::Net::HTTPRequest req(Poco::Net::HTTPRequest::HTTP_PATCH, path, Poco::Net::HTTPMessage::HTTP_1_1);

        req.setContentType("application/octet-stream");
        req.setContentLength(rawData.length());
        req.setCredentials("Basic", QString("cypress:H9DqvCGjJdJE").toUtf8().toBase64().toStdString());

        std::ostream &os = session.sendRequest(req);
        os.write(rawData.constData(), rawData.size());
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

QJsonObject Utils::httpPost(const QJsonObject& data)
{

}

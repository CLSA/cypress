#include "file_utils.h"

#include <QDir>
#include <QFileInfo>
#include <QCryptographicHash>

#include <stdexcept>

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


QJsonObject FileUtils::readJsonFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        switch (file.error()) {
            case QFile::OpenError:
                throw std::runtime_error("Open error");
            case QFile::PermissionsError:
                throw std::runtime_error("Permission error");
            case QFile::ReadError:
                throw std::runtime_error("Permission error");
            default:
                throw std::runtime_error(file.errorString().toStdString());
        }
    }

    QByteArray jsonData = file.readAll();
    QJsonDocument jsonDoc(QJsonDocument::fromJson(jsonData));

    return jsonDoc.object();
}

void FileUtils::sendHTTPSRequest(const QString& method, const QString& endpoint, const QString& contentType, const QByteArray& data)
{
    Q_UNUSED(method)

    Poco::Net::initializeSSL();
    Poco::SharedPtr<Poco::Net::InvalidCertificateHandler> ptrHandler = new Poco::Net::AcceptCertificateHandler(false);
    Poco::Net::Context::Ptr ptrContext = new Poco::Net::Context(
        Poco::Net::Context::CLIENT_USE, "", "", "",
        Poco::Net::Context::VERIFY_RELAXED, 9, true,
        "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH"
    );
    Poco::Net::SSLManager::instance().initializeClient(0, ptrHandler, ptrContext);

    const QString& pinePath = QString(endpoint);

    Poco::URI uri(pinePath.toStdString());
    Poco::Net::HTTPSClientSession session(uri.getHost(), uri.getPort());

    std::string path(uri.getPathAndQuery());
    if (path.empty()) path = "/";

    Poco::Net::HTTPRequest req(Poco::Net::HTTPRequest::HTTP_PATCH, path, Poco::Net::HTTPMessage::HTTP_1_1);

    req.setContentType(contentType.toUtf8().toStdString());
    req.setContentLength(data.length());
    req.setCredentials("Basic", QString("cypress:H9DqvCGjJdJE").toUtf8().toBase64().toStdString());

    std::ostream &os = session.sendRequest(req);
    os.write(data.constData(), data.size());
    os.flush();

    Poco::Net::HTTPResponse res;

    std::istream &is = session.receiveResponse(res);
    std::stringstream ss;
    Poco::StreamCopier::copyStream(is, ss);
}

QByteArray FileUtils::readFileIntoByteArray(const QString &filePath)
{
    QByteArray fileContent;

    // Open the file with the provided file path
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        // Error opening file
        qWarning("Cannot open file for reading: %s", qPrintable(file.errorString()));
        return fileContent;
    }

    // Read the content of the file
    fileContent = file.readAll();

    // Close the file
    file.close();

    return fileContent;
}

bool FileUtils::doesExeExist(const QString &absolutePath)
{
    if (absolutePath.isEmpty()) return false;
    if (absolutePath.isNull())  return false;

    QFileInfo info(absolutePath);
    if (!info.isAbsolute())   return false;
    if (!info.exists()) 	  return false;
    if (!info.isExecutable()) return false;

    return true;
}

bool FileUtils::doesFileExist(const QString &absolutePath, const bool checkWritable)
{
    if (absolutePath.isEmpty()) return false;
    if (absolutePath.isNull())  return false;

    QFileInfo info(absolutePath);
    if (!info.isAbsolute()) return false;
    if (!info.isFile()) 	return false;
    if (!info.isReadable()) return false;

    if (checkWritable && !info.isWritable()) return false;

    return true;
}

bool FileUtils::doesDirExist(const QString &absolutePath, const bool checkWritable)
{
    Q_UNUSED(checkWritable)
    if (absolutePath.isEmpty()) return false;
    if (absolutePath.isNull())  return false;

    QFileInfo info(absolutePath);
    if (!info.isAbsolute()) 	return false;
    if (!info.isDir()) 			return false;
    if (!info.isWritable())		return false;

    return true;
}

bool FileUtils::backupFile(const QString& fromPath, const QString& toPath)
{
    bool fromExists = doesFileExist(fromPath, false);
    bool toExists = doesDirExist(toPath, true);

    if (!fromExists)
    {

    }

    if (!toExists)
    {

    }

    return false;
}

bool FileUtils::restoreBackup(const QString& fromPath, const QString& toPath)
{
    bool fromExists = doesFileExist(fromPath, false);
    bool toExists = doesDirExist(toPath, true);

    if (!fromExists)
    {

    }

    if (!toExists)
    {

    }

    return false;
}

bool FileUtils::removeDirectory(const QString& dirPath)
{
    bool success { false };

    QDir dir(dirPath);
    if (dir.exists()) {
        success = dir.removeRecursively(); // Removes the directory and its contents
    }

    return success;
}

bool FileUtils::createDirectory(const QString& dirPath)
{
    bool success { false };

    QDir dir;
    if (!dir.exists(dirPath))
    {
        success = dir.mkpath(dirPath);
    }

    return success;
}


QByteArray FileUtils::readFileAsBase64(QFile& file)
{
    bool opened = file.open(QIODevice::ReadOnly);
    if (!opened)
    {
        throw std::exception();
    }

    QByteArray bytes = file.readAll().toBase64();

    return bytes;
}


QString FileUtils::generateHash(const QByteArray& bytes)
{
    QByteArray hash = QCryptographicHash::hash(bytes, QCryptographicHash::Sha256);
    return hash;
}

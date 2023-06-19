#include <stdexcept>

#include <QFileInfo>
#include <QCryptographicHash>

#include "FileUtils.h"

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

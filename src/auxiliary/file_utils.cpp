#include "file_utils.h"

#include <QDir>
#include <QFileInfo>
#include <QCryptographicHash>

#include <stdexcept>

QJsonObject FileUtils::readJson(const QString &absoluteFilePath)
{
    QFile file(absoluteFilePath);
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

bool FileUtils::createFile(const QString& destinationAbsolutePath, const QByteArray& bytes) {
    QFile file(destinationAbsolutePath);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }
    file.write(bytes);
    file.close();
    return true;
}

QByteArray FileUtils::readFile(const QString& absoluteFilePath) {
    QFile file(absoluteFilePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return QByteArray();
    }
    QByteArray data = file.readAll();
    file.close();
    return data;
}

bool FileUtils::copyFile(const QString& sourceAbsolutePath, const QString& destinationAbsolutePath) {
    return QFile::copy(sourceAbsolutePath, destinationAbsolutePath);
}

bool FileUtils::moveFile(const QString& sourceAbsolutePath, const QString& destinationAbsolutePath) {
    return QFile::rename(sourceAbsolutePath, destinationAbsolutePath);
}

bool FileUtils::deleteFile(const QString& absoluteFilePath) {
    return QFile::remove(absoluteFilePath);
}

bool FileUtils::doesFileExist(const QString &absoluteFilePath) {
    return QFileInfo::exists(absoluteFilePath);
}

bool FileUtils::isFileReadable(const QString& absoluteFilePath) {
    QFileInfo fileInfo(absoluteFilePath);
    return fileInfo.isReadable();
}

bool FileUtils::isFileWritable(const QString& absoluteFilePath) {
    QFileInfo fileInfo(absoluteFilePath);
    return fileInfo.isWritable();
}

bool FileUtils::isFileExecutable(const QString& absoluteFilePath) {
    QFileInfo fileInfo(absoluteFilePath);
    return fileInfo.isExecutable();
}

bool FileUtils::createDirectory(const QString &absoluteDirectoryPath)
{
    QDir dir;
    return dir.mkpath(absoluteDirectoryPath);
}

bool FileUtils::copyDirectory(QDir source, QDir destination, bool overwrite, bool includeDirs) {
    destination.mkpath(".");

    foreach (QString file, source.entryList(QDir::Files))
    {
        QFile sourceFile(source.absoluteFilePath(file));
        QFile destinationFile(destination.absoluteFilePath(file));

        if (overwrite || !destinationFile.exists())
        {
            if (!destinationFile.remove())
                qDebug() << "could not remove destination file";

            if (!sourceFile.copy(destination.absoluteFilePath(file)))
                return false;
        }
    }

    if (!includeDirs)
        return true;

    foreach (QString directory, source.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
    {
        if (!FileUtils::copyDirectory(QDir(source.absoluteFilePath(directory)), QDir(destination.absoluteFilePath(directory)), overwrite)) {
            return false;
        }
    }

    return true;
}

bool FileUtils::moveDirectory(const QString &sourceAbsolutePath, const QString &destinationAbsolutePath)
{
    QDir dir;
    return dir.rename(sourceAbsolutePath, destinationAbsolutePath);
}

bool FileUtils::removeDirectory(const QString &absoluteDirectoryPath)
{
    QDir dir(absoluteDirectoryPath);
    return dir.removeRecursively();
}

bool FileUtils::clearDirectory(const QString &absoluteDirectoryPath)
{
    QDir dir(absoluteDirectoryPath);
    if (!dir.exists())
        return false;

    dir.setFilter(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    QFileInfoList files = dir.entryInfoList();

    for (const QFileInfo &info : files) {
        if (info.isDir()) {
            QDir subDir(info.absoluteFilePath());
            if (!subDir.removeRecursively()) {
                qDebug() << "File Utils - could not remove dir: " << info.absoluteFilePath();
                return false;
            }
        } else {
            if (!QFile::remove(info.absoluteFilePath())) {
                qDebug() << "File Utils - could not remove: " << info.absoluteFilePath();
                return false;
            }
        }
    }

    return true;
}

bool FileUtils::doesDirectoryExist(const QString &absoluteDirectoryPath)
{
    QDir dir(absoluteDirectoryPath);
    return dir.exists();
}

bool FileUtils::isDirectoryReadable(const QString &absoluteDirectoryPath)
{
    QFileInfo dirInfo(absoluteDirectoryPath);
    return dirInfo.isReadable() && dirInfo.isDir();
}

bool FileUtils::isDirectoryWritable(const QString &absoluteDirectoryPath)
{
    QFileInfo dirInfo(absoluteDirectoryPath);
    return dirInfo.isWritable() && dirInfo.isDir();
}

QString FileUtils::getSha256Hash(const QByteArray &bytes)
{
    QByteArray hash = QCryptographicHash::hash(bytes, QCryptographicHash::Sha256);
    return hash;
}

QString FileUtils::getHumanReadableFileSize(const QString& absoluteFilePath) {
    QFileInfo fileInfo(absoluteFilePath);
    if (!fileInfo.exists())
        return "";

    double size = fileInfo.size();
    QStringList units = {"B", "KB", "MB", "GB", "TB", "PB", "EB"};
    int i;

    for (i = 0; i < units.size() && size >= 1024; i++) {
        size /= 1024.0;
    }

    return QString::number(size, 'f', 2) + " " + units[i];
}

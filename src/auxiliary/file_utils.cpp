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

bool FileUtils::copyDirectory(const QString &sourceAbsolutePath, const QString &destinationAbsolutePath)
{
    QDir sourceDir(sourceAbsolutePath);
    if (!sourceDir.exists()) {
        return false;
    }

    QDir destinationDir;
    if (!destinationDir.mkpath(destinationAbsolutePath)) {
        return false;
    }

    QStringList files = sourceDir.entryList(QDir::Files);
    foreach (const QString &file, files) {
        const QString srcName = sourceAbsolutePath + QDir::separator() + file;
        const QString destName = destinationAbsolutePath + QDir::separator() + file;
        if (!QFile::copy(srcName, destName)) {
                return false;
        }
    }

    QStringList directories = sourceDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach (const QString &dir, directories) {
        const QString srcName = sourceAbsolutePath + QDir::separator() + dir;
        const QString destName = destinationAbsolutePath + QDir::separator() + dir;
        if (!copyDirectory(srcName, destName)) {
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

QString FileUtils::generateHash(const QByteArray& bytes)
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

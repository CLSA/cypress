#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QDir>

namespace FileUtils
{
    QJsonObject readJson(const QString &filePath);
    QString getSha256Hash(const QByteArray &bytes);

    bool createFile(const QString& destinationAbsolutePath, const QByteArray& bytes);
    QByteArray readFile(const QString& absolutefilePath);
    bool copyFile(const QString& sourceAbsolutePath, const QString& destinationAbsolutePath);
    bool moveFile(const QString& sourceAbsolutePath, const QString& destinationAbsolutePath);
    bool deleteFile(const QString& fileAbsolutePath);

    bool doesFileExist(const QString &absoluteFilePath);
    bool isFileReadable(const QString &absoluteFilePath);
    bool isFileWritable(const QString &absoluteFilePath);
    bool isFileExecutable(const QString &absoluteFilePath);

    bool createDirectory(const QString& absoluteDirectoryPath);
    bool copyDirectory(QDir source, QDir destination, bool overwrite = true, bool includeDirs = true);
    bool moveDirectory(const QString& sourceAbsolutePath, const QString& destinationAbsolutePath);
    bool removeDirectory(const QString& absoluteDirectoryPath);

    bool doesDirectoryExist(const QString &absoluteDirectoryPath);
    bool isDirectoryReadable(const QString &absoluteDirectoryPath);
    bool isDirectoryWritable(const QString &absoluteDirectoryPath);

    bool clearDirectory(const QString &absoluteDirectoryPath);

    QString getHumanReadableFileSize(const QString& absoluteFilePath);
    QString bytesToHumanReadable(quint64 bytes);
    QString getDirectorySize(const QString &dirPath);
}

#endif // FILEUTILS_H

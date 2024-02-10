#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QDir>

class FileUtils
{
public:
    static QJsonObject readJson(const QString &filePath);
    static QString getSha256Hash(const QByteArray &bytes);

    static bool createFile(const QString& destinationAbsolutePath, const QByteArray& bytes);
    static QByteArray readFile(const QString& absolutefilePath);
    static bool copyFile(const QString& sourceAbsolutePath, const QString& destinationAbsolutePath);
    static bool moveFile(const QString& sourceAbsolutePath, const QString& destinationAbsolutePath);
    static bool deleteFile(const QString& fileAbsolutePath);

    static bool doesFileExist(const QString &absoluteFilePath);
    static bool isFileReadable(const QString &absoluteFilePath);
    static bool isFileWritable(const QString &absoluteFilePath);
    static bool isFileExecutable(const QString &absoluteFilePath);

    static bool createDirectory(const QString& absoluteDirectoryPath);
    static bool copyDirectory(QDir source, QDir destination, bool overwrite = true, bool includeDirs = true);
    static bool moveDirectory(const QString& sourceAbsolutePath, const QString& destinationAbsolutePath);
    static bool removeDirectory(const QString& absoluteDirectoryPath);

    static bool doesDirectoryExist(const QString &absoluteDirectoryPath);
    static bool isDirectoryReadable(const QString &absoluteDirectoryPath);
    static bool isDirectoryWritable(const QString &absoluteDirectoryPath);

    static QString getHumanReadableFileSize(const QString& absoluteFilePath);
    static bool clearDirectory(const QString &absoluteDirectoryPath);

private:
    FileUtils() = delete;
};

#endif // FILEUTILS_H

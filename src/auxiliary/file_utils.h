#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>

class FileUtils
{
public:
    static QJsonObject readJsonFile(const QString &filePath);

    static QByteArray readFileIntoByteArray(const QString& filePath);

    static QByteArray readFileAsBase64(QFile& file);
    static QString generateHash(const QByteArray& bytes);

    static QString copyFile(const QString& src, const QString& dest);

    static bool doesExeExist(const QString &absolutePath);
    static bool doesFileExist(const QString &absolutePath, const bool isWritable);
    static bool doesDirExist(const QString &absolutePath, const bool isWritable);

    static bool backupFile(const QString& fromPath, const QString& toPath);
    static bool restoreBackup(const QString& fromPath, const QString& toPath);

    static bool createDirectory(const QString& dirPath);
    static bool removeDirectory(const QString& dirPath);

    static void sendHTTPSRequest(const QString &method, const QString &endpoint, const QString &contentType, const QByteArray &data);
private:
    FileUtils() = default;
};

#endif // FILEUTILS_H

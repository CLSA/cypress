#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>

class FileUtils
{
public:
    static QJsonObject readJsonFile(const QString &filePath);

    static QByteArray readFileAsBase64(QFile& file);
    static QString generateHash(const QByteArray& bytes);

    static bool doesExeExist(const QString &absolutePath);
    static bool doesFileExist(const QString &absolutePath, const bool isWritable);
    static bool doesDirExist(const QString &absolutePath, const bool isWritable);

    static bool backupFile(const QString& fromPath, const QString& toPath);
    static bool restoreBackup(const QString& fromPath, const QString& toPath);

private:
    FileUtils() = default;
};

#endif // FILEUTILS_H

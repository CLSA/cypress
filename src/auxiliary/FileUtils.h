#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>

class FileUtils
{
public:
    FileUtils();

    static QJsonObject readJsonFile(const QString &filePath);
};

#endif // FILEUTILS_H

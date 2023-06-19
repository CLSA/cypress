#ifndef FILE_COMPRESSOR_H
#define FILE_COMPRESSOR_H

#include <QObject>
#include <QByteArray>
#include <QString>

class FileCompressor : public QObject {
    Q_OBJECT

public:
    explicit FileCompressor(QObject* parent = nullptr);

    QByteArray compressFile(const QString& filePath);

private:
    QByteArray readFileContent(const QString& filePath);
};

#endif // FILE_COMPRESSOR_H

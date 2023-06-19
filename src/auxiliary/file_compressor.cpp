#include "file_compressor.h"

#include <QFile>
#include <QFileInfo>
#include <QDataStream>
#include <QBuffer>
#include <QIODevice>
#include <QTextStream>
#include <QtEndian>
#include <stdexcept>

FileCompressor::FileCompressor(QObject* parent) : QObject(parent)
{

}

QByteArray FileCompressor::compressFile(const QString& filePath) {
    const QByteArray fileContent = readFileContent(filePath);
    return qCompress(fileContent, 1);
}

QByteArray FileCompressor::readFileContent(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        throw std::runtime_error("Cannot open file: " + filePath.toStdString());
    }
    return file.readAll();
}

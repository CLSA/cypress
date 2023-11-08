#include "smbfilecopier.h"

#include <QObject>
#include <QCoreApplication>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <iostream>


void SMBFileCopier::copyFileFromSMB(const QUrl& smbUrl, const QString& localPath) {
    QNetworkRequest request(smbUrl);
    reply = manager.get(request);

    QFile localFile;
    localFile.setFileName(localPath);

    if (!localFile.open(QIODevice::WriteOnly)) {
        qDebug() << "Error: Unable to open local file for writing: " << localPath << Qt::endl;

        reply->abort();
        reply->deleteLater();

        return;
    }

    connect(reply, &QNetworkReply::readyRead, this, &SMBFileCopier::onReadyRead);

    connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::errorOccurred), this, &SMBFileCopier::onError);

    connect(reply, &QNetworkReply::finished, this, &SMBFileCopier::onFinished);
}

void SMBFileCopier::onReadyRead() {
    localFile.write(reply->readAll());
}

void SMBFileCopier::onError(QNetworkReply::NetworkError) {
    std::cerr << "Error: Unable to read from SMB file: "
              << reply->errorString().toStdString() << std::endl;

    reply->disconnect();
    reply->abort();
    reply->deleteLater();
}

void SMBFileCopier::onFinished() {
    localFile.close();
    reply->deleteLater();

    QFileInfo fileInfo(localFile);

    emit fileCopied(fileInfo);
}





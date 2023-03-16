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

    localFile.setFileName(localPath);
    if (!localFile.open(QIODevice::WriteOnly)) {
        std::cerr << "Error: Unable to open local file for writing: "
                  << localPath.toStdString() << std::endl;
        reply->abort();
        reply->deleteLater();
        return;
    }

    // Connect the readyRead signal to the onReadyRead slot
    connect(reply, &QNetworkReply::readyRead, this, &SMBFileCopier::onReadyRead);

    // Connect the error signal to the onError slot
    connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::errorOccurred), this, &SMBFileCopier::onError);

    // Connect the finished signal to the onFinished slot
    connect(reply, &QNetworkReply::finished, this, &SMBFileCopier::onFinished);
}

void SMBFileCopier::onReadyRead() {
    localFile.write(reply->readAll());
}

void SMBFileCopier::onError(QNetworkReply::NetworkError) {
    std::cerr << "Error: Unable to read from SMB file: "
              << reply->errorString().toStdString() << std::endl;

    // Disconnect all signals from this reply to prevent further processing
    reply->disconnect();
    reply->abort();
    reply->deleteLater();
}

void SMBFileCopier::onFinished() {
    localFile.close();
    reply->deleteLater();
}





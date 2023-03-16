#ifndef SMBFILECOPIER_H
#define SMBFILECOPIER_H

#include <QCoreApplication>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <iostream>

// Usage

// QUrl smbUrl("smb://username:password@hostname/shared_folder/file_to_copy.txt");
// QString localPath("local_copy.txt");
//
// SMBFileCopier fileCopier;
// fileCopier.copyFileFromSMB(smbUrl, localPath);
//
// std::cout << "File copied successfully." << std::endl;

class SMBFileCopier : public QObject {
    Q_OBJECT

public:
    // Function to copy a file from an SMB share to a local path
    void copyFileFromSMB(const QUrl& smbUrl, const QString& localPath);

private slots:
    // Slot for handling data received from the SMB file
    void onReadyRead();

    // Slot for handling errors while reading from the SMB file
    void onError(QNetworkReply::NetworkError);

    // Slot for handling the finished signal from the network reply
    void onFinished();

private:
    QNetworkAccessManager manager;
    QNetworkReply* reply = nullptr;
    QFile localFile;
};


#endif // SMBFILECOPIER_H

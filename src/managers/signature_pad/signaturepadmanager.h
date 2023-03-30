#ifndef SIGNATUREPADMANAGER_H
#define SIGNATUREPADMANAGER_H

#include "managers/ManagerBase.h"
#include "managers//signature_pad/signaturepadcommunication.h"

#ifndef SIGLIB_H
#define SIGLIB_H
#include "SigLib.h"
#endif

#include <QObject>
#include <QThread>
#include <stdio.h>
#include <tchar.h>


class SignaturePadManager : public ManagerBase
{
    Q_OBJECT
public:
    explicit SignaturePadManager(QJsonObject inputData);
    ~SignaturePadManager();

    void restart();

public slots:
    void start() override;
    void measure() override;
    void finish() override;

    void receiveSignature(const QByteArray& bytes);

signals:
    void startCapture();
    void displaySignature(const QByteArray& bytes);
    void reset();

private:
    QThread captureThread;
    QByteArray signature;
    QScopedPointer<SignaturePadCommunication> spc;

    void sendRawDataToPine(QByteArray data);

    qint32 m_answerId; 	 // from Pine
    QString m_participantId; // from Pine

    // Reset the session
    bool clearData() override;

    // Set up device
    bool setUp() override;

    // Clean up the device for next time
    bool cleanUp() override;

    // set input parameters for the test
    void setInputData(const QVariantMap& inputData) override;
};




#endif // SIGNATUREPADMANAGER_H

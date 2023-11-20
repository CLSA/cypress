#ifndef SIGNATURE_PAD_MANAGER_H
#define SIGNATURE_PAD_MANAGER_H

#include "managers/manager_base.h"
#include "signature_pad_communication.h"

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
    explicit SignaturePadManager(QSharedPointer<CypressSession> session);
    ~SignaturePadManager();

    static bool isInstalled();
    static bool isAvailable();

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

    // Reset the session
    bool clearData() override;

    // Set up device
    bool setUp() override;

    // Clean up the device for next time
    bool cleanUp() override;
};




#endif // SIGNATURE_PAD_MANAGER_H

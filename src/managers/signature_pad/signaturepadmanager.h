#ifndef SIGNATUREPADMANAGER_H
#define SIGNATUREPADMANAGER_H

#include "managers/ManagerBase.h"

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
    explicit SignaturePadManager();
    ~SignaturePadManager();

public slots:
    void start() override;
    void measure() override;
    void finish() override;

signals:
    void startCapture();

private:
    QThread captureThread;

    // Reset the session
    bool clearData() override;

    // Set up device
    bool setUp() override;

    // Clean up the device for next time
    bool cleanUp() override;

    // Send the test results to Pine
    bool sendResultsToPine(const QJsonObject &data) override;
};




#endif // SIGNATUREPADMANAGER_H

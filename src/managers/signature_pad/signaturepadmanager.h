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
};




#endif // SIGNATUREPADMANAGER_H

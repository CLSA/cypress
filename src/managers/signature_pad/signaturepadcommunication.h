#ifndef SIGNATUREPADCOMMUNICATION_H
#define SIGNATUREPADCOMMUNICATION_H

#include <stdio.h>
#include <tchar.h>

#include <QObject>
#include <QDebug>
#include <QThread>

#ifndef SIGLIB_H
#define SIGLIB_H
#include "SigLib.h"
#endif

class SignaturePadCommunication : public QObject
{
    Q_OBJECT
public:
    explicit SignaturePadCommunication();

public slots:
    void start();

signals:
    void signatureFinished(const QString& path);
    void signatureOk(const QByteArray& data);
    void signatureReset();

private:
    void toggleDisplay(bool on);

    void processInput();
    void finish();

    Signature* signature;
    TabletInterface* tablet;
    LCDInterface* lcd;

    LCDGraphicBitmap* please;
    LCDGraphicBitmap* thanks;
    LCDGraphicBitmap* sign;
    LCDGraphicBitmap* clear;
    LCDGraphicBitmap* ok;
};

#endif // SIGNATUREPADCOMMUNICATION_H

#include "signature_pad_communication.h"

#include <cstring>
#include <QFile>
#include <QException>


SignaturePadCommunication::SignaturePadCommunication()
{
    qDebug() << "Initialize SigPadCommunication";
    tablet = new TabletInterface();
    signature = new Signature(tablet);
    lcd = new LCDInterface(tablet);

    TabletParameters tp;
    tablet->GetTabletParameters(&tp);
    tp.TabletX1 = 400;
    tp.TabletX2 = 2400;
    tp.TabletY1 = 350;
    tp.TabletY2 = 950;
    tp.TabletLogicalXSize = 2000;
    tp.TabletLogicalYSize = 600;
    tablet->SetTabletParameters(&tp);

    please = new LCDGraphicBitmap(0, 0, (char*)"please.tif");
    thanks = new LCDGraphicBitmap(0, 0, (char*)"thankyou.tif");
    sign = new LCDGraphicBitmap(0, 20, (char*)"sign.tif");
    clear = new LCDGraphicBitmap(15, 4, (char*)"clear.tif");
    ok = new LCDGraphicBitmap(207, 4, (char*)"ok.tif");

    qDebug() << "SigPadComm finished init";
}

SignaturePadCommunication::~SignaturePadCommunication()
{
    qDebug() << "Destroy SigPadCommunication";
    lcd->LCDRefresh(0, 0, 0, 240, 64); // clear screen;
    SigSleep(500);
    toggleDisplay(false);
    tablet->CloseTablet();

    delete signature;
    delete tablet;
    delete lcd;

    delete please;
    delete thanks;
    delete sign;
    delete clear;
    delete ok;
}

void SignaturePadCommunication::drawThanks()
{
    lcd->LCDRefresh(0, 0, 0, 240, 64);
    lcd->LCDSendGraphic(1, 2, thanks);
    lcd->LCDSetWindow(0, 0, 0, 0);
    lcd->LCDRefresh(2, 0, 0, 240, 52); //Brings the background image already loaded into foreground
}

void SignaturePadCommunication::drawSign()
{
    clearSignature();


    lcd->LCDSendGraphic(0, 2, sign); //redraw sig box
    lcd->SetLCDCaptureMode(LCDCapInk);

    signature->KeyPadAddHotSpot(1, 1, 10, 5, 53, 17); //For CLEAR button
    signature->KeyPadAddHotSpot(0, 1, 260, 4, 25, 17); //For OK button
}

void SignaturePadCommunication::start()
{
    if (tablet->OpenTablet() == false)
    {
        qDebug() << "SignaturePadCommunication::start() - could not open tablet";
        return;
    }

    lcd->LCDRefresh(0, 0, 0, 240, 64);
    SigSleep(500);
    toggleDisplay(true);

    //load into foreground
    //for some old device, the first image won't display
    lcd->LCDSendGraphic(1, 2, thanks);
    lcd->LCDSendGraphic(0, 2, clear);
    lcd->LCDSendGraphic(0, 2, ok);
    lcd->LCDSendGraphic(0, 2, sign);

    //Create the hot spot for the OK BMP
    signature->KeyPadAddHotSpot(0, 1, 260, 4, 25, 17); //For OK button
    signature->KeyPadAddHotSpot(1, 1, 10, 5, 53, 17); //For CLEAR button
    signature->StartCapture(tablet);

    lcd->SetLCDCaptureMode(LCDCapInk);
    lcd->LCDSetWindow(0, 22, 240, 44);

    bool captured = capture();
    if (!captured)
    {
        qDebug() << "Could not capture signature";
        return;
    }

    finish();
}

void SignaturePadCommunication::finish()
{
    //toggleDisplay(false);
    drawThanks();
    SigSleep(500);
    tablet->CloseTablet();
}

void SignaturePadCommunication::clearSignature()
{
    signature->ClearHotSpotPoints();
    signature->ClearSigWindow(1);
    signature->ClearSignature();
}

bool SignaturePadCommunication::capture()
{
    bool captured = false;
    QFile file("C:/dev/clsa/output.jpg");
    bool ok = file.open(QIODevice::ReadWrite);
    if (!ok)
    {
        qDebug() << "could not open file for read/write";
    }


    while (!captured) {
        SigSleep(500);

        if (QThread::currentThread()->isInterruptionRequested())
        {
            return captured;
        }

        if (signature->KeyPadQueryHotSpot(0) > 0)
        {
            //pen down on the OK button area, count as 1 stroke
            if ( signature->NumberOfStrokes() > 1)
            {
                qDebug() << "number of strokes";
                lcd->LCDRefresh(1, 205, 4, 25, 17); // Refresh LCD at 'OK' to indicate to user that this option has been sucessfully chosen
                lcd->LCDRefresh(1, 205, 4, 25, 17); // Refresh LCD at 'OK' to indicate to user that this option has been sucessfully chosen

                signature->DrawSignature(0, 0, 2000, 600, JustifyModes::JustifyLowerLeft, 4);
                const bool ok = signature->WriteImageFile((char*) "C:\\work\\clsa\\output.jpg");
                if (!ok)
                {
                    qDebug() << "error";
                    qDebug() << "SignaturePad:: could not write captured signature to disk";
                    continue;
                }

                file.seek(0);
                emit signatureOk(file.readAll());

                captured = true;
            }
        }
        if (signature->KeyPadQueryHotSpot(1) > 0)
        {
            lcd->LCDRefresh(1, 10, 0, 53, 17); //Refresh LCD at 'CLEAR' to indicate to user that this option has been sucessfully chosen
            lcd->LCDRefresh(1, 10, 0, 53, 17); //Refresh LCD at 'CLEAR' to return CLEAR button to normal

            signature->ClearHotSpotPoints();
            signature->ClearSigWindow(1);
            signature->ClearSignature();

            drawSign();
        }
    }

    return captured;
}


void SignaturePadCommunication::toggleDisplay(bool on)
{
    char lightSwitch[2];
    char offResult[1];

    lightSwitch[0] = 0x02;
    lightSwitch[1] = 0x00;

    if (!on) {
        lightSwitch[0] = 0x03;
    }

    lcd->LCDSendCmdString(lightSwitch, 0, offResult, 100);
}

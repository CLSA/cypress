#include "signaturepadcommunication.h"


SignaturePadCommunication::SignaturePadCommunication()
{
    qDebug() << "spc started";
}

void SignaturePadCommunication::start()
{
    tablet = new TabletInterface();
    signature = new Signature(tablet);
    lcd = new LCDInterface(tablet);

    please = new LCDGraphicBitmap(0, 0, (char*)"please.tif");
    thanks = new LCDGraphicBitmap(0, 0, (char*)"thankyou.tif");
    sign = new LCDGraphicBitmap(0, 20, (char*)"sign.tif");
    clear = new LCDGraphicBitmap(15, 4, (char*)"clear.tif");
    ok = new LCDGraphicBitmap(207, 4, (char*)"ok.tif");

    qDebug() << "sig pad start";
    if (tablet->OpenTablet() == false)
    {
        qDebug() << "OpenTablet() failed\n";
        return;
    }

    qDebug() << "tp";
    TabletParameters tp;
    tablet->GetTabletParameters(&tp);
    tp.TabletX1 = 400;
    tp.TabletX2 = 2400;
    tp.TabletY1 = 350;
    tp.TabletY2 = 1050;
    tp.TabletLogicalXSize = 2000;
    tp.TabletLogicalYSize = 700;
    tablet->SetTabletParameters(&tp);
    qDebug() << "set parameters";

    lcd->LCDRefresh(0, 0, 0, 240, 64);
    SigSleep(500);
    qDebug() << "sleep";
    toggleDisplay(true);

    qDebug() << "toggle display";
    //load screen to background
    //LCD->LCDSendGraphic(1, 2, thanks);

    //load into foreground
    //for some old device, the first image won't display
    lcd->LCDSendGraphic(0, 2, clear);
    lcd->LCDSendGraphic(0, 2, ok);
    lcd->LCDSendGraphic(0, 2, sign);

    //Create the hot spot for the OK BMP
    signature->KeyPadAddHotSpot(0, 1, 250, 5, 23, 17); //For OK button
    signature->KeyPadAddHotSpot(1, 1, 10, 5, 53, 17); //For CLEAR button

    signature->StartCapture(tablet);
    lcd->SetLCDCaptureMode(LCDCapInk);
    lcd->LCDSetWindow(0, 22, 240, 44);

    processInput();
    finish();
}

void SignaturePadCommunication::finish()
{
    qDebug() << "finish";
    lcd->LCDRefresh(0, 0, 0, 240, 64);
    signature->StopCapture();
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

void SignaturePadCommunication::processInput()
{
    bool done = false;
    while (!done) {

        SigSleep(100);
        qDebug() << "1 checking hot spot...\n";
        if (signature->KeyPadQueryHotSpot(0) > 0)
        {
            qDebug() << "hot 1 got OK...\n";
            signature->ClearSigWindow(1);
            signature->ClearHotSpotPoints();
            lcd->LCDRefresh(1, 240, 5, 23, 17); //Refresh LCD at 'OK' to indicate to user that this option has been sucessfully chosen
            //pen down on the OK button area, count as 1 stroke
            if ( signature->NumberOfStrokes() > 1)
            {
                qDebug() << "OK got 2 strokes ...\n";
                int nPts = signature->TotalPoints();
                qDebug() << "got points in signing \n" << nPts;

                lcd ->LCDRefresh(0, 0, 0, 240, 64);
                lcd ->LCDSendGraphic(1, 2, thanks);
                lcd ->LCDSetWindow(0, 0, 0, 0);
                lcd ->LCDRefresh(2, 0, 0, 240, 52); //Brings the background image already loaded into foreground

                signature->ClearSigWindow(1);
                signature->KeyPadClearHotSpotList();
                signature->KeyPadAddHotSpot(2, 1, 29, 38, 44, 19); //For BACK button
                signature->KeyPadAddHotSpot(3, 1, 172, 37, 40, 19); //For EXIT button
                signature->ClearSignature();

                char* filePath = QString("C:\\Users\\Anthony\\Documents").toLocal8Bit().data();
                signature->WriteImageFile(filePath);

                //emit signatureOk();

            }
            else
            {
                qDebug() << "OK no stroke...\n";
                lcd->LCDSendGraphic(1, 2, please);
                lcd->LCDRefresh(2, 0, 0, 240, 64); //Brings the background image already loaded into foreground
                SigSleep(2750);
                lcd->LCDRefresh(0, 0, 0, 240, 64);
                //m_SigPlus.ClearTablet();
                lcd->LCDSendGraphic(0, 2, sign);
                lcd->LCDSendGraphic(0, 2, ok);
                lcd->LCDSendGraphic(0, 2, clear);
                int nPts = signature->TotalPoints();
                if (nPts > 0)
                {
                    signature->ClearSignature();
                }
                lcd->SetLCDCaptureMode(LCDCapInk);
            }
        }
        if (signature->KeyPadQueryHotSpot(1) > 0)
        {
            qDebug() << "hot 1 got Clear...\n";
            signature->ClearHotSpotPoints();
            signature->ClearSigWindow(1);

            lcd->LCDRefresh(1, 10, 0, 53, 17); //Refresh LCD at 'CLEAR' to indicate to user that this option has been sucessfully chosen
            lcd->LCDRefresh(1, 10, 0, 53, 17); //Refresh LCD at 'CLEAR' to return CLEAR button to normal
            lcd->LCDSendGraphic(0, 2, sign); //redraw sig box
            lcd->SetLCDCaptureMode(LCDCapInk);

            int nPts = signature->TotalPoints();
            if (nPts > 0)
            {
                signature->ClearSignature();
            }
            qDebug() << "Clear, now points \n" << signature->TotalPoints();
        }
        if (signature->KeyPadQueryHotSpot(2) > 0)
        {
            qDebug() << "hot 2 got Back...\n";
            signature->ClearSigWindow(1);
            signature->ClearHotSpotPoints();
            lcd->LCDRefresh(1, 29, 38, 44, 19); //Refresh (invert) LCD at 'BACK' to indicate to user that this option has been sucessfully chosen
            lcd->LCDRefresh(0, 0, 0, 240, 64);
            //LCD->ClearTablet();
            lcd->LCDSetWindow(0, 22, 240, 40);
            lcd->LCDSendGraphic(0, 2, sign);
            lcd->LCDSendGraphic(0, 2, ok);
            lcd->LCDSendGraphic(0, 2, clear);
            //m_SigPlus.ClearTablet();
            signature->KeyPadClearHotSpotList();
            signature->KeyPadAddHotSpot(0, 1, 197, 5, 23, 17); //For OK button
            signature->KeyPadAddHotSpot(1, 1, 10, 5, 53, 17); //For CLEAR button
            lcd->SetLCDCaptureMode(LCDCapInk);
            int nPts = signature->TotalPoints();
            if (nPts > 0)
            {
                signature->ClearSignature();
            }
            lcd->SetLCDCaptureMode(LCDCapInk);
        }
        if (signature->KeyPadQueryHotSpot(3) > 0)
        {
            qDebug() << "hot 3 got Exit...\n";
            signature->ClearSigWindow(1);
            signature->ClearHotSpotPoints();
            lcd->LCDRefresh(1, 182, 37, 30, 17); //Refresh (invert) LCD at 'Exit' to indicate to user that this option has been sucessfully chosen
            lcd->SetLCDCaptureMode(LCDCapDefault);
            lcd->LCDRefresh(0, 0, 0, 240, 64);
            signature->StopCapture();
            break;
        }
    }
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

#include <QDebug>

#include "DialogFactory.h"
#include "AudiometerDialog.h"
#include "BloodPressureDialog.h"
#include "BodyCompositionDialog.h"
#include "CDTTDialog.h"
#include "ChoiceReactionDialog.h"
#include "ECGDialog.h"
#include "FraxDialog.h"
#include "SpirometerDialog.h"
#include "TonometerDialog.h"
#include "WeighScaleDialog.h"
#include "GripStrengthDialog.h"
#include "RetinalCameraDialog.h"
#include "signaturepaddialog.h"

#include "DXADialog.h"


DialogFactory *DialogFactory::pInstance = Q_NULLPTR;

DialogFactory *DialogFactory::instance()
{
    if(!pInstance)
        pInstance = new DialogFactory();
    return pInstance;
}

DialogFactory::~DialogFactory()
{
    if(pInstance)
        pInstance = Q_NULLPTR;
}

DialogBase* DialogFactory::instantiate(const Constants::MeasureType &type)
{
    DialogBase* dialog = nullptr;
    switch(type)
    {
      case Constants::MeasureType::Weigh_Scale:
        dialog = new WeighScaleDialog();
        break;
      case Constants::MeasureType::Body_Composition:
        dialog = new BodyCompositionDialog();
        break;
      case Constants::MeasureType::Audiometer:
        dialog = new AudiometerDialog();
        break;
      case Constants::MeasureType::Choice_Reaction:
        dialog = new ChoiceReactionDialog();
        break;
      //case Constants::MeasureType::Thermometer:
      //  dialog = new ThermometerDialog();
      //  break;
      case Constants::MeasureType::Frax:
        dialog = new FraxDialog();
        break;
      case Constants::MeasureType::CDTT:
        dialog = new CDTTDialog();
        break;
      case Constants::MeasureType::Tonometer:
        dialog = new TonometerDialog();
        break;
      case Constants::MeasureType::Spirometer:
        dialog = new SpirometerDialog();
        break;
      case Constants::MeasureType::Blood_Pressure:
        dialog = new BloodPressureDialog();
        break;
      case Constants::MeasureType::Grip_Strength:
        dialog = new GripStrengthDialog();
        break;
      case Constants::MeasureType::Retinal_Camera:
        dialog = new RetinalCameraDialog();
        break;
      case Constants::MeasureType::ECG:
        //dialog = new EcgDialog(inputData);
        break;
      case Constants::MeasureType::CarotidIntima:
        break;
      case Constants::MeasureType::DxaWholeBody:
        dialog = new DXADialog();
        break;
      case Constants::MeasureType::Signature:
        //dialog = new SignaturePadDialog();
        break;
      case Constants::MeasureType::Unknown:
        break;
      default:
        break;
    }
    return dialog;
}

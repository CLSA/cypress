#include <QDebug>

#include "DialogFactory.h"
#include "AudiometerDialog.h"
#include "BloodPressureDialog.h"
#include "BodyCompositionDialog.h"
#include "CDTTDialog.h"
#include "ChoiceReactionDialog.h"
#include "ecgdialog.h"
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

DialogBase* DialogFactory::instantiate(const Constants::MeasureType &type, const QJsonObject& inputData)
{
    DialogBase* dialog = nullptr;
    switch(type)
    {
      case Constants::MeasureType::Audiometer:
        dialog = new AudiometerDialog(inputData);
        break;

      case Constants::MeasureType::Blood_Pressure:
        dialog = new BloodPressureDialog(inputData);
        break;

      case Constants::MeasureType::Body_Composition:
        dialog = new BodyCompositionDialog(inputData);
        break;

      case Constants::MeasureType::CDTT:
        dialog = new CDTTDialog(inputData);
        break;

      case Constants::MeasureType::Choice_Reaction:
        dialog = new ChoiceReactionDialog(inputData);
        break;

      case Constants::MeasureType::DxaWholeBody:
        dialog = new DXADialog(inputData);
        break;

      case Constants::MeasureType::ECG:
        dialog = new EcgDialog(inputData);
        break;

      case Constants::MeasureType::Frax:
        dialog = new FraxDialog(inputData);
        break;

      case Constants::MeasureType::Grip_Strength:
        dialog = new GripStrengthDialog(inputData);
        break;

      case Constants::MeasureType::Weigh_Scale:
        dialog = new WeighScaleDialog(inputData);
        break;

      //case Constants::MeasureType::Thermometer:
      //  dialog = new ThermometerDialog();
      //  break;

      case Constants::MeasureType::Tonometer:
        dialog = new TonometerDialog(inputData);
        break;

      //case Constants::MeasureType::CarotidIntima:
      //  dialog = new UltrasoundDialog(inputData);
      //  break;

      case Constants::MeasureType::Retinal_Camera:
        dialog = new RetinalCameraDialog(inputData);
        break;

      case Constants::MeasureType::Spirometer:
        dialog = new SpirometerDialog(inputData);
        break;

      case Constants::MeasureType::Signature:
        dialog = new SignaturePadDialog(inputData);
        break;

      case Constants::MeasureType::Unknown:
        break;

      default:
        break;
    }
    return dialog;
}

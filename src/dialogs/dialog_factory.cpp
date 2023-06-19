#include <QDebug>

#include "dialog_factory.h"
#include "audiometer_dialog.h"
#include "blood_pressure_dialog.h"
//#include "temperature_dialog.h"
#include "body_composition_dialog.h"
#include "cdtt_dialog.h"
#include "choice_reaction_dialog.h"
#include "ecg_dialog.h"
#include "frax_dialog.h"
#include "spirometer_dialog.h"
#include "tonometer_dialog.h"
#include "weigh_scale_dialog.h"
#include "grip_strength_dialog.h"
#include "retinal_camera_dialog.h"
#include "signature_pad_dialog.h"
#include "dxa_dialog.h"


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

DialogBase* DialogFactory::instantiate(const Constants::MeasureType &type, const QJsonObject& requestData)
{
    DialogBase* dialog = nullptr;
    switch(type)
    {
      case Constants::MeasureType::Audiometer:
        dialog = new AudiometerDialog(requestData);
        break;

      case Constants::MeasureType::Blood_Pressure:
        dialog = new BloodPressureDialog(requestData);
        break;

      case Constants::MeasureType::Body_Composition:
        dialog = new BodyCompositionDialog(requestData);
        break;

      case Constants::MeasureType::CDTT:
        dialog = new CDTTDialog(requestData);
        break;

      case Constants::MeasureType::Choice_Reaction:
        dialog = new ChoiceReactionDialog(requestData);
        break;

      case Constants::MeasureType::DxaWholeBody:
        dialog = new DXADialog(requestData);
        break;

      case Constants::MeasureType::ECG:
        dialog = new EcgDialog(requestData);
        break;

      case Constants::MeasureType::Frax:
        dialog = new FraxDialog(requestData);
        break;

      case Constants::MeasureType::Grip_Strength:
        dialog = new GripStrengthDialog(requestData);
        break;

      case Constants::MeasureType::Weigh_Scale:
        dialog = new WeighScaleDialog(requestData);
        break;

      //case Constants::MeasureType::Thermometer:
        //dialog = new ThermometerDialog(requestData);
        //break;

      case Constants::MeasureType::Tonometer:
        dialog = new TonometerDialog(requestData);
        break;

      //case Constants::MeasureType::CarotidIntima:
      //  dialog = new UltrasoundDialog(requestData);
      //  break;

      case Constants::MeasureType::Retinal_Camera:
        dialog = new RetinalCameraDialog(requestData);
        break;

      case Constants::MeasureType::Spirometer:
        dialog = new SpirometerDialog(requestData);
        break;

      case Constants::MeasureType::Signature:
        dialog = new SignaturePadDialog(requestData);
        break;

      case Constants::MeasureType::Unknown:
        break;

      default:
        break;
    }
    return dialog;
}

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
//#include "ThermometerDialog.h"

#include <QDebug>

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
    DialogBase* dialog = Q_NULLPTR;
    switch(type)
    {
      case Constants::MeasureType::typeWeigh_Scale:
        dialog = new WeighScaleDialog();
        break;
      case Constants::MeasureType::typeBody_Composition:
        dialog = new BodyCompositionDialog();
        break;
      case Constants::MeasureType::typeAudiometer:
        dialog = new AudiometerDialog();
        break;
      case Constants::MeasureType::typeChoice_Reaction:
        dialog = new ChoiceReactionDialog();
        break;
/*
      case Constants::MeasureType::typeThermometer:
        dialog = new ThermometerDialog();
        break;
*/
      case Constants::MeasureType::typeFrax:
        dialog = new FraxDialog();
        break;
      case Constants::MeasureType::typeCDTT:
        dialog = new CDTTDialog();
        break;
      case Constants::MeasureType::typeTonometer:
        dialog = new TonometerDialog();
        break;
      case Constants::MeasureType::typeSpirometer:
        dialog = new SpirometerDialog();
        break;
      case Constants::MeasureType::typeBlood_Pressure:
        dialog = new BloodPressureDialog();
        break;
      case Constants::MeasureType::typeGrip_Strength:
        dialog = new GripStrengthDialog();
        break;
      case Constants::MeasureType::typeRetinal_Camera:
        break;
      case Constants::MeasureType::typeECG:
        dialog = new ECGDialog();
        break;
      case Constants::MeasureType::typeUltrasound:
        break;
      case Constants::MeasureType::typeDexa:
        break;
      case Constants::MeasureType::typeUnknown:
        break;
      default:
        break;
    }
    return dialog;
}

DialogBase* DialogFactory::instantiate(const QString& name)
{
    return instantiate(Constants::getMeasureType(name));
}

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

#include "DXADialog.h"
#include "managers/dxa/ApSpineScanManager.h"
#include "managers/dxa/ForearmScanManager.h"
#include "managers/dxa/IVAImagingScanManager.h"
#include "managers/dxa/WholeBodyScanManager.h"
#include "managers/dxa/HipScanManager.h"


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

DialogBase* DialogFactory::instantiate(const Constants::MeasureType &type, QString uuid)
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
        dialog = new RetinalCameraDialog(uuid);
        break;
      case Constants::MeasureType::typeECG:
        dialog = new ECGDialog();
        break;

      case Constants::MeasureType::typeCarotidIntima:
        break;

      case Constants::MeasureType::typeDxaWholeBody:
        dialog = new DXADialog(new WholeBodyScanManager);
        break;
      case Constants::MeasureType::typeDxaForearmBone:
        dialog = new DXADialog(new ForearmScanManager);
        break;
      case Constants::MeasureType::typeDxaSpineBone:
        dialog = new DXADialog(new APSpineScanManager);
        break;
      case Constants::MeasureType::typeDxaLateralBone:
        dialog = new DXADialog(new IVAImagingScanManager);
        break;
      case Constants::MeasureType::typeDxaDualHip:
        dialog = new DXADialog(new HipScanManager);
        break;
      case Constants::MeasureType::typeUnknown:
        break;
      default:
        break;
    }
    return dialog;
}

DialogBase* DialogFactory::instantiate(const QString& name, QString uuid)
{
    return instantiate(Constants::getMeasureType(name), uuid);
}

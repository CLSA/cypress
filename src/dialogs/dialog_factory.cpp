#include <QDebug>
#include <QMessageBox>

#include "cypress_session.h"

#include "dialog_factory.h"
//#include "blood_pressure_dialog.h"
//#include "temperature_dialog.h"
//#include "body_composition_dialog.h"
//#include "cdtt_dialog.h"
//#include "choice_reaction_dialog.h"
//#include "ecg_dialog.h"
//#include "frax_dialog.h"
//#include "spirometer_dialog.h"
//#include "tonometer_dialog.h"
//#include "weigh_scale_dialog.h"
//#include "grip_strength_dialog.h"
//#include "retinal_camera_dialog.h"
//#include "signature_pad_dialog.h"
//#include "dxa_dialog.h"
//#include "cimt_vivid_i_dialog.h"

DialogBase* DialogFactory::instantiate(const CypressSession& session)
{
    Q_UNUSED(session)
    //switch(session.getDeviceType())
    //{
    //  case Constants::MeasureType::Audiometer:
    //    return new AudiometerDialog(nullptr, session);

    //  case Constants::MeasureType::Blood_Pressure:
    //    return new BloodPressureDialog(nullptr, session);

    //  case Constants::MeasureType::Body_Composition:
    //    return new BodyCompositionDialog(nullptr, session);

    //  case Constants::MeasureType::CDTT:
    //    return new CDTTDialog(nullptr, session);

    //  case Constants::MeasureType::Choice_Reaction:
    //    return new ChoiceReactionDialog(nullptr, session);

    //  case Constants::MeasureType::DxaWholeBody:
    //    return new DXADialog(nullptr, session);

    //  case Constants::MeasureType::ECG:
    //    return new EcgDialog(nullptr, session);

    //  case Constants::MeasureType::Frax:
    //    return new FraxDialog(nullptr, session);

    //  case Constants::MeasureType::Grip_Strength:
    //    return new GripStrengthDialog(nullptr, session);

    //  case Constants::MeasureType::Weigh_Scale:
    //    return new WeighScaleDialog(nullptr, session);

    //  //case Constants::MeasureType::Thermometer:
    //  //  return new ThermometerDialog(session);

    //  case Constants::MeasureType::Tonometer:
    //    return new TonometerDialog(nullptr, session);

    //  case Constants::MeasureType::CarotidIntima:
    //    return new CimtVividiDialog(nullptr, session);

    //  case Constants::MeasureType::Retinal_Camera:
    //    return new RetinalCameraDialog(nullptr, session);

    //  case Constants::MeasureType::Spirometer:
    //    return new SpirometerDialog(nullptr, session);

    //  case Constants::MeasureType::Signature:
    //    return new SignaturePadDialog(nullptr, session);

    //  case Constants::MeasureType::Unknown:
    //  default:
    //    break;
    //}

    QMessageBox::warning(nullptr, "Error", "Could not find a supported instrument");

    return nullptr;
}

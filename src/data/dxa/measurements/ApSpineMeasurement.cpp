#include "ApSpineMeasurement.h"
#include <QJsonObject>
#include <QStringList>

#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmdata/dcuid.h"
#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmdata/dcmetinf.h"

ApSpineMeasurement::ApSpineMeasurement()
{
    ValidDCMTag modality(DCM_Modality, "OT");
    ValidDCMTag bodyPartExamined(DCM_BodyPartExamined, "LSPINE");
    ValidDCMTag samplesPerPixel(DCM_SamplesPerPixel, "3");
    ValidDCMTag photometricInterpretation(DCM_PhotometricInterpretation, "RGB");
    ValidDCMTag imageAndFluoroscopyAreaDoseProduct(DCM_ImageAndFluoroscopyAreaDoseProduct, "");
    ValidDCMTag patientOrientation(DCM_PatientOrientation, "");
    ValidDCMTag bitsAllocated(DCM_BitsAllocated, "");
    ValidDCMTag pixelSpacing(DCM_PixelSpacing, "");
    ValidDCMTag mediaStorageSOPClassUID(DCM_MediaStorageSOPClassUID, UID_SecondaryCaptureImageStorage);

    m_metaInfoTagExists.append(mediaStorageSOPClassUID);

    m_datasetTagExistsWithValue.append(modality);
    m_datasetTagExistsWithValue.append(bodyPartExamined);
    m_datasetTagExistsWithValue.append(samplesPerPixel);
    m_datasetTagExistsWithValue.append(photometricInterpretation);

    m_datasetTagExists.append(imageAndFluoroscopyAreaDoseProduct);
    m_datasetTagExists.append(patientOrientation);
    m_datasetTagExists.append(bitsAllocated);
    m_datasetTagExists.append(pixelSpacing);
    m_datasetTagExists.append(mediaStorageSOPClassUID);
}

QString ApSpineMeasurement::toString() const
{
   return "";
};

QStringList ApSpineMeasurement::toStringList(const bool& no_keys) const
{
    Q_UNUSED(no_keys)
    return QStringList {{}};
};

bool ApSpineMeasurement::isValid() const
{
   return false;
};

QJsonObject ApSpineMeasurement::toJsonObject() const
{
   return QJsonObject();
};

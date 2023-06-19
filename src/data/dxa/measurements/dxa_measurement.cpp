#include "dxa_measurement.h"
#include <QJsonObject>

DXAMeasurement::DXAMeasurement()
{

}

QString DXAMeasurement::toString() const
{
   return "";
};

QStringList DXAMeasurement::toStringList(const bool& no_keys) const
{
    Q_UNUSED(no_keys)
    return QStringList {{}};
};


bool DXAMeasurement::isValid() const
{
    return false;
}

bool DXAMeasurement::isValidDicomFile(DcmFileFormat& dicomFileFormat) const
{
    DcmMetaInfo *metaInfo = dicomFileFormat.getMetaInfo();
    DcmDataset *dataset = dicomFileFormat.getDataset();
    OFString tagValue;

    if (metaInfo == nullptr || dataset == nullptr) return false;

    for (const ValidDCMTag& validTag : qAsConst(m_metaInfoTagExistsWithValue))
    {
        if (!metaInfo->tagExistsWithValue(validTag.key))
        {
            return false;
        }

        if (!metaInfo->findAndGetOFString(validTag.key, tagValue).good())
        {
            return false;
        }

        if (tagValue != validTag.value)
        {
            return false;
        }
    }

    for (const ValidDCMTag& validTag : qAsConst(m_metaInfoTagExists))
    {
        if (!metaInfo->tagExists(validTag.key))
        {
            return false;
        }
    }

    for (const ValidDCMTag& validTag : qAsConst(m_datasetTagExistsWithValue))
    {
        if (!dataset->tagExistsWithValue(validTag.key))
        {
            return false;
        }

        if (!dataset->findAndGetOFString(validTag.key, tagValue).good())
        {
            return false;
        }

        if (tagValue != validTag.value)
        {
            return false;
        }
    }

    for (const ValidDCMTag& validTag : qAsConst(m_datasetTagExists))
    {
        if (!metaInfo->tagExists(validTag.key))
        {
            return false;
        }
    }

    return true;
};

// String keys are converted to snake_case
//
QJsonObject DXAMeasurement::toJsonObject() const
{
   return QJsonObject();
};

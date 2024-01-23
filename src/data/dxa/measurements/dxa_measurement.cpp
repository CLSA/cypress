#include "dxa_measurement.h"

#include "dcmtk/dcmdata/dcuid.h"
#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmdata/dcmetinf.h"

#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>

DXAMeasurement::DXAMeasurement()
{

}

QJsonObject DXAMeasurement::readJsonFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        switch (file.error()) {
            case QFile::OpenError:
                throw std::runtime_error("Open error");
            case QFile::PermissionsError:
                throw std::runtime_error("Permission error");
            case QFile::ReadError:
                throw std::runtime_error("Permission error");
            default:
                throw std::runtime_error(file.errorString().toStdString());
        }
    }

    QByteArray jsonData = file.readAll();
    QJsonDocument jsonDoc(QJsonDocument::fromJson(jsonData));

    return jsonDoc.object();
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

bool DXAMeasurement::isValidDicomFile(DicomFile file) const
{
    DcmFileFormat loadedFileFormat;
    if (!loadedFileFormat.loadFile(file.absFilePath.toStdString().c_str()).good())
        return false;

    DcmMetaInfo *metaInfo = loadedFileFormat.getMetaInfo();
    DcmDataset *dataset = loadedFileFormat.getDataset();
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

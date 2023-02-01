#include "IVAImagingScanManager.h"
#include <QVariant>
#include <QString>

#include "CypressApplication.h"
#include "auxiliary/JsonSettings.h"

#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmdata/dcuid.h"
#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmdata/dcmetinf.h"

IVAImagingScanManager::IVAImagingScanManager(QWidget *parent)
    : DXAManager{parent}
{

}

QVariantMap IVAImagingScanManager::retrieveDeviceData()
{
    return QVariantMap();
}

QString IVAImagingScanManager::getName()
{
    return "IVA";
}

QString IVAImagingScanManager::getBodyPartName()
{
   return "";
}

Side IVAImagingScanManager::getSide()
{
    return m_test.side;
}

quint8 IVAImagingScanManager::getScanType()
{
    //switch(m_test.side) {
    //    case LEFT:
    //        return 61;
    //    case RIGHT:
    //        return 71;
    //}

    return -1;
}

QString IVAImagingScanManager::getRefType()
{
    return "";
}

QString IVAImagingScanManager::getRefSource()
{
   return "Hologic";
}

void IVAImagingScanManager::finish()
{
    DXAManager::finish();
    if (CypressApplication::mode == Mode::Sim)
    {
        QJsonObject results = JsonSettings::readJsonFromFile("C:/work/clsa/cypress/src/tests/fixtures/dxa/iva/output.json");
        if (results.empty()) return;

        bool ok = sendResultsToPine(results);
        if (!ok)
        {
            qDebug() << "Could not send results to Pine";
        }

        CypressApplication::status = Status::Waiting;
    }
}

bool IVAImagingScanManager::isDicomFile1(DcmFileFormat &loadedFileFormat)
{
    // _DICOM_MEASURE
    //dicom1.validator.put(Tag.Modality, new TagEntry(true,true,"OT"));
    //dicom1.validator.put(Tag.BodyPartExamined, new TagEntry(true,true,"LSPINE"));
    //dicom1.validator.put(Tag.ImageAndFluoroscopyAreaDoseProduct, new TagEntry(true,false,""));
    //dicom1.validator.put(Tag.PatientOrientation, new TagEntry(true,false,""));
    //dicom1.validator.put(Tag.BitsAllocated, new TagEntry(true,true,"16"));
    //dicom1.validator.put(Tag.PhotometricInterpretation, new TagEntry(true,true,"MONOCHROME2"));
    //dicom1.validator.put(Tag.PixelSpacing, new TagEntry(true,false,""));
    //dicom1.validator.put(Tag.SamplesPerPixel, new TagEntry(true,true,"1"));
    //dicom1.validator.put(Tag.MediaStorageSOPClassUID, new TagEntry(true,true,UID.SecondaryCaptureImageStorage));
    //dicom1.name = getResultPrefix() + "_DICOM_MEASURE";
    //apexDicomList.add(dicom1);

    bool valid = true;
    OFString value = "";
    DcmDataset* dataset = loadedFileFormat.getDataset();

    OFString modality = "OT";
    OFString bodyPartExamined = "LSPINE";
    OFString imageAndFluoroscopyAreaDoseProduct = "";
    OFString patientOrientation = "";
    OFString bitsAllocated = "16";
    OFString photometricInterpretation = "MONOCHROME2";
    OFString pixelSpacing = "";
    OFString samplesPerPixel = "1";
    OFString mediaStorageSOPClassUID = UID_SecondaryCaptureImageStorage;

    // Modality == "OT"
    valid = dataset->tagExistsWithValue(DCM_Modality);
    if (!valid)
    {
        return false;
    }

    dataset->findAndGetOFString(DCM_Modality, value);
    if (value != modality)
    {
        return false;
    }

    // BodyPartExamined == ""
    valid = dataset->tagExistsWithValue(DCM_BodyPartExamined);
    if (!valid)
    {
        return false;
    }
    dataset->findAndGetOFString(DCM_BodyPartExamined, value);
    if (value != bodyPartExamined)
    {
        return false;
    }

    valid = dataset->tagExists(DCM_ImageAndFluoroscopyAreaDoseProduct);
    if (!valid)
    {
        return false;
    }

    valid = dataset->tagExists(DCM_PatientOrientation);
    if (!valid)
    {
        return false;
    }

    valid = dataset->tagExistsWithValue(DCM_BitsAllocated);
    if (!valid)
    {
        return false;
    }
    dataset->findAndGetOFString(DCM_BitsAllocated, value);
    if (value != bitsAllocated)
    {
        return false;
    }

    valid = dataset->tagExistsWithValue(DCM_PhotometricInterpretation);
    if (!valid)
    {
        return false;
    }
    dataset->findAndGetOFString(DCM_PhotometricInterpretation, value);
    if (value != photometricInterpretation)
    {
        return false;
    }

    valid = dataset->tagExists(DCM_PixelSpacing);
    if (!valid)
    {
        return false;
    }

    valid = dataset->tagExistsWithValue(DCM_SamplesPerPixel);
    if (!valid)
    {
        return false;
    }
    dataset->findAndGetOFString(DCM_SamplesPerPixel, value);
    if (value != samplesPerPixel)
    {
        return false;
    }

    loadedFileFormat.getMetaInfo()->tagExists(DCM_MediaStorageSOPClassUID);
    if (!valid)
    {
        return false;
    }

    loadedFileFormat.getMetaInfo()->findAndGetOFString(DCM_MediaStorageSOPClassUID, value);
    if (value != mediaStorageSOPClassUID)
    {
        return false;
    }

    return valid;
}

bool IVAImagingScanManager::isDicomFile2(DcmFileFormat &loadedFileFormat)
{
    //ApexDicomData dicom2 = new ApexDicomData();
    //dicom2.validator.put(Tag.Modality, new TagEntry(true,true,"PR"));
    //dicom2.validator.put(Tag.MediaStorageSOPClassUID, new TagEntry(true,true,UID.GrayscaleSoftcopyPresentationStateStorageSOPClass));
    //dicom2.name = getResultPrefix() + "_DICOM_PR";
    //apexDicomList.add(dicom2);
    bool valid = true;
    OFString value = "";
    DcmDataset* dataset = loadedFileFormat.getDataset();

    OFString modality = "PR";
    OFString mediaStorageSOPClassUID = UID_GrayscaleSoftcopyPresentationStateStorage;

    valid = dataset->tagExistsWithValue(DCM_Modality);
    if (!valid)
    {
        return false;
    }
    dataset->findAndGetOFString(DCM_Modality, value);
    if (value != modality)
    {
        return false;
    }

    loadedFileFormat.getMetaInfo()->tagExists(DCM_MediaStorageSOPClassUID);
    if (!valid)
    {
        return false;
    }
    loadedFileFormat.getMetaInfo()->findAndGetOFString(DCM_MediaStorageSOPClassUID, value);
    if (value != mediaStorageSOPClassUID)
    {
        return false;
    }

    return valid;
}

bool IVAImagingScanManager::isDicomFile3(DcmFileFormat &loadedFileFormat)
{
 //   ApexDicomData dicom3 = new ApexDicomData();
    //dicom3.validator.put(Tag.Modality, new TagEntry(true,true,"OT"));
    //dicom3.validator.put(Tag.BitsAllocated, new TagEntry(true,true,"8"));
    //dicom3.validator.put(ApexTag.IVARESULTSFLAG.getValue(), new TagEntry(true,true,"IVA Results"));
    //dicom3.validator.put(Tag.PhotometricInterpretation, new TagEntry(true,true,"MONOCHROME2"));
    //dicom3.validator.put(Tag.SamplesPerPixel, new TagEntry(true,true,"1"));
    //dicom3.validator.put(Tag.MediaStorageSOPClassUID, new TagEntry(true,true,UID.SecondaryCaptureImageStorage));
    //dicom3.name = getResultPrefix() + "_DICOM_OT";
    //apexDicomList.add(dicom3);

    bool valid = true;
    OFString value = "";
    DcmDataset* dataset = loadedFileFormat.getDataset();

    OFString modality = "OT";
    OFString bitsAllocated = "16";
    OFString ivaResults = "IVA Results";
    OFString photometricInterpretation = "MONOCHROME2";
    OFString samplesPerPixel = "1";
    OFString mediaStorageSOPClassUID = UID_SecondaryCaptureImageStorage;

    // Modality == "OT"
    valid = dataset->tagExistsWithValue(DCM_Modality);
    if (!valid)
    {
        return false;
    }
    dataset->findAndGetOFString(DCM_Modality, value);
    if (value != modality)
    {
        return false;
    }

    valid = dataset->tagExistsWithValue(DCM_BitsAllocated);
    if (!valid)
    {
        return false;
    }
    dataset->findAndGetOFString(DCM_BitsAllocated, value);
    if (value != bitsAllocated)
    {
        return false;
    }

    DcmTagKey tag(0x0013, 0x1000); // IVA Results Custom APEX Tag
    valid = dataset->tagExistsWithValue(tag);
    if (!valid)
    {
        return false;
    }
    dataset->findAndGetOFString(DCM_PhotometricInterpretation, value);
    if (value != ivaResults)
    {
        return false;
    }

    valid = dataset->tagExistsWithValue(DCM_PhotometricInterpretation);
    if (!valid)
    {
        return false;
    }
    dataset->findAndGetOFString(DCM_PhotometricInterpretation, value);
    if (value != photometricInterpretation)
    {
        return false;
    }

    valid = dataset->tagExistsWithValue(DCM_SamplesPerPixel);
    if (!valid)
    {
        return false;
    }
    dataset->findAndGetOFString(DCM_SamplesPerPixel, value);
    if (value != samplesPerPixel)
    {
        return false;
    }

    loadedFileFormat.getMetaInfo()->tagExists(DCM_MediaStorageSOPClassUID);
    if (!valid)
    {
        return false;
    }

    loadedFileFormat.getMetaInfo()->findAndGetOFString(DCM_MediaStorageSOPClassUID, value);
    if (value != mediaStorageSOPClassUID)
    {
        return false;
    }

    return valid;
}


bool IVAImagingScanManager::validateDicomFile(DcmFileFormat &fileFormat)
{
    bool file1 = isDicomFile1(fileFormat);
    bool file2 = isDicomFile2(fileFormat);
    bool file3 = isDicomFile3(fileFormat);

    return file1 || file2 || file3;
}

QVariantMap IVAImagingScanManager::extractScanAnalysisData()
{
    // make connection to PatScan.mdb
    // query data

    return QVariantMap {{"", ""}};
}

QVariantMap IVAImagingScanManager::computeTandZScores()
{
    return QVariantMap {{"", ""}};
}


void IVAImagingScanManager::clearData()
{

}

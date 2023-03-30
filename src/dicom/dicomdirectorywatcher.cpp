#include "dicomdirectorywatcher.h"

DicomDirectoryWatcher::DicomDirectoryWatcher(const QString &directoryPath, QObject *parent)
    : QObject(parent)
{
    // Add the directory to the watcher
    m_watcher.addPath(directoryPath);

    // Connect the directoryChanged signal to the onDirectoryChanged slot
    connect(&m_watcher, &QFileSystemWatcher::directoryChanged, this, &DicomDirectoryWatcher::onDirectoryChanged);
}

void DicomDirectoryWatcher::onDirectoryChanged(const QString &path)
{
    QDir dir(path);
    QStringList filters { "*.dcm", "*.dicom"};
    const QStringList dicomFiles = dir.entryList(filters, QDir::Files | QDir::NoDotAndDotDot);

    DcmFileFormat fileFormat;

    for (const QString& filePath : dicomFiles)
    {
        QString absoluteFilePath = dir.absoluteFilePath(filePath);
        try
        {
            if (!fileFormat.loadFile(absoluteFilePath.toLocal8Bit().constData()).good())
            {
                qCritical() << "Error loading DICOM file" << absoluteFilePath;
            }

            bool valid = validateTags(fileFormat);
            if (!valid)
            {
                qWarning() << "DICOM file is not valid" << absoluteFilePath;
            }
        }
        catch (const std::exception& e)
        {
            qCritical() << "Error loading DICOM file" << absoluteFilePath << ": " << e.what();
        }
    }
}

bool DicomDirectoryWatcher::validateTags(DcmFileFormat& dicomFileFormat)
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

    for (const ValidDCMTag& validTag : qAsConst(m_metainfoTagExists))
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
}



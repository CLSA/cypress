#include "dicom_directory_watcher.h"

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
    emit dicomDirectoryChanged(path);
}


#include "dicom_directory_watcher.h"

#include <QDebug>

const int TIMER_WAIT = 2000; // ms

DicomDirectoryWatcher::DicomDirectoryWatcher(const QString &directoryPath, QObject *parent)
    : QObject(parent)
{
    // Add the directory to the watcher
    m_watcher.addPath(directoryPath);

    // Debounce timer for emitting an update 1 second after last change
    m_timer.setSingleShot(true);

    // Connect the directoryChanged signal to the onDirectoryChanged slot
    connect(&m_watcher, &QFileSystemWatcher::directoryChanged, this, &DicomDirectoryWatcher::onDirectoryChanged);
    connect(&m_timer, &QTimer::timeout, this, [=]() {
        if (QDir(directoryPath).exists())
        {
            emit dicomDirectoryChanged();
        }
    });
}

void DicomDirectoryWatcher::onDirectoryChanged(const QString &path)
{
    Q_UNUSED(path)
    m_timer.start(TIMER_WAIT);
}


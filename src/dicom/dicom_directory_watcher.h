#ifndef DICOM_DIRECTORY_WATCHER_H
#define DICOM_DIRECTORY_WATCHER_H

#include <QObject>
#include <QFileSystemWatcher>
#include <QStringList>
#include <QDir>
#include <QDebug>






class DicomDirectoryWatcher : public QObject
{
    Q_OBJECT

public:
    explicit DicomDirectoryWatcher(const QString &directoryPath, QObject *parent = nullptr);

signals:
    void dicomDirectoryChanged(const QString &path);

private slots:
    void onDirectoryChanged(const QString &path);

private:

    QFileSystemWatcher m_watcher;
};


#endif // DICOM_DIRECTORY_WATCHER_H
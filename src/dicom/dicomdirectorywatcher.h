#ifndef DICOMDIRECTORYWATCHER_H
#define DICOMDIRECTORYWATCHER_H

#include <QObject>
#include <QFileSystemWatcher>
#include <QStringList>
#include <QDir>
#include <QDebug>

#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dctk.h"
#include "dcmtk/ofstd/ofstdinc.h"


struct ValidDCMTag {
    DcmTagKey key;
    OFString value;
};

class DicomDirectoryWatcher : public QObject
{
    Q_OBJECT

public:
    explicit DicomDirectoryWatcher(const QString &directoryPath, QObject *parent = nullptr);

private slots:
    void onDirectoryChanged(const QString &path);

private:
    bool validateTags(DcmFileFormat& dicomFileFormat);

    QFileSystemWatcher m_watcher;

    QList<ValidDCMTag> m_metaInfoTagExistsWithValue;
    QList<ValidDCMTag> m_metainfoTagExists;

    QList<ValidDCMTag> m_datasetTagExistsWithValue;
    QList<ValidDCMTag> m_datasetTagExists;
};


#endif // DICOMDIRECTORYWATCHER_H

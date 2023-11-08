#ifndef DCM_RECV_H
#define DCM_RECV_H

#include "dicom_directory_watcher.h"
#include "dcmtk/dcmdata/dctk.h"

#include <QObject>
#include <QProcess>
#include <QTemporaryDir>
#include <QDir>

struct DicomFile {
    QString studyId;
    QString patientId;
    QString bodyPartExamined;
    QString modality;
    QString studyDate;

    QFileInfo fileInfo;
};

class DcmRecv : public QObject
{
    Q_OBJECT

public:
    explicit DcmRecv(
        const QString& executablePath,
        const QString& configPath,
        const QString& storageDir,
        const QString& aetitle,
        const QString& port,
        QObject* parent = nullptr
    );
    ~DcmRecv();

    bool start();
    bool stop();

    QList<DicomFile> receivedFiles;

    QString receivedFilesDir() const;

signals:
    void dicomFilesReceived();

    void logUpdate(const QString& line);

    void notRunning();
    void starting();
    void running();

    void exitNormal();
    void exitCrash();

private slots:
    void onFilesReceived();

    void onReadyReadStandardOutput();
    void onReadyReadStandardError();

    void onErrorOccurred(QProcess::ProcessError error);
    void onFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    QScopedPointer<DicomDirectoryWatcher> m_watcher;
    QProcess m_process;


    QString m_executablePath;
    QString m_configPath;
    QString m_outputDir;
    QString m_aeTitle;
    QString m_port;
};


#endif // DCM_RECV_H

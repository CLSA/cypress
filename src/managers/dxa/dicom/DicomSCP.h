#ifndef DICOMSCP_H
#define DICOMSCP_H

#include <QObject>
#include <QProcess>
#include <QThread>
#include <QFileSystemWatcher>


class DicomSCP : public QObject
{
    Q_OBJECT
public:
    explicit DicomSCP();
    ~DicomSCP();

    bool start();
    bool stop();

signals:
    void dicomFilesReceived(QStringList& dicomFilePaths);
    void logUpdate(QString line);

    void notRunning();
    void starting();
    void running();

    void exitNormal();
    void exitCrash();

private:
    void initConnections();
    void initProcess();

    QScopedPointer<QProcess> m_process;
    QScopedPointer<QFileSystemWatcher> m_fileSystemWatcher;
};

#endif // DICOMSCP_H

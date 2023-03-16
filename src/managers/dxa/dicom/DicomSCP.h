#ifndef DICOMSCP_H
#define DICOMSCP_H

#include <QObject>
#include <QProcess>
#include <QFileSystemWatcher>
#include <QSettings>

/**
 * @brief DicomSCP class is responsible for handling DICOM Storage Service Class Provider (SCP) operations.
 *
 * This class utilizes the Qt framework (version 5.15.2) and is designed to manage the SCP process,
 * watching the output directory for new DICOM files, and emitting relevant signals.
 */
class DicomSCP : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Constructs a DicomSCP object.
     */
    explicit DicomSCP();

    /**
     * @brief DicomSCP destructor.
     */
    ~DicomSCP();

    /**
     * @brief Starts the DICOM SCP process.
     * @return True if the process started successfully, false otherwise.
     */
    bool start();

    /**
     * @brief Stops the DICOM SCP process.
     * @return True if the process stopped successfully, false otherwise.
     */
    bool stop();

signals:
    void dicomFilesReceived(const QStringList& dicomFilePaths);
    void logUpdate(const QString& line);

    void notRunning();
    void starting();
    void running();

    void exitNormal();
    void exitCrash();

private:
    void initConnections();
    void initProcess();

    void onDirectoryChanged(const QString& path);
    void onFileChanged();
    void onWatcherDestroyed();
    void onProcessStateChanged();
    void onProcessStandardOutputReady();
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);

    QScopedPointer<QProcess> m_process;
    QScopedPointer<QFileSystemWatcher> m_fileSystemWatcher;
    QSettings m_settings;
};

#endif // DICOMSCP_H


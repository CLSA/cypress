#ifndef DICOMSCP_H
#define DICOMSCP_H

#include <QObject>
#include <QProcess>
#include <QThread>
#include <QFileSystemWatcher>
#include <QSettings>

class DicomSCP : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Constructor.
     */
    explicit DicomSCP();

    /**
     * @brief Destructor.
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
    /**
     * @brief Initializes the connections for the process and the file system watcher.
     */
    void initConnections();

    /**
     * @brief Initializes the DICOM SCP process.
     */
    void initProcess();

    /**
     * @brief Triggered when a monitored directory changes.
     * @param path The path of the directory that has changed.
     */
    void onDirectoryChanged(const QString& path);

    /**
     * @brief Triggered when a monitored file changes.
     */
    void onFileChanged();

    /**
     * @brief Triggered when the file system watcher is destroyed.
     */
    void onFileSystemWatcherDestroyed();

    /**
     * @brief Triggered when the process state changes.
     */
    void onStateChanged();

    /**
     * @brief Triggered when there is standard output available from the process.
     */
    void onReadyReadStandardOutput();

    /**
     * @brief Triggered when the process finishes execution.
     * @param exitCode The exit code of the process.
     * @param exitStatus The exit status of the process.
     */
    void onFinished(int exitCode, QProcess::ExitStatus exitStatus);

    QScopedPointer<QProcess> m_process;
    QScopedPointer<QFileSystemWatcher> m_fileSystemWatcher;
    QSettings m_settings;
};

#endif // DICOMSCP_H

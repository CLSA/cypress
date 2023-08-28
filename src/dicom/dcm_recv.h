#ifndef DCM_RECV_H
#define DCM_RECV_H

#include <QObject>
#include <QProcess>
#include <QTemporaryDir>
#include <QDir>

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

    QString receivedFilesDir() const;

signals:
    void dicomFilesReceived(const QStringList& dicomFilePaths);
    void logUpdate(const QString& line);

    void notRunning();
    void starting();
    void running();

    void exitNormal();
    void exitCrash();

private slots:
    void onReadyReadStandardOutput();
    void onReadyReadStandardError();
    void onErrorOccurred(QProcess::ProcessError error);
    void onFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    QProcess m_process;

    QString m_executablePath;
    QString m_configPath;
    QString m_outputDir;
    QString m_aeTitle;
    QString m_port;
};


#endif // DCM_RECV_H

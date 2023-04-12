#ifndef DCMRECV_H
#define DCMRECV_H

#include <QObject>
#include <QProcess>
#include <QTemporaryDir>
#include <QDir>

class DcmRecv : public QObject
{
    Q_OBJECT

public:
    explicit DcmRecv(QObject* parent = nullptr);
    ~DcmRecv();

    bool startDcmRecv(const QString& executablePath, const QString& aetitle, quint16 port);

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
    QTemporaryDir m_tempDir;
};


#endif // DCMRECV_H

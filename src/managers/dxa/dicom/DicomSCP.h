#ifndef DICOMSCP_H
#define DICOMSCP_H

#include <QObject>
#include <QProcess>
#include <QThread>


class DicomSCP : public QObject
{
    Q_OBJECT
public:
    explicit DicomSCP(QObject *parent = nullptr);
    ~DicomSCP();

    bool start();
    bool stop();

signals:
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
};

#endif // DICOMSCP_H

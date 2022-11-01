#ifndef DICOMSCP_H
#define DICOMSCP_H

#include <QObject>
#include <QProcess>
#include <QThread>

#include "LogWorker.h"


class DicomSCP : public QObject
{
    Q_OBJECT
public:
    explicit DicomSCP(QObject *parent = nullptr);
    ~DicomSCP();

    bool start();
    bool stop();

private:
    void initConnections();
    QScopedPointer<QProcess> m_process;
    QScopedPointer<LogWorker>  m_logWorker;

private slots:
    void readLog(QString line);

signals:
    void logUpdate(QString line);

    void notRunning();
    void starting();
    void running();

    void exitNormal();
    void exitCrash();
};

#endif // DICOMSCP_H

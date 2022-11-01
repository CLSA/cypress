#ifndef LOGWORKER_H
#define LOGWORKER_H

#include <QObject>
#include <QThread>

class LogWorker: public QThread
{
    Q_OBJECT

    void run() override;

signals:
    void updateLog(QString line);
};

#endif // LOGWORKER_H

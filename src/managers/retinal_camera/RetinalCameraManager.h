#ifndef RETINALCAMERAMANAGER_H
#define RETINALCAMERAMANAGER_H

#include <QObject>
#include <QProcess>
#include <QSettings>

#include "managers/ManagerBase.h"


class RetinalCameraManager : public ManagerBase
{
    Q_OBJECT
public:
    explicit RetinalCameraManager(QString uuid = "");

    bool isAvailable();
public slots:
    // subclasses call methods after main initialization just prior
    // to running (eg., emit dataChanged signal)
    //
    void start() override;

    // actual measure will only execute if the barcode has been
    // verified.  Subclasses must reimplement accordingly.
    //
    void measure() override;

    // subclasses call methods just prior to main close event
    //
    void finish() override;

protected:
    // Context dependent clear test data and possibly device data (eg., serial port info)
    bool clearData();

private:
    QProcess m_process;
    QString m_uuid;

    bool startRetinalCamera();
    bool cleanupDatabase();
    bool initializeDatabase();
    bool restoreDatabase();
    bool openDatabase();
    bool backupDatabase();
};

#endif // RETINALCAMERAMANAGER_H

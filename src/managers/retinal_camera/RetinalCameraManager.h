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
    explicit RetinalCameraManager(QWidget *parent = nullptr);

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
    void clearData() override;

private:
    QProcess m_process;

    bool startRetinalCamera();
    bool cleanupDatabase();
    bool initializeDatabase();
    bool restoreDatabase();
    bool openDatabase();
    bool backupDatabase();
};

#endif // RETINALCAMERAMANAGER_H

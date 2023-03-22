#ifndef RETINALCAMERAMANAGER_H
#define RETINALCAMERAMANAGER_H

#include <QObject>
#include <QProcess>
#include <QSettings>
#include <QSqlDatabase>

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

private:
    QProcess m_process;
    QString m_uuid;
    QSqlDatabase m_db;
    QString defaultPersonUUID = "11111111-2222-3333-4444-555555555555";
    QString defaultPatientUUID = "11111111-2222-3333-4444-555555555555";

    bool startRetinalCamera();
    bool cleanupDatabase();
    bool initializeDatabase();
    bool restoreDatabase();
    bool openDatabase();
    bool backupDatabase();

    // Reset the session
    bool clearData() override;

    // Set up device
    bool setUp() override;

    // Clean up the device for next time
    bool cleanUp() override;

    // set input parameters for the test
    void setInputData(const QVariantMap& inputData) override;
};

#endif // RETINALCAMERAMANAGER_H

#ifndef RETINAL_CAMERA_MANAGER_H
#define RETINAL_CAMERA_MANAGER_H

#include "managers/manager_base.h"
#include "server/sessions/retinal_camera_session.h"

#include "data/retinal_camera/retinal_camera_test.h"

#include <QObject>
#include <QProcess>
#include <QSettings>
#include <QSqlDatabase>
#include <QJsonDocument>

class RetinalCameraManager: public ManagerBase
{
    Q_OBJECT
public:
    explicit RetinalCameraManager(QSharedPointer<RetinalCameraSession> session);
    ~RetinalCameraManager();

    static bool isInstalled();

    QJsonDocument m_exam;

public slots:
    void start() override;
    void measure() override;
    void finish() override;
    void cancel();

    void addManualMeasurement() override;

protected:
    bool setUp() override;
    bool clearData() override;
    bool cleanUp() override;

private:
    RetinalCameraTest* m_test;

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

};

#endif // RETINAL_CAMERA_MANAGER_H

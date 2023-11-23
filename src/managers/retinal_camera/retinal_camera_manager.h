#ifndef RETINAL_CAMERA_MANAGER_H
#define RETINAL_CAMERA_MANAGER_H

#include "managers/manager_base.h"
#include "server/sessions/retinal_camera_session.h"

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

public slots:
    void start() override;

    void measure() override;

    void finish() override;

    void cancel();

private:
    QProcess m_process;
    QString m_uuid;

    QSqlDatabase m_db;

    QString defaultPersonUUID = "11111111-2222-3333-4444-555555555555";
    QString defaultPatientUUID = "11111111-2222-3333-4444-555555555555";

    bool setUp() override;

    bool backupDatabase();
    bool initializeDatabase();
    void configureProcess();
    bool startRetinalCamera();

    bool openDatabase();

    bool clearData() override;

    bool cleanUp() override;
    bool restoreDatabase();
    bool cleanupDatabase();

};

#endif // RETINAL_CAMERA_MANAGER_H

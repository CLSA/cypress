#ifndef RETINAL_CAMERA_MANAGER_H
#define RETINAL_CAMERA_MANAGER_H

#include "managers/manager_base.h"
#include "server/sessions/retinal_camera_session.h"
#include "config/device_config.h"

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

    static DeviceConfig config;

public slots:
    bool start() override;

    void measure() override;

    void finish() override;

    void readOutput() override;

private:
    bool setUp() override;

    bool openDatabase();
    bool initializeDatabase();
    void configureProcess();

    bool clearData() override;
    bool cleanUp() override;
    bool cleanupDatabase();

    QString defaultPersonUUID = "11111111-2222-3333-4444-555555555555";
    QString defaultPatientUUID = "11111111-2222-3333-4444-555555555555";

    QString m_runnableName {};
    QString m_runnablePath {};

    QString m_databaseName {};
    int m_databasePort = -1;
    QString m_databaseUser {};
    QString m_databasePassword {};

    QProcess m_process;
    QString m_uuid;
    QSqlDatabase m_db;

    QJsonObject getLeftEye();
    QJsonObject getRightEye();
};

#endif // RETINAL_CAMERA_MANAGER_H

#ifndef RETINAL_CAMERA_MANAGER_H
#define RETINAL_CAMERA_MANAGER_H

#include "managers/manager_base.h"

#include <QObject>
#include <QProcess>
#include <QSettings>
#include <QSqlDatabase>
#include <QJsonDocument>

class RetinalCameraManager: public ManagerBase
{
    Q_OBJECT
public:
    explicit RetinalCameraManager(const CypressSession& session);
    ~RetinalCameraManager();

    static bool isInstalled();
    static bool isAvailable();

    QJsonDocument m_exam;

public slots:
    void start() override;
    void measure() override;
    void finish() override;
    void cancel();

protected:
    bool setUp() override;
    bool clearData() override;
    bool cleanUp() override;
    void setInputData(const QVariantMap &) override;

private:
    QProcess m_process;
    QString m_uuid;
    QSqlDatabase m_db;

    QString defaultPersonUUID = "11111111-2222-3333-4444-555555555555";
    QString defaultPatientUUID = "11111111-2222-3333-4444-555555555555";

    bool simMode = false;
    bool startRetinalCamera();
    bool cleanupDatabase();
    bool initializeDatabase();
    bool restoreDatabase();
    bool openDatabase();
    bool backupDatabase();

};

#endif // RETINAL_CAMERA_MANAGER_H

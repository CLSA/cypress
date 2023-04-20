#ifndef RETINALCAMERAMANAGER_H
#define RETINALCAMERAMANAGER_H

#include <QObject>
#include <QProcess>
#include <QSettings>
#include <QSqlDatabase>
#include <QJsonDocument>

class RetinalCameraManager: public QObject
{
    Q_OBJECT
public:
    explicit RetinalCameraManager(QString uuid = "");
    ~RetinalCameraManager();

    void start();
    void measure();
    void finish();

    QJsonDocument m_exam;

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

#endif // RETINALCAMERAMANAGER_H

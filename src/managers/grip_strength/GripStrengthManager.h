#ifndef GRIPSTRENGTHMANAGER_H
#define GRIPSTRENGTHMANAGER_H

#include <QObject>
#include <QJsonDocument>
#include <QProcess>
#include <QTemporaryDir>
#include <QSqlDatabase>
#include <QSqlQuery>

class GripStrengthManager : public QObject
{
    Q_OBJECT

public:
    explicit GripStrengthManager(QObject *parent = nullptr);
    ~GripStrengthManager();

    QJsonDocument m_exam;

    void start();
    void measure();
    void finish();

signals:
    void validExam(QJsonDocument exam);

private:
    QProcess m_process;
    QSqlDatabase m_database;

    QMap<QString, QVariant> extractExam() const;

    QDir m_backupDir;
    QDir m_trackerDir;
    QString m_workingDirPath;
    QString m_executablePath;
    QString m_databaseName;


    void initialize();
    void createDatabaseBackupFolder();
    void backupDatabase();
    void openDatabase();
    void startApp();
    void restoreTrackerDatabase();
    void sendToPine();

    QString getTrackerDatabaseFolder();
};

#endif // GRIPSTRENGTHMANAGER_H

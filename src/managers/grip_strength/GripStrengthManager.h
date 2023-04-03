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

    void start();
    void measure();
    void finish();

signals:
    void validExam(QJsonDocument exam);

private:
    QString m_workingDirPath;
    QString m_executablePath;
    QString m_databaseName;

    QDir m_backupDir;
    QDir m_trackerDir;

    QProcess m_process;
    QJsonDocument m_exam;
    QSqlDatabase m_database;

    void initialize();
    void createDatabaseBackupFolder();
    void backupDatabase();
    void openDatabase();
    void startApp();
    void restoreTrackerDatabase();
    void sendToPine();

    QString getTrackerDatabaseFolder();
    QMap<QString, QVariant> extractExam() const;
};

#endif // GRIPSTRENGTHMANAGER_H

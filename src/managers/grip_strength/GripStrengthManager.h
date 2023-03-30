#ifndef GRIPSTRENGTHMANAGER_H
#define GRIPSTRENGTHMANAGER_H

#include <QObject>
#include <QTemporaryDir>
#include <QSqlDatabase>
#include <QSqlQuery>

class GripStrengthManager : public QObject
{
    Q_OBJECT

public:
    explicit GripStrengthManager(QObject *parent = nullptr);
    ~GripStrengthManager();

    void setTrackerDatabaseName(const QString &name);
    QByteArray extractTrials();

public slots:
    void initialize();
    void run();
    void shutdown();

private:
    QDir backupDir;
    QDir trackerDir;

    QString trackerDatabaseName;
    QSqlDatabase database;
    QSqlDatabase getTracker5DB();
    QMap<QString, QVariant> extractExam();

    void createDatabaseBackupFolder();
    void backupTrackerDatabase();
    void restoreTrackerDatabase();
    void sendToPine(const QMap<QString, QVariant> &values);

    QString getTrackerDatabaseFolder();
    QTemporaryDir databaseBackupFolder;
};

#endif // GRIPSTRENGTHMANAGER_H

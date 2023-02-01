#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QtSql>
#include <QJsonObject>
#include <QIODevice>

enum EyeType {
    LEFT = 1,
    RIGHT = 2
};

class DatabaseManager
{
public:
    static QJsonObject extractEyeData(const QString& patientUUID, const EyeType& side);
    static QJsonObject getDatabaseSettings();

    static bool openDatabase();
    static bool isDatabaseOpen();

    static bool initializeParticipantData(
            const QString& participantId, const QString& firstName, const QString& lastName);

    static bool cleanupDatabase();
    static bool closeDatabase();

private:
    DatabaseManager() = default;

    static QString defaultPersonUUID;
    static QString defaultPatientUUID;

    static bool wasSqlError(QSqlDatabase& db, const QSqlQuery& query, bool isTransaction);

    static QString getImageLocation(const QString& storagePathUid);
    static QByteArray getEyeImage(const QString& path);

    static bool removeImageFiles();
};

#endif // DATABASEMANAGER_H

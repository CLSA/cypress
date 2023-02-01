#include "databasemanager.h"

QString DatabaseManager::defaultPersonUUID = "11111111-2222-3333-4444-555555555555";
QString DatabaseManager::defaultPatientUUID = "11111111-2222-3333-4444-555555555555";

QJsonObject DatabaseManager::getDatabaseSettings()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");

    QString host = settings.value("retinal_camera/db_hostname", "").toString();
    QString name = settings.value("retinal_camera/db_name", "").toString();
    QString username = settings.value("retinal_camera/db_username", "").toString();
    QString password = settings.value("retinal_camera/db_password", "").toString();

    return QJsonObject {
        {"host",     host     },
        {"name",     name     },
        {"user",     username },
        {"password", password },
    };
}


bool DatabaseManager::openDatabase()
{
    if (!QSqlDatabase::isDriverAvailable("QMYSQL")) {
        qDebug() << "MySQL Driver is not available";
        return false;
    }

    QJsonObject settings = getDatabaseSettings();
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");

    db.setHostName(settings["host"].toString());
    db.setDatabaseName(settings["name"].toString());
    db.setUserName(settings["user"].toString());
    db.setPassword(settings["password"].toString());

    bool ok = db.open();
    if (!ok)
    {
        qDebug() << db.lastError().nativeErrorCode();
        return false;
    }

    return true;
}


bool DatabaseManager::isDatabaseOpen()
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isValid())
    {
       qDebug() << "Error: This workstation does not have the appropriate database driver installed.";
       return false;
    }

    if (!db.isOpen())
    {
       qDebug() << "Error: Unable to connect to the database";
       return false;
    }

    return true;
}

QJsonObject DatabaseManager::extractEyeData(const QString& patientUUID, const EyeType& side)
{
    QSqlQuery query;

    query.prepare(
        "SELECT FileName, FileExt, StoragePathUid, CreateDate "
        "FROM dbo.Media WHERE PatientUid = :patientUid AND "
        "EyeType = :eyeType AND Status = 1 AND Display = 1"
    );

    query.bindValue(":patientUid", patientUUID);
    query.bindValue(":eyeType", side);
    query.exec();

    //const QString location =
    //            query.value(0).toString() + "/"    +
    //            mediaObject["fileName"].toString() +
    //            mediaObject["fileExt"].toString();

    QJsonObject data = {
        {"EYE_PICT_VENDOR", ""},
        {"EYE_SIDE_VENDOR", 1 ? "LEFT" : "RIGHT" }
    };

    // For file in files, read bytes
    return data;
}

bool DatabaseManager::closeDatabase()
{
    return false;
}

bool DatabaseManager::initializeParticipantData(const QString& participantId, const QString& firstName, const QString& lastName)
{
    QSqlDatabase db = QSqlDatabase::database();

    bool open = isDatabaseOpen();
    if (!open) return false;

    db.transaction();
    QSqlQuery query;

    query.prepare(
        "INSERT INTO dbo.Persons (PersonUid, SurName, ForeName) VALUES (:personUid, :surName, :foreName)"
    );

    query.bindValue(":personUid", defaultPersonUUID);
    query.bindValue(":surName",  lastName);
    query.bindValue(":foreName", firstName);
    query.exec();

    if (wasSqlError(db, query, true)) return false;

    bool success = false;
    query.prepare(
        "INSERT INTO dbo.Patients (PatientUid, PatientIdentifier, PersonUid) VALUES (:patientUid, :patientIdentifier, :personUid)"
    );

    query.bindValue(":patientUid",        defaultPersonUUID);
    query.bindValue(":patientIdentifier", participantId);
    query.bindValue(":personUid",         defaultPersonUUID);
    query.exec();

    if (wasSqlError(db, query, true)) return false;

    db.commit();

    return true;
}



bool DatabaseManager::cleanupDatabase()
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isValid() || !db.isOpen())
    {
       qDebug() << "Database is not valid or open";
       return false;
    }

    db.transaction();

    QSqlQuery query;
    query.exec("DELETE FROM dbo.Exams WHERE PatientUid = "    + defaultPatientUUID);
    if (wasSqlError(db, query, true)) return false;

    query.exec("DELETE FROM dbo.Media WHERE PatientUid = "    + defaultPatientUUID);
    if (wasSqlError(db, query, true)) return false;

    query.exec("DELETE FROM dbo.Patients WHERE PatientUid = " + defaultPatientUUID);
    if (wasSqlError(db, query, true)) return false;

    query.exec("DELETE FROM dbo.Persons WHERE PatientUid = "  + defaultPatientUUID);
    if (wasSqlError(db, query, true)) return false;

    bool success = db.commit();
    if (!success)
    {
        qDebug() << "Could not complete transaction";
        return false;
    }

    return true;
}


bool DatabaseManager::removeImageFiles()
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isValid() || !db.isOpen())
    {
       qDebug() << "Database is not valid or open";
       return false;
    }

    QList<QJsonObject> filesToDelete;

    QSqlQuery query;
    query.exec("SELECT FileName, FileExt, StoragePathUid FROM dbo.Media WHERE PatientUid = " + defaultPatientUUID);

    while (query.next())
    {
        const QString storagePathUid = query.value(0).toString().trimmed();
        const QString fileName =       query.value(1).toString().trimmed();
        const QString fileExt  =       query.value(2).toString().trimmed();

        filesToDelete << QJsonObject {
            { "storagePathUid", storagePathUid },
            { "fileName", 	    fileName },
            { "fileExt",        fileExt }
        };
    }

    QList<QJsonObject>::const_iterator i;
    for (i = filesToDelete.constBegin(); i != filesToDelete.constEnd(); ++i)
    {
        const QJsonObject& mediaObject = *i;
        const QString storagePathUid = mediaObject["storagePathUid"].toString();

        query.exec("SELECT Location FROM dbo.StoragePaths WHERE StoragePathUid = " + storagePathUid);
        if (wasSqlError(db, query, false)) return false;

        const QString location =
                query.value(0).toString() + "/"    +
                mediaObject["fileName"].toString() +
                mediaObject["fileExt"].toString();

        QFile file(location);
        if (file.exists() && file.isReadable() && file.isWritable())
        {
            file.remove();
        }
    }

    return true;
}

bool DatabaseManager::wasSqlError(QSqlDatabase& db, const QSqlQuery& query, bool isTransaction = false)
{
    if (query.lastError().isValid())
    {
        qDebug() << query.lastError().nativeErrorCode();

        if (isTransaction)
        {
            bool rollbackSuccess = db.rollback();
            if (!rollbackSuccess)
            {
                qDebug() << "Could not complete transaction rollback";
            }
        }

        return true;
    }

    return false;
}

QString getFileLocation()
{
    return "";
}

#include "databasemanager.h"

QString EyeExtractorQueryUtil::getLocation(const QSqlDatabase& db, const QString& storagePathUid)
{
    QSqlQuery query(db);
    query.prepare("SELECT Location FROM dbo.StoragePaths WHERE StoragePathUid = ?");
    query.addBindValue(storagePathUid);

    if (query.exec() && query.next()) {
        return query.value(0).toString();
    } else {
        qWarning() << "Error getting location:" << query.lastError().text();
        return QString();
    }
}

QMap<QString, QVariant> EyeExtractorQueryUtil::extractData(const QSqlDatabase &db, const QString &patientUUID, int eyeTypeIntValue, const QString &sideName)
{
    qDebug() << "EyeExtractorQueryUtil::extracting data";
    QMap<QString, QVariant> data;

    QSqlQuery query(db);
    query.prepare("SELECT FileName, FileExt, StoragePathUid, CreateDate"
              " FROM dbo.Media"
              " WHERE PatientUid = :patientUUID"
              " AND EyeType = :eyeTypeIntValue"
              " AND Status = 1"
              " AND Display = 1"
              " ORDER BY CreateDate ASC");
    query.bindValue(":patientUUID", patientUUID);
    query.bindValue(":eyeTypeIntValue", eyeTypeIntValue);
    query.exec();

    if (query.last())
    {
        QString storagePathUid = query.value("StoragePathUid").toString();
        QString fileName = query.value("FileName").toString().trimmed();
        QString extension = query.value("FileExt").toString().trimmed();

        QString location = EyeExtractorQueryUtil::getLocation(db, storagePathUid);
        QByteArray imageByteArray = pathToByteArray(location, fileName, extension);

        data.insert("EYE_PICT_VENDOR", imageByteArray);
        data.insert("EYE_SIDE_VENDOR", sideName);
    }
    else
    {
        qDebug() << "Missing Picture";
    }

    return data;
}

QByteArray EyeExtractorQueryUtil::pathToByteArray(const QString &location, const QString &fileName, const QString &extension)
{
    QFile file(location + "/" + fileName + extension);
    if (file.open(QIODevice::ReadOnly))
    {
        return file.readAll();
    }
    else
    {
        throw std::runtime_error("Failed to read the file");
    }
}

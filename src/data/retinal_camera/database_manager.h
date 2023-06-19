#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <QtSql>
#include <QByteArray>
#include <QJsonObject>
#include <QIODevice>

enum EyeType {
    LEFT = 1,
    RIGHT = 2
};

class EyeExtractorQueryUtil {
public:
    static QString getLocation(const QSqlDatabase& db, const QString& storagePathUid);
    static QByteArray pathToByteArray(const QString &location, const QString &fileName, const QString &extension);
    static QMap<QString, QVariant> extractData(const QSqlDatabase &db, const QString &patientUUID, int eyeTypeIntValue, const QString &sideName);

protected:
    const QString EYE_PICT_VENDOR = "";
    const QString EYE_SIDE_VENDOR = "";
};

#endif // DATABASE_MANAGER_H

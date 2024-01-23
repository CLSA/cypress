#ifndef JSON_SETTINGS_H
#define JSON_SETTINGS_H

#include <QtCore>

namespace JsonSettings {
    QJsonObject readJsonFromFile(const QString &path);
    QJsonObject deserializeJson(const QString &jsonString);
    QString serializeJson(const QJsonObject &jsonObject);

    bool readSettingsJson(QIODevice &device, QVariantMap &map);
    bool writeSettingsJson(QIODevice &device, const QVariantMap &map);


    QVariantMap jsonObjectToVariantMap(const QJsonObject& jsonObject);

    void parseJsonObject(QJsonObject &json, QString prefix, QVariantMap &map);
    QJsonObject restoreJsonObject(QVariantMap &map);
}

#endif // JSON_SETTINGS_H

#ifndef JSON_SETTINGS_H
#define JSON_SETTINGS_H

#include <QtCore>

class JsonSettings {

public:

    static QJsonObject readJsonFromFile(const QString &path);
    static QJsonObject deserializeJson(const QString &jsonString);
    static QString serializeJson(const QJsonObject &jsonObject);

    static bool readSettingsJson(QIODevice &device, QVariantMap &map);
    static bool writeSettingsJson(QIODevice &device, const QVariantMap &map);
    static QSettings::Format JsonFormat;

private:
    static void parseJsonObject(QJsonObject &json, QString prefix, QVariantMap &map);
    static QJsonObject restoreJsonObject(QVariantMap &map);
};

#endif // JSON_SETTINGS_H

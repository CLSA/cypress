#ifndef DEVICE_CONFIG_H
#define DEVICE_CONFIG_H

#include "cypress_settings.h"

#include <QJsonObject>
#include <QString>
#include <QFileInfo>


enum Type {
    Exe,
    File,
    Dir,
    NonEmptyString,
};


class Setting {
public:
    Setting(QString key, Type type);
    Setting();

    QString iniKey {};
    Type type {};
    QString iniValue {};
};

class DeviceConfig {
public:
    DeviceConfig(QMap<QString, Setting> settings);;

    void validate();
    bool hasErrors();

    QString getSetting(const QString& key) const;
    QJsonObject getErrors() const;

private:
    bool m_hasErrors;
    QMap<QString, Setting> settings {};
    QMap<QString, QString> errors {};

    void isNonEmptyString(Setting &setting);

    void isExe(Setting &setting);
    void isFile(Setting &setting);
    void isDir(Setting &setting);
    bool isValidString(Setting &setting);

    void setError(const QString &key, const QString &error);
};

#endif // DEVICE_CONFIG_H

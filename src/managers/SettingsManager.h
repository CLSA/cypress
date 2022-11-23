#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QObject>
#include <QString>
#include <QVariant>
#include <QSettings>



class SettingsManager : public QObject
{
    Q_OBJECT
public:
    explicit SettingsManager(QObject *parent = nullptr,
                             QSettings::Format format = QSettings::IniFormat,
                             QSettings::Scope scope = QSettings::UserScope,
                             QString orgName = "CLSA",
                             QString appName = "Cypress");

    bool setDefaultSettings();

    QVariant getSetting(QString& key);
    bool setSetting(QString& key, QVariant& value);

    bool isDefined(QString& key);

    static QMap<QString, QVariant> defaultSettings;

private:
    QScopedPointer<QSettings> m_settings;

    QSettings::Format m_format;
    QSettings::Scope  m_scope;

    QString m_orgName;
    QString m_appName;
signals:


};



#endif // SETTINGSMANAGER_H

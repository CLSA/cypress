#ifndef CYPRESSSETTINGS_H
#define CYPRESSSETTINGS_H


#include <QString>
#include <QSettings>

// .ini file can be found at C:\Users\<User>\AppData\Roaming\CLSA\Cypress.ini

// Static class
class CypressSettings
{
public:
    CypressSettings() = delete;
    CypressSettings(const CypressSettings&) = delete;
    CypressSettings &operator=(const CypressSettings &) = delete;

    static bool isSimMode();
    static bool isDebugMode();
    static QString getFilePath();

    static QString getPineHost();
    static void setPineHost(const QString &host);

    static QString getPineEndpoint();
    static void setPineEndpoint(const QString &newEndpoint);

    static QString getDeviceEndpoint();

    static QString getPineCredentials();
    static QString getAnswerUrl(int answer_id);

    static QVariant readSetting(const QString& key);
    static void writeSetting(const QString& key, const QVariant& value);

private:
    static QSettings* m_settings;
};

#endif // CYPRESSSETTINGS_H

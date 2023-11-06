#ifndef CYPRESSSETTINGS_H
#define CYPRESSSETTINGS_H


#include <QString>
#include <QSettings>


// .ini file can be found at C:\Users\<User>\AppData\Roaming\CLSA\Cypress.ini

class CypressSettings
{
public:
    static CypressSettings& getInstance()
    {
        static CypressSettings instance;
        return instance;
    }

    // Singleton, delete the copy constructor and assignment operator
    CypressSettings(const CypressSettings&) = delete;
    CypressSettings &operator=(const CypressSettings &) = delete;

    QString getPineHost() const;
    void setPineHost(const QString &host);

    QString getPineEndpoint() const;
    void setPineEndpoint(const QString &newEndpoint);

    QString getDeviceEndpoint() const;

    QString getPineCredentials() const;
    QString getAnswerUrl(int answer_id) const;

    QVariant readSetting(const QString& key) const;
    void writeSetting(const QString& key, const QVariant& value);

    void setDefaultSettings();

private:
    CypressSettings();

    QSettings settings;

    QSettings& getSettings() const;
    void clearSettings();
};

#endif // CYPRESSSETTINGS_H

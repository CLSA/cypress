#include "cypress_settings.h"

#include <QSettings>
#include <QException>
#include <QString>
#include <QDebug>

// .ini file can be found at C:\Users\<User>\AppData\Roaming\CLSA\Cypress.ini
QSettings* CypressSettings::m_settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");

bool CypressSettings::isDebugMode()
{
    bool isDebugMode = m_settings->value("debug_mode", 1).toInt();

    return isDebugMode;
}

QString CypressSettings::getFilePath()
{
    return m_settings->fileName();
}

bool CypressSettings::isSimMode()
{
    bool isSimMode = m_settings->value("sim_mode", 0).toInt();

    return isSimMode;
}

QString CypressSettings::getPineHost()
{
    QString host = m_settings->value("pine/host", "127.0.0.1:9000").toString();
    return host;
}

void CypressSettings::setPineHost(const QString &host)
{
    if (host.isEmpty() || host.isNull())
        return;

    m_settings->setValue("pine/host", host);
}

QString CypressSettings::getPineEndpoint()
{
    QString endpoint{m_settings->value("pine/endpoint", "/qa/pine/api/answer/").toString()};
    return endpoint;
}

void CypressSettings::setPineEndpoint(const QString &newEndpoint)
{
    if (newEndpoint.isEmpty() || newEndpoint.isNull())
        return;

    m_settings->setValue("pine/endpoint", newEndpoint);
}

QString CypressSettings::getPineCredentials()
{
    QString credentials { m_settings->value("pine/credentials", "cypress:H9DqvCGjJdJE").toString() };

    return credentials;
}

QString CypressSettings::getAnswerUrl(int answer_id)
{
    QString host = getPineHost();
    QString endpoint = getPineEndpoint();

    QString answerUrl{QString("%1%2%3").arg(host, endpoint, QString::number(answer_id))};

    return answerUrl;
}

QString CypressSettings::getDeviceEndpoint()
{
    return m_settings->value("pine/device_endpoint", "/host/pine/api/answer_device/uuid=").toString();
}

QVariant CypressSettings::readSetting(const QString& key)
{
    return m_settings->value(key);
}

void CypressSettings::writeSetting(const QString& key, const QVariant& value)
{
    m_settings->setValue(key, value);
}

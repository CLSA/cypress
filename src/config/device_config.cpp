#include "device_config.h"



DeviceConfig::DeviceConfig(QMap<QString, Setting> settings): settings { settings }
{
    validate();
}

Setting::Setting(QString key, Type type): iniKey { key }, type { type }
{
    iniValue = CypressSettings::readSetting(iniKey).toString();
}

Setting::Setting() {}

void DeviceConfig::validate() {
    for (auto it = settings.begin(); it != settings.end(); ++it)
    {
        Setting& setting = *it;
        switch (setting.type) {
            case NonEmptyString:
                isNonEmptyString(setting);
                break;
            case Exe:
                isExe(setting);
                break;
            case File:
                isFile(setting);
                break;
            case Dir:
                isDir(setting);
                break;
            default:
                break;
        }
    }
}

QString DeviceConfig::getSetting(const QString &key) const
{
    return settings[key].iniValue;
}

bool DeviceConfig::hasErrors()
{
    return m_hasErrors;
}

QJsonObject DeviceConfig::getErrors() const
{
    QJsonObject json {};

    for (auto it = errors.begin(); it != errors.end(); ++it)
    {
        json[it.key()] = it.value();
    }

    return json;
}

void DeviceConfig::isNonEmptyString(Setting &setting)
{
    if (!isValidString(setting))
    {
        setError(setting.iniKey, "is not defined");
        return;
    }
}

void DeviceConfig::isExe(Setting &setting)
{
    if (!isValidString(setting))
    {
        setError(setting.iniKey, "is not defined");
        return;
    }

    const QFileInfo file(setting.iniValue);
    if (!file.exists())
    {
        setError(setting.iniKey, "does not exist");
        return;
    }

    if (!file.isExecutable())
    {
        setError(setting.iniKey, "is not executable");
        return;
    }
}

void DeviceConfig::isFile(Setting &setting)
{
    if (!isValidString(setting))
    {
        setError(setting.iniKey, "is not defined");
        return;
    }

    const QFileInfo file(setting.iniValue);

    if (file.isDir())
    {
        setError(setting.iniKey, "is a directory");
        return;
    }

    if (!file.exists())
    {
        setError(setting.iniKey, "does not exist");
        return;
    }

    if (!file.isReadable())
    {
        setError(setting.iniKey, "is not readable");
        return;
    }

    if (!file.isWritable())
    {
        setError(setting.iniKey, "is not writable");
        return;
    }
}

void DeviceConfig::isDir(Setting &setting) {
    if (!isValidString(setting))
    {
        setError(setting.iniKey, "is not defined");
        return;
    }

    const QFileInfo file(setting.iniValue);

    if (!file.isDir())
    {
        setError(setting.iniKey, "is not a directory");
        return;
    }

    if (!file.isReadable())
    {
        setError(setting.iniKey, "is not readable");
        return;
    }

    if (!file.isWritable())
    {
        setError(setting.iniKey, "is not writable");
        return;
    }
}

bool DeviceConfig::isValidString(Setting &setting)
{
    return !setting.iniValue.isEmpty() && !setting.iniValue.isNull();
}

void DeviceConfig::setError(const QString &key, const QString &error)
{
    m_hasErrors = true;
    errors[key] = error;
}



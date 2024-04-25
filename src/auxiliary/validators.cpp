#include "validators.h"


bool Validators::isValidString(const QJsonObject& input, const QString &key)
{
    if (!input.contains(key)) return false;

    QJsonValue val { input.value(key) };

    if (!val.isString())      return false;
    if (val.toString() == "") return false;

    return true;
}

bool Validators::isValidDouble(const QJsonObject& input, const QString &key)
{
    if (!input.contains(key)) return false;

    QJsonValue val { input.value(key) };
    if (val.isDouble()) return true;

    return false;
}

bool Validators::isValidInteger(const QJsonObject& input, const QString &key)
{
    if (!input.contains(key)) return false;

    QJsonValue val { input.value(key) };
    if (val.isDouble()) return true;

    return false;
}

bool Validators::isValidBool(const QJsonObject& input, const QString &key)
{
    if (!input.contains(key)) return false;

    QJsonValue val { input.value(key) };

    if (val.isBool()) return true;

    return false;
}

bool Validators::isValidDate(const QJsonObject& input, const QString &key, const QString &dateFormat)
{
    if (!input.contains(key)) return false;

    QJsonValue val { input.value(key) };

    if (!val.isString()) return false;

    QDate date = QDate::fromString(val.toString(), dateFormat);
    qDebug() << date;

    return date.isValid();
}

bool Validators::isValidDateTime(const QJsonObject& input, const QString &key, const QString &dateFormat)
{
    if (!input.contains(key)) return false;

    QJsonValue val { input.value(key) };

    if (!val.isString()) return false;

    QDate date = QDate::fromString(val.toString(), dateFormat);

    return date.isValid();
}

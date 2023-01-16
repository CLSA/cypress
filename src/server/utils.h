#ifndef UTILS_H
#define UTILS_H

#include <QJsonObject>

class Utils
{
public:
    Utils();
    static QJsonObject httpPost(const QJsonObject& data);
};

#endif // UTILS_H

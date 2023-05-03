#include "ManagerBase.h"
#include <QStandardItemModel>
#include "server/utils.h"
#include <QException>
#include <QDebug>

ManagerBase::ManagerBase()
{
}

ManagerBase::~ManagerBase()
{
}

bool ManagerBase::sendResultsToPine(const QJsonObject& results)
{
    try {
        QJsonObject response = Utils::httpPost(results);
        return true;
    }
    catch (QException &e)
    {
       qDebug() << e.what();
    }

    return false;
}

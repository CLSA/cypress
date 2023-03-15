#ifndef DIALOGFACTORY_H
#define DIALOGFACTORY_H

#include "../auxiliary/Constants.h"
#include <QJsonObject>

QT_FORWARD_DECLARE_CLASS(DialogBase)

class DialogFactory
{
public:
    static DialogFactory *instance();
    ~DialogFactory();

    DialogBase* instantiate(const Constants::MeasureType&, QJsonObject inputData);
    DialogBase* instantiate(const QString&, QJsonObject inputData);

private:
    DialogFactory() = default;
    static DialogFactory *pInstance;
};

#endif // DIALOGFACTORY_H

#ifndef DIALOG_FACTORY_H
#define DIALOG_FACTORY_H

#include "../auxiliary/Constants.h"
#include <QJsonObject>

QT_FORWARD_DECLARE_CLASS(DialogBase)

class DialogFactory
{
public:
    static DialogFactory *instance();
    ~DialogFactory();

    DialogBase* instantiate(const Constants::MeasureType& type, const QJsonObject& requestData);

private:
    DialogFactory() = default;
    static DialogFactory *pInstance;
};

#endif // DIALOG_FACTORY_H

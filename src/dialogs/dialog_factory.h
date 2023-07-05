#ifndef DIALOG_FACTORY_H
#define DIALOG_FACTORY_H

#include "cypress_session.h"

#include <QJsonObject>

QT_FORWARD_DECLARE_CLASS(DialogBase)

class DialogFactory
{
public:
    static DialogBase* instantiate(const CypressSession& session);

private:
    DialogFactory() = default;
};

#endif // DIALOG_FACTORY_H

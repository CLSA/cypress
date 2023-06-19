#ifndef VIVIDIDIALOG_H
#define VIVIDIDIALOG_H

#include "dialog_base.h"
#include <QObject>

class VividiDialog : public DialogBase
{
    Q_OBJECT
public:
    VividiDialog(QJsonObject inputData);

    void userClose() override;
    void closeEvent(QCloseEvent* event) override;
};

#endif // VIVIDIDIALOG_H

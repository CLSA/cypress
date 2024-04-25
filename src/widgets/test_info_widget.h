#ifndef TEST_INFO_WIDGET_H
#define TEST_INFO_WIDGET_H

#include "cypress_session.h"
#include <QWidget>

namespace Ui {
class TestInfoWidget;
}

class TestInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TestInfoWidget(QWidget *parent = nullptr);
    ~TestInfoWidget();

    void setSessionInformation(const CypressSession& session, const QString& deviceName = "");
    void setDeviceStatus(QString status);
    void setStatus(const QString status);

private:
    Ui::TestInfoWidget *ui;
};

#endif // TEST_INFO_WIDGET_H

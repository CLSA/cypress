#ifndef TEST_INFO_WIDGET_H
#define TEST_INFO_WIDGET_H

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

private:
    Ui::TestInfoWidget *ui;
};

#endif // TEST_INFO_WIDGET_H

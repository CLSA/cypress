#ifndef TESTINFOWIDGET_H
#define TESTINFOWIDGET_H

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

#endif // TESTINFOWIDGET_H

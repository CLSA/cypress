#include "testinfowidget.h"
#include "ui_testinfowidget.h"

TestInfoWidget::TestInfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestInfoWidget)
{
    ui->setupUi(this);
}

TestInfoWidget::~TestInfoWidget()
{
    delete ui;
}

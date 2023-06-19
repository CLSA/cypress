#include "test_info_widget.h"
#include "ui_test_info_widget.h"

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

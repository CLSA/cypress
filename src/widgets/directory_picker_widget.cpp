#include "directory_picker_widget.h"
#include "ui_directory_picker_widget.h"

DirectoryPickerWidget::DirectoryPickerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DirectoryPickerWidget)
{
    ui->setupUi(this);
}

DirectoryPickerWidget::~DirectoryPickerWidget()
{
    delete ui;
}

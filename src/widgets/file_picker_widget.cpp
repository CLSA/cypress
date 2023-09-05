#include "file_picker_widget.h"
#include "ui_file_picker_widget.h"

#include <QFileDialog>


FilePickerWidget::FilePickerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FilePickerWidget)
{
    ui->setupUi(this);
}

FilePickerWidget::~FilePickerWidget()
{
    delete ui;
}


QString FilePickerWidget::pickFile()
{
    QString selectedFilePath = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("All Files (*)"));
    if (!selectedFilePath.isEmpty())
    {
        ui->filePathValue->setText(selectedFilePath);
    }
}

void FilePickerWidget::on_pickFileButton_clicked()
{
    QString filePath = pickFile();

    ui->filePathValue->setText(filePath);
}


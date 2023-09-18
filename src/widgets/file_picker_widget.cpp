#include "file_picker_widget.h"
#include "ui_file_picker_widget.h"

#include "auxiliary/file_utils.h"

#include <QFileDialog>
#include <QMessageBox>



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

void FilePickerWidget::setPath(QString path)
{
    ui->filePath->blockSignals(true);
    ui->filePath->setText(path);
    ui->filePath->blockSignals(false);
}

void FilePickerWidget::setTitle(QString title)
{
    ui->groupBox->setTitle(title);
}

bool FilePickerWidget::validate(const QString& path)
{
    if (m_isFile)
    {
        return FileUtils::doesFileExist(path, true);
    }

    else if (m_isExe)
    {
        return FileUtils::doesExeExist(path);
    }

    else if (m_isDir)
    {
        return FileUtils::doesDirExist(path, true);
    }

    return true;
}

QString FilePickerWidget::pickFile()
{
    QString path;

    if (m_isDir)
    {
        path = QFileDialog::getExistingDirectory(this, tr("Choose Directory"), "");
    }

    else
    {
        path = QFileDialog::getOpenFileName(this, tr("Choose File"), "", tr("All Files (*)"));
    }

    return path;
}

bool FilePickerWidget::getIsFile()
{
    return m_isFile;
}

bool FilePickerWidget::getIsDir()
{
    return m_isDir;
}

bool FilePickerWidget::getIsExe()
{
    return m_isExe;
}

void FilePickerWidget::setIsFile(bool value)
{
    if (m_isFile != value)
    {
        m_isFile = value;
        emit isFileChanged();
    }
}

void FilePickerWidget::setIsDir(bool value)
{
    if (m_isDir != value)
    {
        m_isDir = value;
        emit isDirChanged();
    }
}

void FilePickerWidget::setIsExe(bool value)
{
    if (m_isExe != value)
    {
        m_isExe = value;
        emit isExeChanged();
    }
}

void FilePickerWidget::on_pathButton_clicked()
{
    QString filePath = pickFile();

    if (filePath.isEmpty() || filePath.isNull()) {
        return;
    }

    if (!validate(filePath))
    {
        QMessageBox::warning(this, "Invalid path", "This path is not valid for this field.");
    }

    m_path = filePath;

    setPath(filePath);

    emit pathChanged(m_path);
}


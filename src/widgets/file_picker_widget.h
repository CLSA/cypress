#ifndef FILE_PICKER_WIDGET_H
#define FILE_PICKER_WIDGET_H

#include <QWidget>

namespace Ui {
class FilePickerWidget;
}

class FilePickerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FilePickerWidget(QWidget *parent = nullptr);
    ~FilePickerWidget();

    QString pickFile();

private:
    Ui::FilePickerWidget *ui;
};

#endif // FILE_PICKER_WIDGET_H

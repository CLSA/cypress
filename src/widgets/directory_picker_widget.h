#ifndef DIRECTORY_PICKER_WIDGET_H
#define DIRECTORY_PICKER_WIDGET_H

#include <QWidget>

namespace Ui {
class DirectoryPickerWidget;
}

class DirectoryPickerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DirectoryPickerWidget(QWidget *parent = nullptr);
    ~DirectoryPickerWidget();

private:
    Ui::DirectoryPickerWidget *ui;
};

#endif // DIRECTORY_PICKER_WIDGET_H

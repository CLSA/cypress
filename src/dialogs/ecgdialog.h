#ifndef ECGDIALOG_H
#define ECGDIALOG_H

#include <QDialog>

namespace Ui {
class EcgDialog;
}

class EcgDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EcgDialog(QWidget *parent = nullptr);
    ~EcgDialog();

private:
    Ui::EcgDialog *ui;
};

#endif // ECGDIALOG_H

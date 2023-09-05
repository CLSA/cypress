#ifndef CIMT_VIVID_IQ_DIALOG_H
#define CIMT_VIVID_IQ_DIALOG_H

#include <QWidget>

namespace Ui {
class CimtVividIQDialog;
}

class CimtVividIQDialog : public QWidget
{
    Q_OBJECT

public:
    explicit CimtVividIQDialog(QWidget *parent = nullptr);
    ~CimtVividIQDialog();

private:
    Ui::CimtVividIQDialog *ui;
};

#endif // CIMT_VIVID_IQ_DIALOG_H

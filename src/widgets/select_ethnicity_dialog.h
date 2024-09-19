#ifndef SELECT_ETHNICITY_DIALOG_H
#define SELECT_ETHNICITY_DIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QPushButton>
#include <QObject>
#include <QVBoxLayout>

class SelectEthnicityDialog : public QDialog
{
    Q_OBJECT

public:
    SelectEthnicityDialog(QWidget *parent = nullptr);
    QString getSelectedValue() const;

private:
    QComboBox *comboBox;
    QPushButton *okButton;
    QPushButton *cancelButton;
};

#endif // SELECT_ETHNICITY_DIALOG_H

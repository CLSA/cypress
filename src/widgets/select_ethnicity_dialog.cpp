#include "select_ethnicity_dialog.h"

#include <QSpacerItem>
SelectEthnicityDialog::SelectEthnicityDialog(QWidget *parent) : QDialog(parent),
    comboBox(new QComboBox(this)), okButton(new QPushButton("OK", this)), cancelButton(new QPushButton("Cancel", this))
{
    comboBox->addItem("Caucasian", QVariant("Caucasian"));
    comboBox->addItem("Hispanic", QVariant("Hispanic"));
    comboBox->addItem("Asian", QVariant("Asian"));
    comboBox->addItem("African", QVariant("African"));
    comboBox->addItem("Other", QVariant("Other_Ethnic"));
    comboBox->addItem("North-East Asian", QVariant("NE_Asian"));
    comboBox->addItem("South-East Asian", QVariant("SE_Asian"));

    comboBox->setCurrentIndex(0);

    QVBoxLayout *vLayout = new QVBoxLayout(this);
    vLayout->addWidget(comboBox);

    QHBoxLayout *hLayout = new QHBoxLayout(this);
    hLayout->addSpacerItem(new QSpacerItem(100, 0));
    hLayout->addWidget(okButton);
    hLayout->addWidget(cancelButton);

    vLayout->addLayout(hLayout);

    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
    setFixedSize(300, 100);

    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    setLayout(vLayout);
}


QString SelectEthnicityDialog::getSelectedValue() const
{
    int currentIndex = comboBox->currentIndex();
    if (currentIndex != -1) {
        return comboBox->itemData(currentIndex).toString();
    }
    return QString("");
}

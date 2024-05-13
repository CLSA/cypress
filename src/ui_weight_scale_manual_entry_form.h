/********************************************************************************
** Form generated from reading UI file 'weight_scale_manual_entry_form.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WEIGHT_SCALE_MANUAL_ENTRY_FORM_H
#define UI_WEIGHT_SCALE_MANUAL_ENTRY_FORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_WeightScaleManualEntryForm
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *weightLabel;
    QDoubleSpinBox *weightValue;
    QHBoxLayout *buttons;
    QSpacerItem *horizontalSpacer;
    QPushButton *okButton;
    QPushButton *cancelButton;
    QSpacerItem *verticalSpacer;

    void setupUi(QDialog *WeightScaleManualEntryForm)
    {
        if (WeightScaleManualEntryForm->objectName().isEmpty())
            WeightScaleManualEntryForm->setObjectName(QString::fromUtf8("WeightScaleManualEntryForm"));
        WeightScaleManualEntryForm->resize(400, 84);
        verticalLayout_2 = new QVBoxLayout(WeightScaleManualEntryForm);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        weightLabel = new QLabel(WeightScaleManualEntryForm);
        weightLabel->setObjectName(QString::fromUtf8("weightLabel"));

        horizontalLayout_2->addWidget(weightLabel);

        weightValue = new QDoubleSpinBox(WeightScaleManualEntryForm);
        weightValue->setObjectName(QString::fromUtf8("weightValue"));
        weightValue->setDecimals(1);
        weightValue->setMaximum(500.000000000000000);

        horizontalLayout_2->addWidget(weightValue);


        verticalLayout->addLayout(horizontalLayout_2);

        buttons = new QHBoxLayout();
        buttons->setObjectName(QString::fromUtf8("buttons"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        buttons->addItem(horizontalSpacer);

        okButton = new QPushButton(WeightScaleManualEntryForm);
        okButton->setObjectName(QString::fromUtf8("okButton"));

        buttons->addWidget(okButton);

        cancelButton = new QPushButton(WeightScaleManualEntryForm);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        buttons->addWidget(cancelButton);


        verticalLayout->addLayout(buttons);


        verticalLayout_2->addLayout(verticalLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        retranslateUi(WeightScaleManualEntryForm);

        QMetaObject::connectSlotsByName(WeightScaleManualEntryForm);
    } // setupUi

    void retranslateUi(QDialog *WeightScaleManualEntryForm)
    {
        WeightScaleManualEntryForm->setWindowTitle(QCoreApplication::translate("WeightScaleManualEntryForm", "Dialog", nullptr));
        weightLabel->setText(QCoreApplication::translate("WeightScaleManualEntryForm", "Weight (kg)", nullptr));
        okButton->setText(QCoreApplication::translate("WeightScaleManualEntryForm", "Ok", nullptr));
        cancelButton->setText(QCoreApplication::translate("WeightScaleManualEntryForm", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WeightScaleManualEntryForm: public Ui_WeightScaleManualEntryForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WEIGHT_SCALE_MANUAL_ENTRY_FORM_H

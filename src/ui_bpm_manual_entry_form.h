/********************************************************************************
** Form generated from reading UI file 'bpm_manual_entry_form.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BPM_MANUAL_ENTRY_FORM_H
#define UI_BPM_MANUAL_ENTRY_FORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_BpmManualEntryForm
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *systolicLayout;
    QLabel *systolicLabel;
    QSpinBox *systolicValue;
    QHBoxLayout *diastolicLayout;
    QLabel *diastolicLabel;
    QSpinBox *diastolicValue;
    QHBoxLayout *pulseLayout;
    QLabel *pulseLabel;
    QSpinBox *pulseValue;
    QHBoxLayout *buttons;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *okButton;
    QPushButton *cancelButton;
    QSpacerItem *verticalSpacer;

    void setupUi(QDialog *BpmManualEntryForm)
    {
        if (BpmManualEntryForm->objectName().isEmpty())
            BpmManualEntryForm->setObjectName(QString::fromUtf8("BpmManualEntryForm"));
        BpmManualEntryForm->resize(501, 148);
        verticalLayout_2 = new QVBoxLayout(BpmManualEntryForm);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        systolicLayout = new QHBoxLayout();
        systolicLayout->setObjectName(QString::fromUtf8("systolicLayout"));
        systolicLabel = new QLabel(BpmManualEntryForm);
        systolicLabel->setObjectName(QString::fromUtf8("systolicLabel"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(systolicLabel->sizePolicy().hasHeightForWidth());
        systolicLabel->setSizePolicy(sizePolicy);

        systolicLayout->addWidget(systolicLabel);

        systolicValue = new QSpinBox(BpmManualEntryForm);
        systolicValue->setObjectName(QString::fromUtf8("systolicValue"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(systolicValue->sizePolicy().hasHeightForWidth());
        systolicValue->setSizePolicy(sizePolicy1);
        systolicValue->setMaximum(200);

        systolicLayout->addWidget(systolicValue);


        verticalLayout->addLayout(systolicLayout);

        diastolicLayout = new QHBoxLayout();
        diastolicLayout->setObjectName(QString::fromUtf8("diastolicLayout"));
        diastolicLabel = new QLabel(BpmManualEntryForm);
        diastolicLabel->setObjectName(QString::fromUtf8("diastolicLabel"));
        sizePolicy.setHeightForWidth(diastolicLabel->sizePolicy().hasHeightForWidth());
        diastolicLabel->setSizePolicy(sizePolicy);

        diastolicLayout->addWidget(diastolicLabel);

        diastolicValue = new QSpinBox(BpmManualEntryForm);
        diastolicValue->setObjectName(QString::fromUtf8("diastolicValue"));
        sizePolicy1.setHeightForWidth(diastolicValue->sizePolicy().hasHeightForWidth());
        diastolicValue->setSizePolicy(sizePolicy1);
        diastolicValue->setMaximum(200);

        diastolicLayout->addWidget(diastolicValue);


        verticalLayout->addLayout(diastolicLayout);

        pulseLayout = new QHBoxLayout();
        pulseLayout->setObjectName(QString::fromUtf8("pulseLayout"));
        pulseLabel = new QLabel(BpmManualEntryForm);
        pulseLabel->setObjectName(QString::fromUtf8("pulseLabel"));
        sizePolicy.setHeightForWidth(pulseLabel->sizePolicy().hasHeightForWidth());
        pulseLabel->setSizePolicy(sizePolicy);

        pulseLayout->addWidget(pulseLabel);

        pulseValue = new QSpinBox(BpmManualEntryForm);
        pulseValue->setObjectName(QString::fromUtf8("pulseValue"));
        sizePolicy1.setHeightForWidth(pulseValue->sizePolicy().hasHeightForWidth());
        pulseValue->setSizePolicy(sizePolicy1);
        pulseValue->setMaximum(500);

        pulseLayout->addWidget(pulseValue);


        verticalLayout->addLayout(pulseLayout);


        verticalLayout_2->addLayout(verticalLayout);

        buttons = new QHBoxLayout();
        buttons->setObjectName(QString::fromUtf8("buttons"));
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        buttons->addItem(horizontalSpacer_4);

        okButton = new QPushButton(BpmManualEntryForm);
        okButton->setObjectName(QString::fromUtf8("okButton"));

        buttons->addWidget(okButton);

        cancelButton = new QPushButton(BpmManualEntryForm);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        buttons->addWidget(cancelButton);


        verticalLayout_2->addLayout(buttons);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        retranslateUi(BpmManualEntryForm);

        QMetaObject::connectSlotsByName(BpmManualEntryForm);
    } // setupUi

    void retranslateUi(QDialog *BpmManualEntryForm)
    {
        BpmManualEntryForm->setWindowTitle(QCoreApplication::translate("BpmManualEntryForm", "Dialog", nullptr));
        systolicLabel->setText(QCoreApplication::translate("BpmManualEntryForm", "Systolic (mmHg)", nullptr));
        diastolicLabel->setText(QCoreApplication::translate("BpmManualEntryForm", "Diastolic (mmHg)", nullptr));
        pulseLabel->setText(QCoreApplication::translate("BpmManualEntryForm", "Pulse (bpm)", nullptr));
        okButton->setText(QCoreApplication::translate("BpmManualEntryForm", "Ok", nullptr));
        cancelButton->setText(QCoreApplication::translate("BpmManualEntryForm", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BpmManualEntryForm: public Ui_BpmManualEntryForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BPM_MANUAL_ENTRY_FORM_H

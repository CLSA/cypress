/********************************************************************************
** Form generated from reading UI file 'grip_strength_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRIP_STRENGTH_DIALOG_H
#define UI_GRIP_STRENGTH_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

#include "widgets/test_info_widget.h"

QT_BEGIN_NAMESPACE

class Ui_GripStrengthDialog
{
public:
    QVBoxLayout *verticalLayout;
    TestInfoWidget *widget;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *completeButton;
    QPushButton *measureButton;
    QPushButton *submitButton;
    QGroupBox *groupBox_2;

    void setupUi(QDialog *GripStrengthDialog)
    {
        if (GripStrengthDialog->objectName().isEmpty())
            GripStrengthDialog->setObjectName(QString::fromUtf8("GripStrengthDialog"));
        GripStrengthDialog->resize(448, 363);
        GripStrengthDialog->setSizeGripEnabled(true);
        verticalLayout = new QVBoxLayout(GripStrengthDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        widget = new TestInfoWidget(GripStrengthDialog);
        widget->setObjectName(QString::fromUtf8("widget"));

        verticalLayout->addWidget(widget);

        groupBox = new QGroupBox(GripStrengthDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        completeButton = new QPushButton(groupBox);
        completeButton->setObjectName(QString::fromUtf8("completeButton"));

        horizontalLayout->addWidget(completeButton);

        measureButton = new QPushButton(groupBox);
        measureButton->setObjectName(QString::fromUtf8("measureButton"));
        measureButton->setEnabled(false);

        horizontalLayout->addWidget(measureButton);

        submitButton = new QPushButton(groupBox);
        submitButton->setObjectName(QString::fromUtf8("submitButton"));
        submitButton->setEnabled(false);

        horizontalLayout->addWidget(submitButton);


        verticalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(GripStrengthDialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy1);

        verticalLayout->addWidget(groupBox_2);


        retranslateUi(GripStrengthDialog);

        QMetaObject::connectSlotsByName(GripStrengthDialog);
    } // setupUi

    void retranslateUi(QDialog *GripStrengthDialog)
    {
        GripStrengthDialog->setWindowTitle(QCoreApplication::translate("GripStrengthDialog", "Grip Strength", nullptr));
        groupBox->setTitle(QString());
        completeButton->setText(QCoreApplication::translate("GripStrengthDialog", "Complete", nullptr));
        measureButton->setText(QCoreApplication::translate("GripStrengthDialog", "Measure", nullptr));
        submitButton->setText(QCoreApplication::translate("GripStrengthDialog", "Submit", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("GripStrengthDialog", "Measurement", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GripStrengthDialog: public Ui_GripStrengthDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRIP_STRENGTH_DIALOG_H

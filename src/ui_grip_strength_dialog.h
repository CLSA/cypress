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
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include "widgets/test_info_widget.h"

QT_BEGIN_NAMESPACE

class Ui_GripStrengthDialog
{
public:
    QVBoxLayout *verticalLayout;
    TestInfoWidget *testInfoWidget;
    QSpacerItem *verticalSpacer;

    void setupUi(QDialog *GripStrengthDialog)
    {
        if (GripStrengthDialog->objectName().isEmpty())
            GripStrengthDialog->setObjectName(QString::fromUtf8("GripStrengthDialog"));
        GripStrengthDialog->resize(448, 50);
        GripStrengthDialog->setSizeGripEnabled(true);
        verticalLayout = new QVBoxLayout(GripStrengthDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        testInfoWidget = new TestInfoWidget(GripStrengthDialog);
        testInfoWidget->setObjectName(QString::fromUtf8("testInfoWidget"));

        verticalLayout->addWidget(testInfoWidget);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(GripStrengthDialog);

        QMetaObject::connectSlotsByName(GripStrengthDialog);
    } // setupUi

    void retranslateUi(QDialog *GripStrengthDialog)
    {
        GripStrengthDialog->setWindowTitle(QCoreApplication::translate("GripStrengthDialog", "Grip Strength", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GripStrengthDialog: public Ui_GripStrengthDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRIP_STRENGTH_DIALOG_H

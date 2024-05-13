/********************************************************************************
** Form generated from reading UI file 'frax_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FRAX_DIALOG_H
#define UI_FRAX_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include "widgets/test_info_widget.h"

QT_BEGIN_NAMESPACE

class Ui_FraxDialog
{
public:
    QVBoxLayout *verticalLayout;
    TestInfoWidget *testInfoWidget;
    QSpacerItem *verticalSpacer;

    void setupUi(QDialog *FraxDialog)
    {
        if (FraxDialog->objectName().isEmpty())
            FraxDialog->setObjectName(QString::fromUtf8("FraxDialog"));
        FraxDialog->resize(400, 34);
        verticalLayout = new QVBoxLayout(FraxDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        testInfoWidget = new TestInfoWidget(FraxDialog);
        testInfoWidget->setObjectName(QString::fromUtf8("testInfoWidget"));

        verticalLayout->addWidget(testInfoWidget);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(FraxDialog);

        QMetaObject::connectSlotsByName(FraxDialog);
    } // setupUi

    void retranslateUi(QDialog *FraxDialog)
    {
        FraxDialog->setWindowTitle(QCoreApplication::translate("FraxDialog", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FraxDialog: public Ui_FraxDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRAX_DIALOG_H

/********************************************************************************
** Form generated from reading UI file 'tonometer_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TONOMETER_DIALOG_H
#define UI_TONOMETER_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include "widgets/test_info_widget.h"

QT_BEGIN_NAMESPACE

class Ui_TonometerDialog
{
public:
    QVBoxLayout *verticalLayout;
    TestInfoWidget *testInfoWidget;
    QSpacerItem *verticalSpacer;

    void setupUi(QDialog *TonometerDialog)
    {
        if (TonometerDialog->objectName().isEmpty())
            TonometerDialog->setObjectName(QString::fromUtf8("TonometerDialog"));
        TonometerDialog->resize(400, 50);
        verticalLayout = new QVBoxLayout(TonometerDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        testInfoWidget = new TestInfoWidget(TonometerDialog);
        testInfoWidget->setObjectName(QString::fromUtf8("testInfoWidget"));

        verticalLayout->addWidget(testInfoWidget);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(TonometerDialog);

        QMetaObject::connectSlotsByName(TonometerDialog);
    } // setupUi

    void retranslateUi(QDialog *TonometerDialog)
    {
        TonometerDialog->setWindowTitle(QCoreApplication::translate("TonometerDialog", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TonometerDialog: public Ui_TonometerDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TONOMETER_DIALOG_H

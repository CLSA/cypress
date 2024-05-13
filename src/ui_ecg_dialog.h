/********************************************************************************
** Form generated from reading UI file 'ecg_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ECG_DIALOG_H
#define UI_ECG_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include "widgets/test_info_widget.h"

QT_BEGIN_NAMESPACE

class Ui_EcgDialog
{
public:
    QVBoxLayout *verticalLayout;
    TestInfoWidget *testInfoWidget;
    QSpacerItem *verticalSpacer;

    void setupUi(QDialog *EcgDialog)
    {
        if (EcgDialog->objectName().isEmpty())
            EcgDialog->setObjectName(QString::fromUtf8("EcgDialog"));
        EcgDialog->resize(400, 300);
        verticalLayout = new QVBoxLayout(EcgDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        testInfoWidget = new TestInfoWidget(EcgDialog);
        testInfoWidget->setObjectName(QString::fromUtf8("testInfoWidget"));

        verticalLayout->addWidget(testInfoWidget);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(EcgDialog);

        QMetaObject::connectSlotsByName(EcgDialog);
    } // setupUi

    void retranslateUi(QDialog *EcgDialog)
    {
        EcgDialog->setWindowTitle(QCoreApplication::translate("EcgDialog", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EcgDialog: public Ui_EcgDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ECG_DIALOG_H

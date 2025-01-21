/********************************************************************************
** Form generated from reading UI file 'general_proxy_form_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GENERAL_PROXY_FORM_DIALOG_H
#define UI_GENERAL_PROXY_FORM_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "widgets/test_info_widget.h"

QT_BEGIN_NAMESPACE

class Ui_GeneralProxyFormDialog
{
public:
    QVBoxLayout *verticalLayout;
    TestInfoWidget *testInfoWidget;

    void setupUi(QWidget *GeneralProxyFormDialog)
    {
        if (GeneralProxyFormDialog->objectName().isEmpty())
            GeneralProxyFormDialog->setObjectName(QString::fromUtf8("GeneralProxyFormDialog"));
        GeneralProxyFormDialog->resize(400, 16);
        verticalLayout = new QVBoxLayout(GeneralProxyFormDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        testInfoWidget = new TestInfoWidget(GeneralProxyFormDialog);
        testInfoWidget->setObjectName(QString::fromUtf8("testInfoWidget"));

        verticalLayout->addWidget(testInfoWidget);


        retranslateUi(GeneralProxyFormDialog);

        QMetaObject::connectSlotsByName(GeneralProxyFormDialog);
    } // setupUi

    void retranslateUi(QWidget *GeneralProxyFormDialog)
    {
        GeneralProxyFormDialog->setWindowTitle(QCoreApplication::translate("GeneralProxyFormDialog", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GeneralProxyFormDialog: public Ui_GeneralProxyFormDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GENERAL_PROXY_FORM_DIALOG_H

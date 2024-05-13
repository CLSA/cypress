/********************************************************************************
** Form generated from reading UI file 'apex_transfer_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_APEX_TRANSFER_WIDGET_H
#define UI_APEX_TRANSFER_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ApexTransferWidget
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *transferStatus;

    void setupUi(QWidget *ApexTransferWidget)
    {
        if (ApexTransferWidget->objectName().isEmpty())
            ApexTransferWidget->setObjectName(QString::fromUtf8("ApexTransferWidget"));
        ApexTransferWidget->resize(400, 60);
        verticalLayout = new QVBoxLayout(ApexTransferWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        transferStatus = new QLabel(ApexTransferWidget);
        transferStatus->setObjectName(QString::fromUtf8("transferStatus"));

        verticalLayout->addWidget(transferStatus);


        retranslateUi(ApexTransferWidget);

        QMetaObject::connectSlotsByName(ApexTransferWidget);
    } // setupUi

    void retranslateUi(QWidget *ApexTransferWidget)
    {
        ApexTransferWidget->setWindowTitle(QCoreApplication::translate("ApexTransferWidget", "Form", nullptr));
        transferStatus->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ApexTransferWidget: public Ui_ApexTransferWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_APEX_TRANSFER_WIDGET_H

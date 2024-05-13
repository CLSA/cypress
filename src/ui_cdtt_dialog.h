/********************************************************************************
** Form generated from reading UI file 'cdtt_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CDTT_DIALOG_H
#define UI_CDTT_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include "widgets/test_info_widget.h"

QT_BEGIN_NAMESPACE

class Ui_CDTTDialog
{
public:
    QVBoxLayout *verticalLayout;
    TestInfoWidget *testInfoWidget;
    QSpacerItem *verticalSpacer;

    void setupUi(QDialog *CDTTDialog)
    {
        if (CDTTDialog->objectName().isEmpty())
            CDTTDialog->setObjectName(QString::fromUtf8("CDTTDialog"));
        CDTTDialog->resize(387, 34);
        verticalLayout = new QVBoxLayout(CDTTDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        testInfoWidget = new TestInfoWidget(CDTTDialog);
        testInfoWidget->setObjectName(QString::fromUtf8("testInfoWidget"));

        verticalLayout->addWidget(testInfoWidget);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(CDTTDialog);

        QMetaObject::connectSlotsByName(CDTTDialog);
    } // setupUi

    void retranslateUi(QDialog *CDTTDialog)
    {
        CDTTDialog->setWindowTitle(QCoreApplication::translate("CDTTDialog", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CDTTDialog: public Ui_CDTTDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CDTT_DIALOG_H

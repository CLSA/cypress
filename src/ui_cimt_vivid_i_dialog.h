/********************************************************************************
** Form generated from reading UI file 'cimt_vivid_i_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CIMT_VIVID_I_DIALOG_H
#define UI_CIMT_VIVID_I_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "widgets/measurement_table.h"
#include "widgets/test_info_widget.h"

QT_BEGIN_NAMESPACE

class Ui_CimtVividiDialog
{
public:
    QVBoxLayout *verticalLayout;
    TestInfoWidget *testInfoWidget;
    MeasurementTable *measurementTable;

    void setupUi(QWidget *CimtVividiDialog)
    {
        if (CimtVividiDialog->objectName().isEmpty())
            CimtVividiDialog->setObjectName(QString::fromUtf8("CimtVividiDialog"));
        CimtVividiDialog->resize(483, 44);
        verticalLayout = new QVBoxLayout(CimtVividiDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        testInfoWidget = new TestInfoWidget(CimtVividiDialog);
        testInfoWidget->setObjectName(QString::fromUtf8("testInfoWidget"));

        verticalLayout->addWidget(testInfoWidget);

        measurementTable = new MeasurementTable(CimtVividiDialog);
        measurementTable->setObjectName(QString::fromUtf8("measurementTable"));

        verticalLayout->addWidget(measurementTable);


        retranslateUi(CimtVividiDialog);

        QMetaObject::connectSlotsByName(CimtVividiDialog);
    } // setupUi

    void retranslateUi(QWidget *CimtVividiDialog)
    {
        CimtVividiDialog->setWindowTitle(QCoreApplication::translate("CimtVividiDialog", "GE VividI", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CimtVividiDialog: public Ui_CimtVividiDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CIMT_VIVID_I_DIALOG_H

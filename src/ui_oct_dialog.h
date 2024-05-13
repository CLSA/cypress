/********************************************************************************
** Form generated from reading UI file 'oct_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OCT_DIALOG_H
#define UI_OCT_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QVBoxLayout>
#include "widgets/measurement_table.h"
#include "widgets/test_info_widget.h"

QT_BEGIN_NAMESPACE

class Ui_OCTDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    TestInfoWidget *testInfoWidget;
    MeasurementTable *measurementTable;

    void setupUi(QDialog *OCTDialog)
    {
        if (OCTDialog->objectName().isEmpty())
            OCTDialog->setObjectName(QString::fromUtf8("OCTDialog"));
        OCTDialog->resize(400, 46);
        verticalLayout_2 = new QVBoxLayout(OCTDialog);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        testInfoWidget = new TestInfoWidget(OCTDialog);
        testInfoWidget->setObjectName(QString::fromUtf8("testInfoWidget"));

        verticalLayout->addWidget(testInfoWidget);

        measurementTable = new MeasurementTable(OCTDialog);
        measurementTable->setObjectName(QString::fromUtf8("measurementTable"));

        verticalLayout->addWidget(measurementTable);


        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(OCTDialog);

        QMetaObject::connectSlotsByName(OCTDialog);
    } // setupUi

    void retranslateUi(QDialog *OCTDialog)
    {
        OCTDialog->setWindowTitle(QCoreApplication::translate("OCTDialog", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class OCTDialog: public Ui_OCTDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OCT_DIALOG_H

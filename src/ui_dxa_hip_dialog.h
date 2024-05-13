/********************************************************************************
** Form generated from reading UI file 'dxa_hip_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DXA_HIP_DIALOG_H
#define UI_DXA_HIP_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "widgets/measurement_table.h"
#include "widgets/test_info_widget.h"

QT_BEGIN_NAMESPACE

class Ui_DxaHipDialog
{
public:
    QVBoxLayout *verticalLayout;
    TestInfoWidget *testInfoWidget;
    MeasurementTable *measurementTable;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *DxaHipDialog)
    {
        if (DxaHipDialog->objectName().isEmpty())
            DxaHipDialog->setObjectName(QString::fromUtf8("DxaHipDialog"));
        DxaHipDialog->resize(718, 333);
        verticalLayout = new QVBoxLayout(DxaHipDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        testInfoWidget = new TestInfoWidget(DxaHipDialog);
        testInfoWidget->setObjectName(QString::fromUtf8("testInfoWidget"));

        verticalLayout->addWidget(testInfoWidget);

        measurementTable = new MeasurementTable(DxaHipDialog);
        measurementTable->setObjectName(QString::fromUtf8("measurementTable"));

        verticalLayout->addWidget(measurementTable);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(DxaHipDialog);

        QMetaObject::connectSlotsByName(DxaHipDialog);
    } // setupUi

    void retranslateUi(QWidget *DxaHipDialog)
    {
        DxaHipDialog->setWindowTitle(QCoreApplication::translate("DxaHipDialog", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DxaHipDialog: public Ui_DxaHipDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DXA_HIP_DIALOG_H

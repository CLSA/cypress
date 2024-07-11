/********************************************************************************
** Form generated from reading UI file 'dxa_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DXA_DIALOG_H
#define UI_DXA_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "widgets/measurement_table.h"
#include "widgets/test_info_widget.h"

QT_BEGIN_NAMESPACE

class Ui_DXADialog
{
public:
    QVBoxLayout *verticalLayout_5;
    TestInfoWidget *testInfoWidget;
    MeasurementTable *measurementTable;

    void setupUi(QWidget *DXADialog)
    {
        if (DXADialog->objectName().isEmpty())
            DXADialog->setObjectName(QString::fromUtf8("DXADialog"));
        DXADialog->resize(719, 44);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(DXADialog->sizePolicy().hasHeightForWidth());
        DXADialog->setSizePolicy(sizePolicy);
        verticalLayout_5 = new QVBoxLayout(DXADialog);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        testInfoWidget = new TestInfoWidget(DXADialog);
        testInfoWidget->setObjectName(QString::fromUtf8("testInfoWidget"));

        verticalLayout_5->addWidget(testInfoWidget);

        measurementTable = new MeasurementTable(DXADialog);
        measurementTable->setObjectName(QString::fromUtf8("measurementTable"));

        verticalLayout_5->addWidget(measurementTable);


        retranslateUi(DXADialog);

        QMetaObject::connectSlotsByName(DXADialog);
    } // setupUi

    void retranslateUi(QWidget *DXADialog)
    {
        DXADialog->setWindowTitle(QCoreApplication::translate("DXADialog", "CLSA Cypress", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DXADialog: public Ui_DXADialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DXA_DIALOG_H

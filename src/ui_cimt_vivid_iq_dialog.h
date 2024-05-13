/********************************************************************************
** Form generated from reading UI file 'cimt_vivid_iq_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CIMT_VIVID_IQ_DIALOG_H
#define UI_CIMT_VIVID_IQ_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "widgets/dicom_widget.h"
#include "widgets/measurement_table.h"
#include "widgets/test_info_widget.h"

QT_BEGIN_NAMESPACE

class Ui_CimtVividIQDialog
{
public:
    QVBoxLayout *verticalLayout;
    TestInfoWidget *widget;
    DicomWidget *widget_2;
    MeasurementTable *widget_3;

    void setupUi(QWidget *CimtVividIQDialog)
    {
        if (CimtVividIQDialog->objectName().isEmpty())
            CimtVividIQDialog->setObjectName(QString::fromUtf8("CimtVividIQDialog"));
        CimtVividIQDialog->resize(400, 300);
        verticalLayout = new QVBoxLayout(CimtVividIQDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        widget = new TestInfoWidget(CimtVividIQDialog);
        widget->setObjectName(QString::fromUtf8("widget"));

        verticalLayout->addWidget(widget);

        widget_2 = new DicomWidget(CimtVividIQDialog);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));

        verticalLayout->addWidget(widget_2);

        widget_3 = new MeasurementTable(CimtVividIQDialog);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));

        verticalLayout->addWidget(widget_3);


        retranslateUi(CimtVividIQDialog);

        QMetaObject::connectSlotsByName(CimtVividIQDialog);
    } // setupUi

    void retranslateUi(QWidget *CimtVividIQDialog)
    {
        CimtVividIQDialog->setWindowTitle(QCoreApplication::translate("CimtVividIQDialog", "GE Vivid IQ", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CimtVividIQDialog: public Ui_CimtVividIQDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CIMT_VIVID_IQ_DIALOG_H

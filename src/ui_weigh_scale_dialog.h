/********************************************************************************
** Form generated from reading UI file 'weigh_scale_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WEIGH_SCALE_DIALOG_H
#define UI_WEIGH_SCALE_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include "widgets/measurement_table.h"
#include "widgets/serial_port_widget.h"
#include "widgets/test_info_widget.h"

QT_BEGIN_NAMESPACE

class Ui_WeighScaleDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    TestInfoWidget *testInfoWidget;
    SerialPortWidget *serialPortPickerWidget;
    QGroupBox *optionsWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *zeroDevicePushButton;
    MeasurementTable *measurementTable;
    QSpacerItem *verticalSpacer;

    void setupUi(QDialog *WeighScaleDialog)
    {
        if (WeighScaleDialog->objectName().isEmpty())
            WeighScaleDialog->setObjectName(QString::fromUtf8("WeighScaleDialog"));
        WeighScaleDialog->resize(654, 139);
        verticalLayout_2 = new QVBoxLayout(WeighScaleDialog);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        testInfoWidget = new TestInfoWidget(WeighScaleDialog);
        testInfoWidget->setObjectName(QString::fromUtf8("testInfoWidget"));

        verticalLayout_2->addWidget(testInfoWidget);

        serialPortPickerWidget = new SerialPortWidget(WeighScaleDialog);
        serialPortPickerWidget->setObjectName(QString::fromUtf8("serialPortPickerWidget"));

        verticalLayout_2->addWidget(serialPortPickerWidget);

        optionsWidget = new QGroupBox(WeighScaleDialog);
        optionsWidget->setObjectName(QString::fromUtf8("optionsWidget"));
        horizontalLayout = new QHBoxLayout(optionsWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        zeroDevicePushButton = new QPushButton(optionsWidget);
        zeroDevicePushButton->setObjectName(QString::fromUtf8("zeroDevicePushButton"));
        zeroDevicePushButton->setEnabled(false);

        horizontalLayout->addWidget(zeroDevicePushButton);


        verticalLayout_2->addWidget(optionsWidget);

        measurementTable = new MeasurementTable(WeighScaleDialog);
        measurementTable->setObjectName(QString::fromUtf8("measurementTable"));

        verticalLayout_2->addWidget(measurementTable);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        retranslateUi(WeighScaleDialog);

        QMetaObject::connectSlotsByName(WeighScaleDialog);
    } // setupUi

    void retranslateUi(QDialog *WeighScaleDialog)
    {
        WeighScaleDialog->setWindowTitle(QCoreApplication::translate("WeighScaleDialog", "Weight", nullptr));
        optionsWidget->setTitle(QCoreApplication::translate("WeighScaleDialog", "Device Controls", nullptr));
        zeroDevicePushButton->setText(QCoreApplication::translate("WeighScaleDialog", "Zero Device", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WeighScaleDialog: public Ui_WeighScaleDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WEIGH_SCALE_DIALOG_H

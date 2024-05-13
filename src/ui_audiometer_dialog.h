/********************************************************************************
** Form generated from reading UI file 'audiometer_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AUDIOMETER_DIALOG_H
#define UI_AUDIOMETER_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include "widgets/measurement_table.h"
#include "widgets/serial_port_widget.h"
#include "widgets/test_info_widget.h"

QT_BEGIN_NAMESPACE

class Ui_AudiometerDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    TestInfoWidget *testInfoWidget;
    SerialPortWidget *serialPortPickerWidget;
    MeasurementTable *measurementTable;
    QSpacerItem *verticalSpacer;

    void setupUi(QDialog *AudiometerDialog)
    {
        if (AudiometerDialog->objectName().isEmpty())
            AudiometerDialog->setObjectName(QString::fromUtf8("AudiometerDialog"));
        AudiometerDialog->resize(561, 106);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(AudiometerDialog->sizePolicy().hasHeightForWidth());
        AudiometerDialog->setSizePolicy(sizePolicy);
        verticalLayout_2 = new QVBoxLayout(AudiometerDialog);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        testInfoWidget = new TestInfoWidget(AudiometerDialog);
        testInfoWidget->setObjectName(QString::fromUtf8("testInfoWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(testInfoWidget->sizePolicy().hasHeightForWidth());
        testInfoWidget->setSizePolicy(sizePolicy1);

        verticalLayout_2->addWidget(testInfoWidget);

        serialPortPickerWidget = new SerialPortWidget(AudiometerDialog);
        serialPortPickerWidget->setObjectName(QString::fromUtf8("serialPortPickerWidget"));

        verticalLayout_2->addWidget(serialPortPickerWidget);

        measurementTable = new MeasurementTable(AudiometerDialog);
        measurementTable->setObjectName(QString::fromUtf8("measurementTable"));

        verticalLayout_2->addWidget(measurementTable);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        retranslateUi(AudiometerDialog);

        QMetaObject::connectSlotsByName(AudiometerDialog);
    } // setupUi

    void retranslateUi(QDialog *AudiometerDialog)
    {
        AudiometerDialog->setWindowTitle(QCoreApplication::translate("AudiometerDialog", "Hearing Measurement", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AudiometerDialog: public Ui_AudiometerDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AUDIOMETER_DIALOG_H

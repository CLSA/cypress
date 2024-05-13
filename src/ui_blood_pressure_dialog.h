/********************************************************************************
** Form generated from reading UI file 'blood_pressure_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BLOOD_PRESSURE_DIALOG_H
#define UI_BLOOD_PRESSURE_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include "widgets/measurement_table.h"
#include "widgets/test_info_widget.h"

QT_BEGIN_NAMESPACE

class Ui_BloodPressureDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    TestInfoWidget *testInfoWidget;
    QGroupBox *deviceOptions;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *armCuff;
    QLabel *armBandSizeLabel;
    QComboBox *armComboBox;
    QLabel *armLabel;
    QComboBox *armBandSizeComboBox;
    QHBoxLayout *startStop;
    QSpacerItem *horizontalSpacer;
    QPushButton *connectPushButton;
    QPushButton *disconnectPushButton;
    QHBoxLayout *horizontalLayout;
    QPushButton *cyclePushButton;
    QPushButton *clearPushButton;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *startPushButton;
    QPushButton *stopPushButton;
    QGroupBox *currentMeasurement;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *state;
    QLabel *stateLabel;
    QLabel *stateValue;
    QHBoxLayout *reading;
    QLabel *readingLabel;
    QLabel *readingValue;
    QHBoxLayout *cuffPressure;
    QLabel *cuffPressureLabel;
    QLabel *cuffPressureValue;
    QHBoxLayout *cycle;
    QLabel *cycleLabel;
    QLabel *cycleValue;
    MeasurementTable *measurementTable;
    QSpacerItem *verticalSpacer;

    void setupUi(QDialog *BloodPressureDialog)
    {
        if (BloodPressureDialog->objectName().isEmpty())
            BloodPressureDialog->setObjectName(QString::fromUtf8("BloodPressureDialog"));
        BloodPressureDialog->resize(643, 418);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(BloodPressureDialog->sizePolicy().hasHeightForWidth());
        BloodPressureDialog->setSizePolicy(sizePolicy);
        verticalLayout_2 = new QVBoxLayout(BloodPressureDialog);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        testInfoWidget = new TestInfoWidget(BloodPressureDialog);
        testInfoWidget->setObjectName(QString::fromUtf8("testInfoWidget"));

        verticalLayout_2->addWidget(testInfoWidget);

        deviceOptions = new QGroupBox(BloodPressureDialog);
        deviceOptions->setObjectName(QString::fromUtf8("deviceOptions"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(deviceOptions->sizePolicy().hasHeightForWidth());
        deviceOptions->setSizePolicy(sizePolicy1);
        verticalLayout = new QVBoxLayout(deviceOptions);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        armCuff = new QHBoxLayout();
        armCuff->setObjectName(QString::fromUtf8("armCuff"));
        armBandSizeLabel = new QLabel(deviceOptions);
        armBandSizeLabel->setObjectName(QString::fromUtf8("armBandSizeLabel"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(armBandSizeLabel->sizePolicy().hasHeightForWidth());
        armBandSizeLabel->setSizePolicy(sizePolicy2);
        armBandSizeLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        armCuff->addWidget(armBandSizeLabel);

        armComboBox = new QComboBox(deviceOptions);
        armComboBox->setObjectName(QString::fromUtf8("armComboBox"));
        QSizePolicy sizePolicy3(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(armComboBox->sizePolicy().hasHeightForWidth());
        armComboBox->setSizePolicy(sizePolicy3);

        armCuff->addWidget(armComboBox);

        armLabel = new QLabel(deviceOptions);
        armLabel->setObjectName(QString::fromUtf8("armLabel"));
        sizePolicy2.setHeightForWidth(armLabel->sizePolicy().hasHeightForWidth());
        armLabel->setSizePolicy(sizePolicy2);
        armLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        armCuff->addWidget(armLabel);

        armBandSizeComboBox = new QComboBox(deviceOptions);
        armBandSizeComboBox->setObjectName(QString::fromUtf8("armBandSizeComboBox"));
        sizePolicy3.setHeightForWidth(armBandSizeComboBox->sizePolicy().hasHeightForWidth());
        armBandSizeComboBox->setSizePolicy(sizePolicy3);

        armCuff->addWidget(armBandSizeComboBox);


        verticalLayout->addLayout(armCuff);

        startStop = new QHBoxLayout();
        startStop->setObjectName(QString::fromUtf8("startStop"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        startStop->addItem(horizontalSpacer);

        connectPushButton = new QPushButton(deviceOptions);
        connectPushButton->setObjectName(QString::fromUtf8("connectPushButton"));

        startStop->addWidget(connectPushButton);

        disconnectPushButton = new QPushButton(deviceOptions);
        disconnectPushButton->setObjectName(QString::fromUtf8("disconnectPushButton"));
        disconnectPushButton->setEnabled(false);

        startStop->addWidget(disconnectPushButton);


        verticalLayout->addLayout(startStop);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        cyclePushButton = new QPushButton(deviceOptions);
        cyclePushButton->setObjectName(QString::fromUtf8("cyclePushButton"));
        cyclePushButton->setEnabled(false);

        horizontalLayout->addWidget(cyclePushButton);

        clearPushButton = new QPushButton(deviceOptions);
        clearPushButton->setObjectName(QString::fromUtf8("clearPushButton"));
        clearPushButton->setEnabled(false);

        horizontalLayout->addWidget(clearPushButton);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        startPushButton = new QPushButton(deviceOptions);
        startPushButton->setObjectName(QString::fromUtf8("startPushButton"));
        startPushButton->setEnabled(false);

        horizontalLayout_2->addWidget(startPushButton);

        stopPushButton = new QPushButton(deviceOptions);
        stopPushButton->setObjectName(QString::fromUtf8("stopPushButton"));
        stopPushButton->setEnabled(false);

        horizontalLayout_2->addWidget(stopPushButton);


        verticalLayout->addLayout(horizontalLayout_2);


        verticalLayout_2->addWidget(deviceOptions);

        currentMeasurement = new QGroupBox(BloodPressureDialog);
        currentMeasurement->setObjectName(QString::fromUtf8("currentMeasurement"));
        verticalLayout_3 = new QVBoxLayout(currentMeasurement);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        state = new QHBoxLayout();
        state->setObjectName(QString::fromUtf8("state"));
        stateLabel = new QLabel(currentMeasurement);
        stateLabel->setObjectName(QString::fromUtf8("stateLabel"));

        state->addWidget(stateLabel);

        stateValue = new QLabel(currentMeasurement);
        stateValue->setObjectName(QString::fromUtf8("stateValue"));

        state->addWidget(stateValue);


        verticalLayout_3->addLayout(state);

        reading = new QHBoxLayout();
        reading->setObjectName(QString::fromUtf8("reading"));
        readingLabel = new QLabel(currentMeasurement);
        readingLabel->setObjectName(QString::fromUtf8("readingLabel"));

        reading->addWidget(readingLabel);

        readingValue = new QLabel(currentMeasurement);
        readingValue->setObjectName(QString::fromUtf8("readingValue"));

        reading->addWidget(readingValue);


        verticalLayout_3->addLayout(reading);

        cuffPressure = new QHBoxLayout();
        cuffPressure->setObjectName(QString::fromUtf8("cuffPressure"));
        cuffPressureLabel = new QLabel(currentMeasurement);
        cuffPressureLabel->setObjectName(QString::fromUtf8("cuffPressureLabel"));

        cuffPressure->addWidget(cuffPressureLabel);

        cuffPressureValue = new QLabel(currentMeasurement);
        cuffPressureValue->setObjectName(QString::fromUtf8("cuffPressureValue"));

        cuffPressure->addWidget(cuffPressureValue);


        verticalLayout_3->addLayout(cuffPressure);

        cycle = new QHBoxLayout();
        cycle->setObjectName(QString::fromUtf8("cycle"));
        cycleLabel = new QLabel(currentMeasurement);
        cycleLabel->setObjectName(QString::fromUtf8("cycleLabel"));

        cycle->addWidget(cycleLabel);

        cycleValue = new QLabel(currentMeasurement);
        cycleValue->setObjectName(QString::fromUtf8("cycleValue"));

        cycle->addWidget(cycleValue);


        verticalLayout_3->addLayout(cycle);


        verticalLayout_2->addWidget(currentMeasurement);

        measurementTable = new MeasurementTable(BloodPressureDialog);
        measurementTable->setObjectName(QString::fromUtf8("measurementTable"));

        verticalLayout_2->addWidget(measurementTable);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        retranslateUi(BloodPressureDialog);

        QMetaObject::connectSlotsByName(BloodPressureDialog);
    } // setupUi

    void retranslateUi(QDialog *BloodPressureDialog)
    {
        BloodPressureDialog->setWindowTitle(QCoreApplication::translate("BloodPressureDialog", "Blood Pressure", nullptr));
        deviceOptions->setTitle(QCoreApplication::translate("BloodPressureDialog", "Device Options", nullptr));
        armBandSizeLabel->setText(QCoreApplication::translate("BloodPressureDialog", "Arm", nullptr));
        armLabel->setText(QCoreApplication::translate("BloodPressureDialog", "Cuff Size", nullptr));
        connectPushButton->setText(QCoreApplication::translate("BloodPressureDialog", "Connect", nullptr));
        disconnectPushButton->setText(QCoreApplication::translate("BloodPressureDialog", "Disconnect", nullptr));
        cyclePushButton->setText(QCoreApplication::translate("BloodPressureDialog", "Cycle", nullptr));
        clearPushButton->setText(QCoreApplication::translate("BloodPressureDialog", "Clear", nullptr));
        startPushButton->setText(QCoreApplication::translate("BloodPressureDialog", "Start", nullptr));
        stopPushButton->setText(QCoreApplication::translate("BloodPressureDialog", "Stop", nullptr));
        currentMeasurement->setTitle(QCoreApplication::translate("BloodPressureDialog", "Current Measurement", nullptr));
        stateLabel->setText(QCoreApplication::translate("BloodPressureDialog", "State", nullptr));
        stateValue->setText(QString());
        readingLabel->setText(QCoreApplication::translate("BloodPressureDialog", "Reading", nullptr));
        readingValue->setText(QString());
        cuffPressureLabel->setText(QCoreApplication::translate("BloodPressureDialog", "Cuff Pressure", nullptr));
        cuffPressureValue->setText(QString());
        cycleLabel->setText(QCoreApplication::translate("BloodPressureDialog", "Cycle", nullptr));
        cycleValue->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class BloodPressureDialog: public Ui_BloodPressureDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BLOOD_PRESSURE_DIALOG_H

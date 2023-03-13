/********************************************************************************
** Form generated from reading UI file 'bloodpressuredialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BLOODPRESSUREDIALOG_H
#define UI_BLOODPRESSUREDIALOG_H

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
#include "widgets/BarcodeWidget.h"
#include "widgets/MeasureWidget.h"

QT_BEGIN_NAMESPACE

class Ui_BloodPressureDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QGroupBox *armBandGroupBox;
    QHBoxLayout *armBandHorizontalLayout;
    QLabel *armBandSizeLabel;
    QComboBox *armBandSizeComboBox;
    QLabel *armLabel;
    QComboBox *armComboBox;
    QSpacerItem *horizontalSpacer;
    QGroupBox *pathGroupBox;
    QHBoxLayout *horizontalLayout_2;
    QLabel *deviceLabel;
    QComboBox *deviceComboBox;
    QPushButton *connectButton;
    QPushButton *disconnectButton;
    QGroupBox *measureGroupBox;
    QVBoxLayout *verticalLayout;
    MeasureWidget *measureWidget;

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
        armBandGroupBox = new QGroupBox(BloodPressureDialog);
        armBandGroupBox->setObjectName(QString::fromUtf8("armBandGroupBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(armBandGroupBox->sizePolicy().hasHeightForWidth());
        armBandGroupBox->setSizePolicy(sizePolicy1);
        armBandHorizontalLayout = new QHBoxLayout(armBandGroupBox);
        armBandHorizontalLayout->setObjectName(QString::fromUtf8("armBandHorizontalLayout"));
        armBandSizeLabel = new QLabel(armBandGroupBox);
        armBandSizeLabel->setObjectName(QString::fromUtf8("armBandSizeLabel"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(armBandSizeLabel->sizePolicy().hasHeightForWidth());
        armBandSizeLabel->setSizePolicy(sizePolicy2);
        armBandSizeLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        armBandHorizontalLayout->addWidget(armBandSizeLabel);

        armBandSizeComboBox = new QComboBox(armBandGroupBox);
        armBandSizeComboBox->setObjectName(QString::fromUtf8("armBandSizeComboBox"));
        QSizePolicy sizePolicy3(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(armBandSizeComboBox->sizePolicy().hasHeightForWidth());
        armBandSizeComboBox->setSizePolicy(sizePolicy3);

        armBandHorizontalLayout->addWidget(armBandSizeComboBox);

        armLabel = new QLabel(armBandGroupBox);
        armLabel->setObjectName(QString::fromUtf8("armLabel"));
        sizePolicy2.setHeightForWidth(armLabel->sizePolicy().hasHeightForWidth());
        armLabel->setSizePolicy(sizePolicy2);
        armLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        armBandHorizontalLayout->addWidget(armLabel);

        armComboBox = new QComboBox(armBandGroupBox);
        armComboBox->setObjectName(QString::fromUtf8("armComboBox"));
        sizePolicy3.setHeightForWidth(armComboBox->sizePolicy().hasHeightForWidth());
        armComboBox->setSizePolicy(sizePolicy3);

        armBandHorizontalLayout->addWidget(armComboBox);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        armBandHorizontalLayout->addItem(horizontalSpacer);


        verticalLayout_2->addWidget(armBandGroupBox);

        pathGroupBox = new QGroupBox(BloodPressureDialog);
        pathGroupBox->setObjectName(QString::fromUtf8("pathGroupBox"));
        sizePolicy1.setHeightForWidth(pathGroupBox->sizePolicy().hasHeightForWidth());
        pathGroupBox->setSizePolicy(sizePolicy1);
        horizontalLayout_2 = new QHBoxLayout(pathGroupBox);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        deviceLabel = new QLabel(pathGroupBox);
        deviceLabel->setObjectName(QString::fromUtf8("deviceLabel"));
        sizePolicy2.setHeightForWidth(deviceLabel->sizePolicy().hasHeightForWidth());
        deviceLabel->setSizePolicy(sizePolicy2);
        deviceLabel->setLayoutDirection(Qt::LeftToRight);
        deviceLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_2->addWidget(deviceLabel);

        deviceComboBox = new QComboBox(pathGroupBox);
        deviceComboBox->setObjectName(QString::fromUtf8("deviceComboBox"));
        sizePolicy3.setHeightForWidth(deviceComboBox->sizePolicy().hasHeightForWidth());
        deviceComboBox->setSizePolicy(sizePolicy3);

        horizontalLayout_2->addWidget(deviceComboBox);

        connectButton = new QPushButton(pathGroupBox);
        connectButton->setObjectName(QString::fromUtf8("connectButton"));

        horizontalLayout_2->addWidget(connectButton);

        disconnectButton = new QPushButton(pathGroupBox);
        disconnectButton->setObjectName(QString::fromUtf8("disconnectButton"));

        horizontalLayout_2->addWidget(disconnectButton);


        verticalLayout_2->addWidget(pathGroupBox);

        measureGroupBox = new QGroupBox(BloodPressureDialog);
        measureGroupBox->setObjectName(QString::fromUtf8("measureGroupBox"));
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(measureGroupBox->sizePolicy().hasHeightForWidth());
        measureGroupBox->setSizePolicy(sizePolicy4);
        verticalLayout = new QVBoxLayout(measureGroupBox);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        measureWidget = new MeasureWidget(measureGroupBox);
        measureWidget->setObjectName(QString::fromUtf8("measureWidget"));

        verticalLayout->addWidget(measureWidget);


        verticalLayout_2->addWidget(measureGroupBox);


        retranslateUi(BloodPressureDialog);

        QMetaObject::connectSlotsByName(BloodPressureDialog);
    } // setupUi

    void retranslateUi(QDialog *BloodPressureDialog)
    {
        BloodPressureDialog->setWindowTitle(QCoreApplication::translate("BloodPressureDialog", "Blood Pressure", nullptr));
        armBandGroupBox->setTitle(QCoreApplication::translate("BloodPressureDialog", "Cuff Information", nullptr));
        armBandSizeLabel->setText(QCoreApplication::translate("BloodPressureDialog", "Cuff size:", nullptr));
        armLabel->setText(QCoreApplication::translate("BloodPressureDialog", "Arm used:", nullptr));
        pathGroupBox->setTitle(QCoreApplication::translate("BloodPressureDialog", "USB Devices", nullptr));
        deviceLabel->setText(QCoreApplication::translate("BloodPressureDialog", "Device:", nullptr));
        connectButton->setText(QCoreApplication::translate("BloodPressureDialog", "Connect", nullptr));
        disconnectButton->setText(QCoreApplication::translate("BloodPressureDialog", "Disconnect", nullptr));
        measureGroupBox->setTitle(QCoreApplication::translate("BloodPressureDialog", "Measure", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BloodPressureDialog: public Ui_BloodPressureDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BLOODPRESSUREDIALOG_H

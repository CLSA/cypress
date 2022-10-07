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
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include "widgets/BarcodeWidget.h"
#include "widgets/MeasureWidget.h"

QT_BEGIN_NAMESPACE

class Ui_BloodPressureDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QGroupBox *idGroupBox;
    QHBoxLayout *horizontalLayout;
    BarcodeWidget *barcodeWidget;
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
    QStatusBar *statusBar;

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
        idGroupBox = new QGroupBox(BloodPressureDialog);
        idGroupBox->setObjectName(QString::fromUtf8("idGroupBox"));
        sizePolicy.setHeightForWidth(idGroupBox->sizePolicy().hasHeightForWidth());
        idGroupBox->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(idGroupBox);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        barcodeWidget = new BarcodeWidget(idGroupBox);
        barcodeWidget->setObjectName(QString::fromUtf8("barcodeWidget"));

        horizontalLayout->addWidget(barcodeWidget);


        verticalLayout_2->addWidget(idGroupBox);

        armBandGroupBox = new QGroupBox(BloodPressureDialog);
        armBandGroupBox->setObjectName(QString::fromUtf8("armBandGroupBox"));
        armBandHorizontalLayout = new QHBoxLayout(armBandGroupBox);
        armBandHorizontalLayout->setObjectName(QString::fromUtf8("armBandHorizontalLayout"));
        armBandSizeLabel = new QLabel(armBandGroupBox);
        armBandSizeLabel->setObjectName(QString::fromUtf8("armBandSizeLabel"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(armBandSizeLabel->sizePolicy().hasHeightForWidth());
        armBandSizeLabel->setSizePolicy(sizePolicy1);
        armBandSizeLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        armBandHorizontalLayout->addWidget(armBandSizeLabel);

        armBandSizeComboBox = new QComboBox(armBandGroupBox);
        armBandSizeComboBox->setObjectName(QString::fromUtf8("armBandSizeComboBox"));
        QSizePolicy sizePolicy2(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(armBandSizeComboBox->sizePolicy().hasHeightForWidth());
        armBandSizeComboBox->setSizePolicy(sizePolicy2);

        armBandHorizontalLayout->addWidget(armBandSizeComboBox);

        armLabel = new QLabel(armBandGroupBox);
        armLabel->setObjectName(QString::fromUtf8("armLabel"));
        sizePolicy1.setHeightForWidth(armLabel->sizePolicy().hasHeightForWidth());
        armLabel->setSizePolicy(sizePolicy1);
        armLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        armBandHorizontalLayout->addWidget(armLabel);

        armComboBox = new QComboBox(armBandGroupBox);
        armComboBox->setObjectName(QString::fromUtf8("armComboBox"));
        sizePolicy2.setHeightForWidth(armComboBox->sizePolicy().hasHeightForWidth());
        armComboBox->setSizePolicy(sizePolicy2);

        armBandHorizontalLayout->addWidget(armComboBox);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        armBandHorizontalLayout->addItem(horizontalSpacer);


        verticalLayout_2->addWidget(armBandGroupBox);

        pathGroupBox = new QGroupBox(BloodPressureDialog);
        pathGroupBox->setObjectName(QString::fromUtf8("pathGroupBox"));
        sizePolicy.setHeightForWidth(pathGroupBox->sizePolicy().hasHeightForWidth());
        pathGroupBox->setSizePolicy(sizePolicy);
        horizontalLayout_2 = new QHBoxLayout(pathGroupBox);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        deviceLabel = new QLabel(pathGroupBox);
        deviceLabel->setObjectName(QString::fromUtf8("deviceLabel"));
        sizePolicy1.setHeightForWidth(deviceLabel->sizePolicy().hasHeightForWidth());
        deviceLabel->setSizePolicy(sizePolicy1);
        deviceLabel->setLayoutDirection(Qt::LeftToRight);
        deviceLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_2->addWidget(deviceLabel);

        deviceComboBox = new QComboBox(pathGroupBox);
        deviceComboBox->setObjectName(QString::fromUtf8("deviceComboBox"));
        sizePolicy2.setHeightForWidth(deviceComboBox->sizePolicy().hasHeightForWidth());
        deviceComboBox->setSizePolicy(sizePolicy2);

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
        verticalLayout = new QVBoxLayout(measureGroupBox);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        measureWidget = new MeasureWidget(measureGroupBox);
        measureWidget->setObjectName(QString::fromUtf8("measureWidget"));

        verticalLayout->addWidget(measureWidget);


        verticalLayout_2->addWidget(measureGroupBox);

        statusBar = new QStatusBar(BloodPressureDialog);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        statusBar->setSizeGripEnabled(false);

        verticalLayout_2->addWidget(statusBar);


        retranslateUi(BloodPressureDialog);

        QMetaObject::connectSlotsByName(BloodPressureDialog);
    } // setupUi

    void retranslateUi(QDialog *BloodPressureDialog)
    {
        BloodPressureDialog->setWindowTitle(QCoreApplication::translate("BloodPressureDialog", "Blood Pressure", nullptr));
        idGroupBox->setTitle(QCoreApplication::translate("BloodPressureDialog", "barcode", nullptr));
        armBandGroupBox->setTitle(QCoreApplication::translate("BloodPressureDialog", "cuff information", nullptr));
        armBandSizeLabel->setText(QCoreApplication::translate("BloodPressureDialog", "Cuff size:", nullptr));
        armLabel->setText(QCoreApplication::translate("BloodPressureDialog", "Arm used:", nullptr));
        pathGroupBox->setTitle(QCoreApplication::translate("BloodPressureDialog", "Usb devices", nullptr));
        deviceLabel->setText(QCoreApplication::translate("BloodPressureDialog", "Device:", nullptr));
        connectButton->setText(QCoreApplication::translate("BloodPressureDialog", "Connect", nullptr));
        disconnectButton->setText(QCoreApplication::translate("BloodPressureDialog", "Disconnect", nullptr));
        measureGroupBox->setTitle(QCoreApplication::translate("BloodPressureDialog", "measure", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BloodPressureDialog: public Ui_BloodPressureDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BLOODPRESSUREDIALOG_H

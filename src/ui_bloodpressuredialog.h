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
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label;
    QLabel *participantId;
    QGroupBox *pathGroupBox;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_2;
    QLabel *deviceLabel;
    QComboBox *deviceComboBox;
    QPushButton *connectButton;
    QPushButton *disconnectButton;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *measureButton;
    QPushButton *submitButton;
    QGroupBox *armBandGroupBox;
    QHBoxLayout *armBandHorizontalLayout;
    QLabel *armBandSizeLabel;
    QComboBox *armBandSizeComboBox;
    QLabel *armLabel;
    QComboBox *armComboBox;
    QSpacerItem *horizontalSpacer;
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
        groupBox = new QGroupBox(BloodPressureDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout_4 = new QVBoxLayout(groupBox);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);

        horizontalLayout_3->addWidget(label);

        participantId = new QLabel(groupBox);
        participantId->setObjectName(QString::fromUtf8("participantId"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(participantId->sizePolicy().hasHeightForWidth());
        participantId->setSizePolicy(sizePolicy2);

        horizontalLayout_3->addWidget(participantId);


        verticalLayout_4->addLayout(horizontalLayout_3);


        verticalLayout_2->addWidget(groupBox);

        pathGroupBox = new QGroupBox(BloodPressureDialog);
        pathGroupBox->setObjectName(QString::fromUtf8("pathGroupBox"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(pathGroupBox->sizePolicy().hasHeightForWidth());
        pathGroupBox->setSizePolicy(sizePolicy3);
        verticalLayout_3 = new QVBoxLayout(pathGroupBox);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        deviceLabel = new QLabel(pathGroupBox);
        deviceLabel->setObjectName(QString::fromUtf8("deviceLabel"));
        QSizePolicy sizePolicy4(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(deviceLabel->sizePolicy().hasHeightForWidth());
        deviceLabel->setSizePolicy(sizePolicy4);
        deviceLabel->setLayoutDirection(Qt::LeftToRight);
        deviceLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_2->addWidget(deviceLabel);

        deviceComboBox = new QComboBox(pathGroupBox);
        deviceComboBox->setObjectName(QString::fromUtf8("deviceComboBox"));
        QSizePolicy sizePolicy5(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(deviceComboBox->sizePolicy().hasHeightForWidth());
        deviceComboBox->setSizePolicy(sizePolicy5);

        horizontalLayout_2->addWidget(deviceComboBox);

        connectButton = new QPushButton(pathGroupBox);
        connectButton->setObjectName(QString::fromUtf8("connectButton"));
        connectButton->setEnabled(false);

        horizontalLayout_2->addWidget(connectButton);

        disconnectButton = new QPushButton(pathGroupBox);
        disconnectButton->setObjectName(QString::fromUtf8("disconnectButton"));
        disconnectButton->setEnabled(false);

        horizontalLayout_2->addWidget(disconnectButton);


        verticalLayout_3->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        measureButton = new QPushButton(pathGroupBox);
        measureButton->setObjectName(QString::fromUtf8("measureButton"));
        measureButton->setEnabled(false);

        horizontalLayout->addWidget(measureButton);

        submitButton = new QPushButton(pathGroupBox);
        submitButton->setObjectName(QString::fromUtf8("submitButton"));
        submitButton->setEnabled(false);

        horizontalLayout->addWidget(submitButton);


        verticalLayout_3->addLayout(horizontalLayout);


        verticalLayout_2->addWidget(pathGroupBox);

        armBandGroupBox = new QGroupBox(BloodPressureDialog);
        armBandGroupBox->setObjectName(QString::fromUtf8("armBandGroupBox"));
        sizePolicy3.setHeightForWidth(armBandGroupBox->sizePolicy().hasHeightForWidth());
        armBandGroupBox->setSizePolicy(sizePolicy3);
        armBandHorizontalLayout = new QHBoxLayout(armBandGroupBox);
        armBandHorizontalLayout->setObjectName(QString::fromUtf8("armBandHorizontalLayout"));
        armBandSizeLabel = new QLabel(armBandGroupBox);
        armBandSizeLabel->setObjectName(QString::fromUtf8("armBandSizeLabel"));
        sizePolicy4.setHeightForWidth(armBandSizeLabel->sizePolicy().hasHeightForWidth());
        armBandSizeLabel->setSizePolicy(sizePolicy4);
        armBandSizeLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        armBandHorizontalLayout->addWidget(armBandSizeLabel);

        armBandSizeComboBox = new QComboBox(armBandGroupBox);
        armBandSizeComboBox->setObjectName(QString::fromUtf8("armBandSizeComboBox"));
        sizePolicy5.setHeightForWidth(armBandSizeComboBox->sizePolicy().hasHeightForWidth());
        armBandSizeComboBox->setSizePolicy(sizePolicy5);

        armBandHorizontalLayout->addWidget(armBandSizeComboBox);

        armLabel = new QLabel(armBandGroupBox);
        armLabel->setObjectName(QString::fromUtf8("armLabel"));
        sizePolicy4.setHeightForWidth(armLabel->sizePolicy().hasHeightForWidth());
        armLabel->setSizePolicy(sizePolicy4);
        armLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        armBandHorizontalLayout->addWidget(armLabel);

        armComboBox = new QComboBox(armBandGroupBox);
        armComboBox->setObjectName(QString::fromUtf8("armComboBox"));
        sizePolicy5.setHeightForWidth(armComboBox->sizePolicy().hasHeightForWidth());
        armComboBox->setSizePolicy(sizePolicy5);

        armBandHorizontalLayout->addWidget(armComboBox);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        armBandHorizontalLayout->addItem(horizontalSpacer);


        verticalLayout_2->addWidget(armBandGroupBox);

        measureGroupBox = new QGroupBox(BloodPressureDialog);
        measureGroupBox->setObjectName(QString::fromUtf8("measureGroupBox"));
        QSizePolicy sizePolicy6(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(measureGroupBox->sizePolicy().hasHeightForWidth());
        measureGroupBox->setSizePolicy(sizePolicy6);
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
        groupBox->setTitle(QString());
        label->setText(QCoreApplication::translate("BloodPressureDialog", "Participant ID:", nullptr));
        participantId->setText(QString());
        pathGroupBox->setTitle(QString());
        deviceLabel->setText(QCoreApplication::translate("BloodPressureDialog", "Device:", nullptr));
        connectButton->setText(QCoreApplication::translate("BloodPressureDialog", "Connect", nullptr));
        disconnectButton->setText(QCoreApplication::translate("BloodPressureDialog", "Disconnect", nullptr));
        measureButton->setText(QCoreApplication::translate("BloodPressureDialog", "Measure", nullptr));
        submitButton->setText(QCoreApplication::translate("BloodPressureDialog", "Submit", nullptr));
        armBandGroupBox->setTitle(QString());
        armBandSizeLabel->setText(QCoreApplication::translate("BloodPressureDialog", "Cuff size:", nullptr));
        armLabel->setText(QCoreApplication::translate("BloodPressureDialog", "Arm used:", nullptr));
        measureGroupBox->setTitle(QString());
    } // retranslateUi

};

namespace Ui {
    class BloodPressureDialog: public Ui_BloodPressureDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BLOODPRESSUREDIALOG_H

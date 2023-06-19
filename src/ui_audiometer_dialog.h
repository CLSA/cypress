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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include "widgets/barcode_widget.h"
#include "widgets/measure_widget.h"

QT_BEGIN_NAMESPACE

class Ui_AudiometerDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QLabel *participantId;
    QGroupBox *serialPortGroupBox;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QLabel *serialPortLabel;
    QComboBox *deviceComboBox;
    QPushButton *completeButton;
    QPushButton *connectButton;
    QPushButton *disconnectButton;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_2;
    QPushButton *pushButton;
    QGroupBox *measureGroupBox;
    QVBoxLayout *verticalLayout_3;
    MeasureWidget *measureWidget;

    void setupUi(QDialog *AudiometerDialog)
    {
        if (AudiometerDialog->objectName().isEmpty())
            AudiometerDialog->setObjectName(QString::fromUtf8("AudiometerDialog"));
        AudiometerDialog->resize(561, 541);
        verticalLayout_2 = new QVBoxLayout(AudiometerDialog);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        groupBox = new QGroupBox(AudiometerDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout_4 = new QVBoxLayout(groupBox);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(label);

        participantId = new QLabel(groupBox);
        participantId->setObjectName(QString::fromUtf8("participantId"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(participantId->sizePolicy().hasHeightForWidth());
        participantId->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(participantId);


        verticalLayout_4->addLayout(horizontalLayout_2);


        verticalLayout_2->addWidget(groupBox);

        serialPortGroupBox = new QGroupBox(AudiometerDialog);
        serialPortGroupBox->setObjectName(QString::fromUtf8("serialPortGroupBox"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(serialPortGroupBox->sizePolicy().hasHeightForWidth());
        serialPortGroupBox->setSizePolicy(sizePolicy2);
        verticalLayout = new QVBoxLayout(serialPortGroupBox);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        serialPortLabel = new QLabel(serialPortGroupBox);
        serialPortLabel->setObjectName(QString::fromUtf8("serialPortLabel"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(serialPortLabel->sizePolicy().hasHeightForWidth());
        serialPortLabel->setSizePolicy(sizePolicy3);

        horizontalLayout_3->addWidget(serialPortLabel);

        deviceComboBox = new QComboBox(serialPortGroupBox);
        deviceComboBox->setObjectName(QString::fromUtf8("deviceComboBox"));
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(1);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(deviceComboBox->sizePolicy().hasHeightForWidth());
        deviceComboBox->setSizePolicy(sizePolicy4);

        horizontalLayout_3->addWidget(deviceComboBox);

        completeButton = new QPushButton(serialPortGroupBox);
        completeButton->setObjectName(QString::fromUtf8("completeButton"));

        horizontalLayout_3->addWidget(completeButton);

        connectButton = new QPushButton(serialPortGroupBox);
        connectButton->setObjectName(QString::fromUtf8("connectButton"));
        connectButton->setEnabled(false);

        horizontalLayout_3->addWidget(connectButton);

        disconnectButton = new QPushButton(serialPortGroupBox);
        disconnectButton->setObjectName(QString::fromUtf8("disconnectButton"));
        disconnectButton->setEnabled(false);

        horizontalLayout_3->addWidget(disconnectButton);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton_2 = new QPushButton(serialPortGroupBox);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setEnabled(false);

        horizontalLayout->addWidget(pushButton_2);

        pushButton = new QPushButton(serialPortGroupBox);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setEnabled(false);

        horizontalLayout->addWidget(pushButton);


        verticalLayout->addLayout(horizontalLayout);


        verticalLayout_2->addWidget(serialPortGroupBox);

        measureGroupBox = new QGroupBox(AudiometerDialog);
        measureGroupBox->setObjectName(QString::fromUtf8("measureGroupBox"));
        QSizePolicy sizePolicy5(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(measureGroupBox->sizePolicy().hasHeightForWidth());
        measureGroupBox->setSizePolicy(sizePolicy5);
        verticalLayout_3 = new QVBoxLayout(measureGroupBox);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        measureWidget = new MeasureWidget(measureGroupBox);
        measureWidget->setObjectName(QString::fromUtf8("measureWidget"));

        verticalLayout_3->addWidget(measureWidget);


        verticalLayout_2->addWidget(measureGroupBox);


        retranslateUi(AudiometerDialog);

        QMetaObject::connectSlotsByName(AudiometerDialog);
    } // setupUi

    void retranslateUi(QDialog *AudiometerDialog)
    {
        AudiometerDialog->setWindowTitle(QCoreApplication::translate("AudiometerDialog", "Hearing Measurement", nullptr));
        groupBox->setTitle(QString());
        label->setText(QCoreApplication::translate("AudiometerDialog", "Participant ID:", nullptr));
        participantId->setText(QString());
        serialPortGroupBox->setTitle(QString());
        serialPortLabel->setText(QCoreApplication::translate("AudiometerDialog", "Serial Port:", nullptr));
        completeButton->setText(QCoreApplication::translate("AudiometerDialog", "Complete", nullptr));
        connectButton->setText(QCoreApplication::translate("AudiometerDialog", "Connect", nullptr));
        disconnectButton->setText(QCoreApplication::translate("AudiometerDialog", "Disconnect", nullptr));
        pushButton_2->setText(QCoreApplication::translate("AudiometerDialog", "Measure", nullptr));
        pushButton->setText(QCoreApplication::translate("AudiometerDialog", "Submit", nullptr));
        measureGroupBox->setTitle(QString());
    } // retranslateUi

};

namespace Ui {
    class AudiometerDialog: public Ui_AudiometerDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AUDIOMETER_DIALOG_H

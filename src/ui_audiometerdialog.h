/********************************************************************************
** Form generated from reading UI file 'audiometerdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AUDIOMETERDIALOG_H
#define UI_AUDIOMETERDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include "widgets/BarcodeWidget.h"
#include "widgets/MeasureWidget.h"

QT_BEGIN_NAMESPACE

class Ui_AudiometerDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QGroupBox *idGroupBox;
    QHBoxLayout *horizontalLayout;
    BarcodeWidget *barcodeWidget;
    QGroupBox *serialPortGroupBox;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QLabel *serialPortLabel;
    QComboBox *deviceComboBox;
    QPushButton *connectButton;
    QPushButton *disconnectButton;
    QGroupBox *measureGroupBox;
    QVBoxLayout *verticalLayout_3;
    MeasureWidget *measureWidget;
    QStatusBar *statusBar;

    void setupUi(QDialog *AudiometerDialog)
    {
        if (AudiometerDialog->objectName().isEmpty())
            AudiometerDialog->setObjectName(QString::fromUtf8("AudiometerDialog"));
        AudiometerDialog->resize(561, 541);
        verticalLayout_2 = new QVBoxLayout(AudiometerDialog);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        idGroupBox = new QGroupBox(AudiometerDialog);
        idGroupBox->setObjectName(QString::fromUtf8("idGroupBox"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(idGroupBox->sizePolicy().hasHeightForWidth());
        idGroupBox->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(idGroupBox);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        barcodeWidget = new BarcodeWidget(idGroupBox);
        barcodeWidget->setObjectName(QString::fromUtf8("barcodeWidget"));

        horizontalLayout->addWidget(barcodeWidget);


        verticalLayout_2->addWidget(idGroupBox);

        serialPortGroupBox = new QGroupBox(AudiometerDialog);
        serialPortGroupBox->setObjectName(QString::fromUtf8("serialPortGroupBox"));
        verticalLayout = new QVBoxLayout(serialPortGroupBox);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        serialPortLabel = new QLabel(serialPortGroupBox);
        serialPortLabel->setObjectName(QString::fromUtf8("serialPortLabel"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(serialPortLabel->sizePolicy().hasHeightForWidth());
        serialPortLabel->setSizePolicy(sizePolicy1);

        horizontalLayout_3->addWidget(serialPortLabel);

        deviceComboBox = new QComboBox(serialPortGroupBox);
        deviceComboBox->setObjectName(QString::fromUtf8("deviceComboBox"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(1);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(deviceComboBox->sizePolicy().hasHeightForWidth());
        deviceComboBox->setSizePolicy(sizePolicy2);

        horizontalLayout_3->addWidget(deviceComboBox);

        connectButton = new QPushButton(serialPortGroupBox);
        connectButton->setObjectName(QString::fromUtf8("connectButton"));

        horizontalLayout_3->addWidget(connectButton);

        disconnectButton = new QPushButton(serialPortGroupBox);
        disconnectButton->setObjectName(QString::fromUtf8("disconnectButton"));

        horizontalLayout_3->addWidget(disconnectButton);


        verticalLayout->addLayout(horizontalLayout_3);


        verticalLayout_2->addWidget(serialPortGroupBox);

        measureGroupBox = new QGroupBox(AudiometerDialog);
        measureGroupBox->setObjectName(QString::fromUtf8("measureGroupBox"));
        sizePolicy.setHeightForWidth(measureGroupBox->sizePolicy().hasHeightForWidth());
        measureGroupBox->setSizePolicy(sizePolicy);
        verticalLayout_3 = new QVBoxLayout(measureGroupBox);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        measureWidget = new MeasureWidget(measureGroupBox);
        measureWidget->setObjectName(QString::fromUtf8("measureWidget"));

        verticalLayout_3->addWidget(measureWidget);


        verticalLayout_2->addWidget(measureGroupBox);

        statusBar = new QStatusBar(AudiometerDialog);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        statusBar->setSizeGripEnabled(false);

        verticalLayout_2->addWidget(statusBar);


        retranslateUi(AudiometerDialog);

        QMetaObject::connectSlotsByName(AudiometerDialog);
    } // setupUi

    void retranslateUi(QDialog *AudiometerDialog)
    {
        AudiometerDialog->setWindowTitle(QCoreApplication::translate("AudiometerDialog", "Hearing Measurement", nullptr));
        idGroupBox->setTitle(QCoreApplication::translate("AudiometerDialog", "barcode", nullptr));
        serialPortGroupBox->setTitle(QCoreApplication::translate("AudiometerDialog", "serial ports", nullptr));
        serialPortLabel->setText(QCoreApplication::translate("AudiometerDialog", "Serial Port:", nullptr));
        connectButton->setText(QCoreApplication::translate("AudiometerDialog", "Connect", nullptr));
        disconnectButton->setText(QCoreApplication::translate("AudiometerDialog", "Disconnect", nullptr));
        measureGroupBox->setTitle(QCoreApplication::translate("AudiometerDialog", "measure", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AudiometerDialog: public Ui_AudiometerDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AUDIOMETERDIALOG_H

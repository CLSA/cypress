/********************************************************************************
** Form generated from reading UI file 'serial_port_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERIAL_PORT_WIDGET_H
#define UI_SERIAL_PORT_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SerialPortWidget
{
public:
    QVBoxLayout *verticalLayout_2;
    QGroupBox *serialPortGroupBox;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QComboBox *deviceComboBox;
    QPushButton *disconnectButton;
    QPushButton *connectButton;

    void setupUi(QWidget *SerialPortWidget)
    {
        if (SerialPortWidget->objectName().isEmpty())
            SerialPortWidget->setObjectName(QString::fromUtf8("SerialPortWidget"));
        SerialPortWidget->resize(400, 87);
        verticalLayout_2 = new QVBoxLayout(SerialPortWidget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        serialPortGroupBox = new QGroupBox(SerialPortWidget);
        serialPortGroupBox->setObjectName(QString::fromUtf8("serialPortGroupBox"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(serialPortGroupBox->sizePolicy().hasHeightForWidth());
        serialPortGroupBox->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(serialPortGroupBox);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        deviceComboBox = new QComboBox(serialPortGroupBox);
        deviceComboBox->setObjectName(QString::fromUtf8("deviceComboBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(1);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(deviceComboBox->sizePolicy().hasHeightForWidth());
        deviceComboBox->setSizePolicy(sizePolicy1);

        horizontalLayout_3->addWidget(deviceComboBox);

        disconnectButton = new QPushButton(serialPortGroupBox);
        disconnectButton->setObjectName(QString::fromUtf8("disconnectButton"));
        disconnectButton->setEnabled(false);

        horizontalLayout_3->addWidget(disconnectButton);

        connectButton = new QPushButton(serialPortGroupBox);
        connectButton->setObjectName(QString::fromUtf8("connectButton"));
        connectButton->setEnabled(false);

        horizontalLayout_3->addWidget(connectButton);


        verticalLayout->addLayout(horizontalLayout_3);


        verticalLayout_2->addWidget(serialPortGroupBox);


        retranslateUi(SerialPortWidget);

        QMetaObject::connectSlotsByName(SerialPortWidget);
    } // setupUi

    void retranslateUi(QWidget *SerialPortWidget)
    {
        SerialPortWidget->setWindowTitle(QCoreApplication::translate("SerialPortWidget", "Form", nullptr));
        serialPortGroupBox->setTitle(QCoreApplication::translate("SerialPortWidget", "Serial Port", nullptr));
        disconnectButton->setText(QCoreApplication::translate("SerialPortWidget", "Disconnect", nullptr));
        connectButton->setText(QCoreApplication::translate("SerialPortWidget", "Connect", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SerialPortWidget: public Ui_SerialPortWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERIAL_PORT_WIDGET_H

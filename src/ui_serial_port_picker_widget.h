/********************************************************************************
** Form generated from reading UI file 'serial_port_picker_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERIAL_PORT_PICKER_WIDGET_H
#define UI_SERIAL_PORT_PICKER_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SerialPortPickerWidget
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QComboBox *serialPorts;
    QPushButton *scanDevicesButton;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_2;
    QLineEdit *productId;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_3;
    QLineEdit *vendorId;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_4;
    QLineEdit *manufacturer;
    QHBoxLayout *horizontalLayout;
    QLabel *label_5;
    QLineEdit *serialNumber;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_6;
    QLineEdit *description;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label;
    QComboBox *baudRates;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_7;
    QComboBox *dataBits;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_8;
    QComboBox *parity;

    void setupUi(QWidget *SerialPortPickerWidget)
    {
        if (SerialPortPickerWidget->objectName().isEmpty())
            SerialPortPickerWidget->setObjectName(QString::fromUtf8("SerialPortPickerWidget"));
        SerialPortPickerWidget->resize(450, 349);
        verticalLayout = new QVBoxLayout(SerialPortPickerWidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        groupBox = new QGroupBox(SerialPortPickerWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        serialPorts = new QComboBox(groupBox);
        serialPorts->setObjectName(QString::fromUtf8("serialPorts"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(serialPorts->sizePolicy().hasHeightForWidth());
        serialPorts->setSizePolicy(sizePolicy);

        horizontalLayout_3->addWidget(serialPorts);

        scanDevicesButton = new QPushButton(groupBox);
        scanDevicesButton->setObjectName(QString::fromUtf8("scanDevicesButton"));

        horizontalLayout_3->addWidget(scanDevicesButton);


        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy1);

        horizontalLayout_4->addWidget(label_2);

        productId = new QLineEdit(groupBox);
        productId->setObjectName(QString::fromUtf8("productId"));
        productId->setEnabled(false);

        horizontalLayout_4->addWidget(productId);


        verticalLayout_2->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        sizePolicy1.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy1);

        horizontalLayout_5->addWidget(label_3);

        vendorId = new QLineEdit(groupBox);
        vendorId->setObjectName(QString::fromUtf8("vendorId"));
        vendorId->setEnabled(false);

        horizontalLayout_5->addWidget(vendorId);


        verticalLayout_2->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        sizePolicy1.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy1);

        horizontalLayout_6->addWidget(label_4);

        manufacturer = new QLineEdit(groupBox);
        manufacturer->setObjectName(QString::fromUtf8("manufacturer"));
        manufacturer->setEnabled(false);

        horizontalLayout_6->addWidget(manufacturer);


        verticalLayout_2->addLayout(horizontalLayout_6);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        sizePolicy1.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(label_5);

        serialNumber = new QLineEdit(groupBox);
        serialNumber->setObjectName(QString::fromUtf8("serialNumber"));
        serialNumber->setEnabled(false);

        horizontalLayout->addWidget(serialNumber);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        sizePolicy1.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(label_6);

        description = new QLineEdit(groupBox);
        description->setObjectName(QString::fromUtf8("description"));
        description->setEnabled(false);

        horizontalLayout_2->addWidget(description);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_7->addWidget(label);

        baudRates = new QComboBox(groupBox);
        baudRates->setObjectName(QString::fromUtf8("baudRates"));

        horizontalLayout_7->addWidget(baudRates);


        verticalLayout_2->addLayout(horizontalLayout_7);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        label_7 = new QLabel(groupBox);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        horizontalLayout_8->addWidget(label_7);

        dataBits = new QComboBox(groupBox);
        dataBits->setObjectName(QString::fromUtf8("dataBits"));

        horizontalLayout_8->addWidget(dataBits);


        verticalLayout_2->addLayout(horizontalLayout_8);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        label_8 = new QLabel(groupBox);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        horizontalLayout_9->addWidget(label_8);

        parity = new QComboBox(groupBox);
        parity->setObjectName(QString::fromUtf8("parity"));

        horizontalLayout_9->addWidget(parity);


        verticalLayout_2->addLayout(horizontalLayout_9);


        verticalLayout->addWidget(groupBox);


        retranslateUi(SerialPortPickerWidget);

        QMetaObject::connectSlotsByName(SerialPortPickerWidget);
    } // setupUi

    void retranslateUi(QWidget *SerialPortPickerWidget)
    {
        SerialPortPickerWidget->setWindowTitle(QCoreApplication::translate("SerialPortPickerWidget", "Form", nullptr));
        groupBox->setTitle(QCoreApplication::translate("SerialPortPickerWidget", "Serial Port", nullptr));
        scanDevicesButton->setText(QCoreApplication::translate("SerialPortPickerWidget", "Scan Devices", nullptr));
        label_2->setText(QCoreApplication::translate("SerialPortPickerWidget", "Product ID", nullptr));
        label_3->setText(QCoreApplication::translate("SerialPortPickerWidget", "Vendor ID", nullptr));
        label_4->setText(QCoreApplication::translate("SerialPortPickerWidget", "Manufacturer", nullptr));
        label_5->setText(QCoreApplication::translate("SerialPortPickerWidget", "Serial Number", nullptr));
        label_6->setText(QCoreApplication::translate("SerialPortPickerWidget", "Description", nullptr));
        label->setText(QCoreApplication::translate("SerialPortPickerWidget", "Baud Rate", nullptr));
        label_7->setText(QCoreApplication::translate("SerialPortPickerWidget", "Data Bits", nullptr));
        label_8->setText(QCoreApplication::translate("SerialPortPickerWidget", "Parity", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SerialPortPickerWidget: public Ui_SerialPortPickerWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERIAL_PORT_PICKER_WIDGET_H

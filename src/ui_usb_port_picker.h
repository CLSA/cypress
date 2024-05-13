/********************************************************************************
** Form generated from reading UI file 'usb_port_picker.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_USB_PORT_PICKER_H
#define UI_USB_PORT_PICKER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UsbPortPicker
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *usbGroup;
    QHBoxLayout *horizontalLayout;
    QComboBox *usbPortComboBox;
    QPushButton *usbDisconnectPushButton;
    QPushButton *usbConnectPushButton;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *UsbPortPicker)
    {
        if (UsbPortPicker->objectName().isEmpty())
            UsbPortPicker->setObjectName(QString::fromUtf8("UsbPortPicker"));
        UsbPortPicker->resize(400, 300);
        verticalLayout = new QVBoxLayout(UsbPortPicker);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        usbGroup = new QGroupBox(UsbPortPicker);
        usbGroup->setObjectName(QString::fromUtf8("usbGroup"));
        horizontalLayout = new QHBoxLayout(usbGroup);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        usbPortComboBox = new QComboBox(usbGroup);
        usbPortComboBox->setObjectName(QString::fromUtf8("usbPortComboBox"));

        horizontalLayout->addWidget(usbPortComboBox);

        usbDisconnectPushButton = new QPushButton(usbGroup);
        usbDisconnectPushButton->setObjectName(QString::fromUtf8("usbDisconnectPushButton"));

        horizontalLayout->addWidget(usbDisconnectPushButton);

        usbConnectPushButton = new QPushButton(usbGroup);
        usbConnectPushButton->setObjectName(QString::fromUtf8("usbConnectPushButton"));

        horizontalLayout->addWidget(usbConnectPushButton);


        verticalLayout->addWidget(usbGroup);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(UsbPortPicker);

        QMetaObject::connectSlotsByName(UsbPortPicker);
    } // setupUi

    void retranslateUi(QWidget *UsbPortPicker)
    {
        UsbPortPicker->setWindowTitle(QCoreApplication::translate("UsbPortPicker", "Form", nullptr));
        usbGroup->setTitle(QCoreApplication::translate("UsbPortPicker", "USB connection", nullptr));
        usbDisconnectPushButton->setText(QCoreApplication::translate("UsbPortPicker", "Disconnect", nullptr));
        usbConnectPushButton->setText(QCoreApplication::translate("UsbPortPicker", "Connect", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UsbPortPicker: public Ui_UsbPortPicker {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USB_PORT_PICKER_H

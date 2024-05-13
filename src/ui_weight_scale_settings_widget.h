/********************************************************************************
** Form generated from reading UI file 'weight_scale_settings_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WEIGHT_SCALE_SETTINGS_WIDGET_H
#define UI_WEIGHT_SCALE_SETTINGS_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "widgets/serial_port_picker_widget.h"

QT_BEGIN_NAMESPACE

class Ui_WeightScaleSettingsWidget
{
public:
    QVBoxLayout *verticalLayout;
    SerialPortPickerWidget *serialPortPicker;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *WeightScaleSettingsWidget)
    {
        if (WeightScaleSettingsWidget->objectName().isEmpty())
            WeightScaleSettingsWidget->setObjectName(QString::fromUtf8("WeightScaleSettingsWidget"));
        WeightScaleSettingsWidget->resize(400, 300);
        verticalLayout = new QVBoxLayout(WeightScaleSettingsWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        serialPortPicker = new SerialPortPickerWidget(WeightScaleSettingsWidget);
        serialPortPicker->setObjectName(QString::fromUtf8("serialPortPicker"));

        verticalLayout->addWidget(serialPortPicker);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(WeightScaleSettingsWidget);

        QMetaObject::connectSlotsByName(WeightScaleSettingsWidget);
    } // setupUi

    void retranslateUi(QWidget *WeightScaleSettingsWidget)
    {
        WeightScaleSettingsWidget->setWindowTitle(QCoreApplication::translate("WeightScaleSettingsWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WeightScaleSettingsWidget: public Ui_WeightScaleSettingsWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WEIGHT_SCALE_SETTINGS_WIDGET_H

/********************************************************************************
** Form generated from reading UI file 'blood_pressure_settings_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BLOOD_PRESSURE_SETTINGS_WIDGET_H
#define UI_BLOOD_PRESSURE_SETTINGS_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BloodPressureSettingsWidget
{
public:
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *BloodPressureSettingsWidget)
    {
        if (BloodPressureSettingsWidget->objectName().isEmpty())
            BloodPressureSettingsWidget->setObjectName(QString::fromUtf8("BloodPressureSettingsWidget"));
        BloodPressureSettingsWidget->resize(400, 300);
        verticalLayout = new QVBoxLayout(BloodPressureSettingsWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(BloodPressureSettingsWidget);

        QMetaObject::connectSlotsByName(BloodPressureSettingsWidget);
    } // setupUi

    void retranslateUi(QWidget *BloodPressureSettingsWidget)
    {
        BloodPressureSettingsWidget->setWindowTitle(QCoreApplication::translate("BloodPressureSettingsWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BloodPressureSettingsWidget: public Ui_BloodPressureSettingsWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BLOOD_PRESSURE_SETTINGS_WIDGET_H

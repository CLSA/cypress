/********************************************************************************
** Form generated from reading UI file 'retinal_camera_settings_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RETINAL_CAMERA_SETTINGS_WIDGET_H
#define UI_RETINAL_CAMERA_SETTINGS_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "widgets/file_picker_widget.h"

QT_BEGIN_NAMESPACE

class Ui_RetinalCameraSettingsWidget
{
public:
    QVBoxLayout *verticalLayout;
    FilePickerWidget *runnableName;
    FilePickerWidget *runnablePath;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *RetinalCameraSettingsWidget)
    {
        if (RetinalCameraSettingsWidget->objectName().isEmpty())
            RetinalCameraSettingsWidget->setObjectName(QString::fromUtf8("RetinalCameraSettingsWidget"));
        RetinalCameraSettingsWidget->resize(400, 300);
        verticalLayout = new QVBoxLayout(RetinalCameraSettingsWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        runnableName = new FilePickerWidget(RetinalCameraSettingsWidget);
        runnableName->setObjectName(QString::fromUtf8("runnableName"));

        verticalLayout->addWidget(runnableName);

        runnablePath = new FilePickerWidget(RetinalCameraSettingsWidget);
        runnablePath->setObjectName(QString::fromUtf8("runnablePath"));

        verticalLayout->addWidget(runnablePath);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(RetinalCameraSettingsWidget);

        QMetaObject::connectSlotsByName(RetinalCameraSettingsWidget);
    } // setupUi

    void retranslateUi(QWidget *RetinalCameraSettingsWidget)
    {
        RetinalCameraSettingsWidget->setWindowTitle(QCoreApplication::translate("RetinalCameraSettingsWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RetinalCameraSettingsWidget: public Ui_RetinalCameraSettingsWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RETINAL_CAMERA_SETTINGS_WIDGET_H

/********************************************************************************
** Form generated from reading UI file 'vivid_i_settings_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIVID_I_SETTINGS_WIDGET_H
#define UI_VIVID_I_SETTINGS_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "widgets/file_picker_widget.h"

QT_BEGIN_NAMESPACE

class Ui_VividISettingsWidget
{
public:
    QVBoxLayout *verticalLayout;
    FilePickerWidget *runnableName;
    FilePickerWidget *runnablePath;
    FilePickerWidget *storagePath;
    FilePickerWidget *logConfigPath;
    FilePickerWidget *ascConfigPath;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *VividISettingsWidget)
    {
        if (VividISettingsWidget->objectName().isEmpty())
            VividISettingsWidget->setObjectName(QString::fromUtf8("VividISettingsWidget"));
        VividISettingsWidget->resize(400, 300);
        verticalLayout = new QVBoxLayout(VividISettingsWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        runnableName = new FilePickerWidget(VividISettingsWidget);
        runnableName->setObjectName(QString::fromUtf8("runnableName"));

        verticalLayout->addWidget(runnableName);

        runnablePath = new FilePickerWidget(VividISettingsWidget);
        runnablePath->setObjectName(QString::fromUtf8("runnablePath"));

        verticalLayout->addWidget(runnablePath);

        storagePath = new FilePickerWidget(VividISettingsWidget);
        storagePath->setObjectName(QString::fromUtf8("storagePath"));

        verticalLayout->addWidget(storagePath);

        logConfigPath = new FilePickerWidget(VividISettingsWidget);
        logConfigPath->setObjectName(QString::fromUtf8("logConfigPath"));

        verticalLayout->addWidget(logConfigPath);

        ascConfigPath = new FilePickerWidget(VividISettingsWidget);
        ascConfigPath->setObjectName(QString::fromUtf8("ascConfigPath"));

        verticalLayout->addWidget(ascConfigPath);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(VividISettingsWidget);

        QMetaObject::connectSlotsByName(VividISettingsWidget);
    } // setupUi

    void retranslateUi(QWidget *VividISettingsWidget)
    {
        VividISettingsWidget->setWindowTitle(QCoreApplication::translate("VividISettingsWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class VividISettingsWidget: public Ui_VividISettingsWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIVID_I_SETTINGS_WIDGET_H

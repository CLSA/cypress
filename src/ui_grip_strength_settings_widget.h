/********************************************************************************
** Form generated from reading UI file 'grip_strength_settings_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRIP_STRENGTH_SETTINGS_WIDGET_H
#define UI_GRIP_STRENGTH_SETTINGS_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "widgets/file_picker_widget.h"

QT_BEGIN_NAMESPACE

class Ui_GripStrengthSettingsWidget
{
public:
    QVBoxLayout *verticalLayout;
    FilePickerWidget *runnablePath;
    FilePickerWidget *runnableName;
    FilePickerWidget *backupPath;
    FilePickerWidget *databasePath;
    FilePickerWidget *gripTestDB;
    FilePickerWidget *gripTestDataDB;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *GripStrengthSettingsWidget)
    {
        if (GripStrengthSettingsWidget->objectName().isEmpty())
            GripStrengthSettingsWidget->setObjectName(QString::fromUtf8("GripStrengthSettingsWidget"));
        GripStrengthSettingsWidget->resize(400, 300);
        verticalLayout = new QVBoxLayout(GripStrengthSettingsWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        runnablePath = new FilePickerWidget(GripStrengthSettingsWidget);
        runnablePath->setObjectName(QString::fromUtf8("runnablePath"));

        verticalLayout->addWidget(runnablePath);

        runnableName = new FilePickerWidget(GripStrengthSettingsWidget);
        runnableName->setObjectName(QString::fromUtf8("runnableName"));

        verticalLayout->addWidget(runnableName);

        backupPath = new FilePickerWidget(GripStrengthSettingsWidget);
        backupPath->setObjectName(QString::fromUtf8("backupPath"));

        verticalLayout->addWidget(backupPath);

        databasePath = new FilePickerWidget(GripStrengthSettingsWidget);
        databasePath->setObjectName(QString::fromUtf8("databasePath"));

        verticalLayout->addWidget(databasePath);

        gripTestDB = new FilePickerWidget(GripStrengthSettingsWidget);
        gripTestDB->setObjectName(QString::fromUtf8("gripTestDB"));

        verticalLayout->addWidget(gripTestDB);

        gripTestDataDB = new FilePickerWidget(GripStrengthSettingsWidget);
        gripTestDataDB->setObjectName(QString::fromUtf8("gripTestDataDB"));

        verticalLayout->addWidget(gripTestDataDB);

        verticalSpacer = new QSpacerItem(20, 183, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(GripStrengthSettingsWidget);

        QMetaObject::connectSlotsByName(GripStrengthSettingsWidget);
    } // setupUi

    void retranslateUi(QWidget *GripStrengthSettingsWidget)
    {
        GripStrengthSettingsWidget->setWindowTitle(QCoreApplication::translate("GripStrengthSettingsWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GripStrengthSettingsWidget: public Ui_GripStrengthSettingsWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRIP_STRENGTH_SETTINGS_WIDGET_H

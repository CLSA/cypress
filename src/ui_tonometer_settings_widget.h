/********************************************************************************
** Form generated from reading UI file 'tonometer_settings_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TONOMETER_SETTINGS_WIDGET_H
#define UI_TONOMETER_SETTINGS_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "widgets/file_picker_widget.h"

QT_BEGIN_NAMESPACE

class Ui_TonometerSettingsWidget
{
public:
    QVBoxLayout *verticalLayout;
    FilePickerWidget *runnablePath;
    FilePickerWidget *databasePath;
    FilePickerWidget *temporaryPath;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *TonometerSettingsWidget)
    {
        if (TonometerSettingsWidget->objectName().isEmpty())
            TonometerSettingsWidget->setObjectName(QString::fromUtf8("TonometerSettingsWidget"));
        TonometerSettingsWidget->resize(400, 300);
        verticalLayout = new QVBoxLayout(TonometerSettingsWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        runnablePath = new FilePickerWidget(TonometerSettingsWidget);
        runnablePath->setObjectName(QString::fromUtf8("runnablePath"));

        verticalLayout->addWidget(runnablePath);

        databasePath = new FilePickerWidget(TonometerSettingsWidget);
        databasePath->setObjectName(QString::fromUtf8("databasePath"));

        verticalLayout->addWidget(databasePath);

        temporaryPath = new FilePickerWidget(TonometerSettingsWidget);
        temporaryPath->setObjectName(QString::fromUtf8("temporaryPath"));

        verticalLayout->addWidget(temporaryPath);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(TonometerSettingsWidget);

        QMetaObject::connectSlotsByName(TonometerSettingsWidget);
    } // setupUi

    void retranslateUi(QWidget *TonometerSettingsWidget)
    {
        TonometerSettingsWidget->setWindowTitle(QCoreApplication::translate("TonometerSettingsWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TonometerSettingsWidget: public Ui_TonometerSettingsWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TONOMETER_SETTINGS_WIDGET_H

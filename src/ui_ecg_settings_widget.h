/********************************************************************************
** Form generated from reading UI file 'ecg_settings_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ECG_SETTINGS_WIDGET_H
#define UI_ECG_SETTINGS_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "widgets/file_picker_widget.h"

QT_BEGIN_NAMESPACE

class Ui_ECGSettingsWidget
{
public:
    QVBoxLayout *verticalLayout;
    FilePickerWidget *runnableName;
    FilePickerWidget *workingPath;
    FilePickerWidget *exportPath;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *ECGSettingsWidget)
    {
        if (ECGSettingsWidget->objectName().isEmpty())
            ECGSettingsWidget->setObjectName(QString::fromUtf8("ECGSettingsWidget"));
        ECGSettingsWidget->resize(400, 300);
        verticalLayout = new QVBoxLayout(ECGSettingsWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        runnableName = new FilePickerWidget(ECGSettingsWidget);
        runnableName->setObjectName(QString::fromUtf8("runnableName"));

        verticalLayout->addWidget(runnableName);

        workingPath = new FilePickerWidget(ECGSettingsWidget);
        workingPath->setObjectName(QString::fromUtf8("workingPath"));

        verticalLayout->addWidget(workingPath);

        exportPath = new FilePickerWidget(ECGSettingsWidget);
        exportPath->setObjectName(QString::fromUtf8("exportPath"));

        verticalLayout->addWidget(exportPath);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(ECGSettingsWidget);

        QMetaObject::connectSlotsByName(ECGSettingsWidget);
    } // setupUi

    void retranslateUi(QWidget *ECGSettingsWidget)
    {
        ECGSettingsWidget->setWindowTitle(QCoreApplication::translate("ECGSettingsWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ECGSettingsWidget: public Ui_ECGSettingsWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ECG_SETTINGS_WIDGET_H

/********************************************************************************
** Form generated from reading UI file 'cdtt_settings_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CDTT_SETTINGS_WIDGET_H
#define UI_CDTT_SETTINGS_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "widgets/file_picker_widget.h"

QT_BEGIN_NAMESPACE

class Ui_CDTTSettingsWidget
{
public:
    QVBoxLayout *verticalLayout;
    FilePickerWidget *runnableName;
    FilePickerWidget *runnablePath;
    FilePickerWidget *outputPath;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *CDTTSettingsWidget)
    {
        if (CDTTSettingsWidget->objectName().isEmpty())
            CDTTSettingsWidget->setObjectName(QString::fromUtf8("CDTTSettingsWidget"));
        CDTTSettingsWidget->resize(400, 300);
        verticalLayout = new QVBoxLayout(CDTTSettingsWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        runnableName = new FilePickerWidget(CDTTSettingsWidget);
        runnableName->setObjectName(QString::fromUtf8("runnableName"));

        verticalLayout->addWidget(runnableName);

        runnablePath = new FilePickerWidget(CDTTSettingsWidget);
        runnablePath->setObjectName(QString::fromUtf8("runnablePath"));

        verticalLayout->addWidget(runnablePath);

        outputPath = new FilePickerWidget(CDTTSettingsWidget);
        outputPath->setObjectName(QString::fromUtf8("outputPath"));

        verticalLayout->addWidget(outputPath);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(CDTTSettingsWidget);

        QMetaObject::connectSlotsByName(CDTTSettingsWidget);
    } // setupUi

    void retranslateUi(QWidget *CDTTSettingsWidget)
    {
        CDTTSettingsWidget->setWindowTitle(QCoreApplication::translate("CDTTSettingsWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CDTTSettingsWidget: public Ui_CDTTSettingsWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CDTT_SETTINGS_WIDGET_H

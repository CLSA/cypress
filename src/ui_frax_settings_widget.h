/********************************************************************************
** Form generated from reading UI file 'frax_settings_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FRAX_SETTINGS_WIDGET_H
#define UI_FRAX_SETTINGS_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "widgets/file_picker_widget.h"

QT_BEGIN_NAMESPACE

class Ui_FraxSettingsWidget
{
public:
    QVBoxLayout *verticalLayout;
    FilePickerWidget *runnableName;
    FilePickerWidget *runnablePath;
    FilePickerWidget *outputFilePath;
    FilePickerWidget *inputFilePath;
    FilePickerWidget *temporaryFilePath;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *FraxSettingsWidget)
    {
        if (FraxSettingsWidget->objectName().isEmpty())
            FraxSettingsWidget->setObjectName(QString::fromUtf8("FraxSettingsWidget"));
        FraxSettingsWidget->resize(400, 300);
        verticalLayout = new QVBoxLayout(FraxSettingsWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        runnableName = new FilePickerWidget(FraxSettingsWidget);
        runnableName->setObjectName(QString::fromUtf8("runnableName"));

        verticalLayout->addWidget(runnableName);

        runnablePath = new FilePickerWidget(FraxSettingsWidget);
        runnablePath->setObjectName(QString::fromUtf8("runnablePath"));

        verticalLayout->addWidget(runnablePath);

        outputFilePath = new FilePickerWidget(FraxSettingsWidget);
        outputFilePath->setObjectName(QString::fromUtf8("outputFilePath"));

        verticalLayout->addWidget(outputFilePath);

        inputFilePath = new FilePickerWidget(FraxSettingsWidget);
        inputFilePath->setObjectName(QString::fromUtf8("inputFilePath"));

        verticalLayout->addWidget(inputFilePath);

        temporaryFilePath = new FilePickerWidget(FraxSettingsWidget);
        temporaryFilePath->setObjectName(QString::fromUtf8("temporaryFilePath"));

        verticalLayout->addWidget(temporaryFilePath);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(FraxSettingsWidget);

        QMetaObject::connectSlotsByName(FraxSettingsWidget);
    } // setupUi

    void retranslateUi(QWidget *FraxSettingsWidget)
    {
        FraxSettingsWidget->setWindowTitle(QCoreApplication::translate("FraxSettingsWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FraxSettingsWidget: public Ui_FraxSettingsWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRAX_SETTINGS_WIDGET_H

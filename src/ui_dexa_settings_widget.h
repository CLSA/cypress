/********************************************************************************
** Form generated from reading UI file 'dexa_settings_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DEXA_SETTINGS_WIDGET_H
#define UI_DEXA_SETTINGS_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "widgets/file_picker_widget.h"

QT_BEGIN_NAMESPACE

class Ui_DexaSettingsWidget
{
public:
    QVBoxLayout *verticalLayout;
    FilePickerWidget *runnableName;
    FilePickerWidget *runnablePath;
    FilePickerWidget *logConfigPath;
    FilePickerWidget *ascConfigPath;
    FilePickerWidget *storagePath;
    FilePickerWidget *patscanDbPath;
    FilePickerWidget *refscanDbPath;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *DexaSettingsWidget)
    {
        if (DexaSettingsWidget->objectName().isEmpty())
            DexaSettingsWidget->setObjectName(QString::fromUtf8("DexaSettingsWidget"));
        DexaSettingsWidget->resize(400, 300);
        verticalLayout = new QVBoxLayout(DexaSettingsWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        runnableName = new FilePickerWidget(DexaSettingsWidget);
        runnableName->setObjectName(QString::fromUtf8("runnableName"));

        verticalLayout->addWidget(runnableName);

        runnablePath = new FilePickerWidget(DexaSettingsWidget);
        runnablePath->setObjectName(QString::fromUtf8("runnablePath"));

        verticalLayout->addWidget(runnablePath);

        logConfigPath = new FilePickerWidget(DexaSettingsWidget);
        logConfigPath->setObjectName(QString::fromUtf8("logConfigPath"));

        verticalLayout->addWidget(logConfigPath);

        ascConfigPath = new FilePickerWidget(DexaSettingsWidget);
        ascConfigPath->setObjectName(QString::fromUtf8("ascConfigPath"));

        verticalLayout->addWidget(ascConfigPath);

        storagePath = new FilePickerWidget(DexaSettingsWidget);
        storagePath->setObjectName(QString::fromUtf8("storagePath"));

        verticalLayout->addWidget(storagePath);

        patscanDbPath = new FilePickerWidget(DexaSettingsWidget);
        patscanDbPath->setObjectName(QString::fromUtf8("patscanDbPath"));

        verticalLayout->addWidget(patscanDbPath);

        refscanDbPath = new FilePickerWidget(DexaSettingsWidget);
        refscanDbPath->setObjectName(QString::fromUtf8("refscanDbPath"));

        verticalLayout->addWidget(refscanDbPath);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(DexaSettingsWidget);

        QMetaObject::connectSlotsByName(DexaSettingsWidget);
    } // setupUi

    void retranslateUi(QWidget *DexaSettingsWidget)
    {
        DexaSettingsWidget->setWindowTitle(QCoreApplication::translate("DexaSettingsWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DexaSettingsWidget: public Ui_DexaSettingsWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DEXA_SETTINGS_WIDGET_H

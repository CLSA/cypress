/********************************************************************************
** Form generated from reading UI file 'spirometer_settings_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SPIROMETER_SETTINGS_WIDGET_H
#define UI_SPIROMETER_SETTINGS_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "widgets/file_picker_widget.h"

QT_BEGIN_NAMESPACE

class Ui_SpirometerSettingsWidget
{
public:
    QVBoxLayout *verticalLayout;
    FilePickerWidget *runnableName;
    FilePickerWidget *runnablePath;
    FilePickerWidget *dataPath;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *SpirometerSettingsWidget)
    {
        if (SpirometerSettingsWidget->objectName().isEmpty())
            SpirometerSettingsWidget->setObjectName(QString::fromUtf8("SpirometerSettingsWidget"));
        SpirometerSettingsWidget->resize(400, 300);
        verticalLayout = new QVBoxLayout(SpirometerSettingsWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        runnableName = new FilePickerWidget(SpirometerSettingsWidget);
        runnableName->setObjectName(QString::fromUtf8("runnableName"));

        verticalLayout->addWidget(runnableName);

        runnablePath = new FilePickerWidget(SpirometerSettingsWidget);
        runnablePath->setObjectName(QString::fromUtf8("runnablePath"));

        verticalLayout->addWidget(runnablePath);

        dataPath = new FilePickerWidget(SpirometerSettingsWidget);
        dataPath->setObjectName(QString::fromUtf8("dataPath"));

        verticalLayout->addWidget(dataPath);

        verticalSpacer = new QSpacerItem(20, 231, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(SpirometerSettingsWidget);

        QMetaObject::connectSlotsByName(SpirometerSettingsWidget);
    } // setupUi

    void retranslateUi(QWidget *SpirometerSettingsWidget)
    {
        SpirometerSettingsWidget->setWindowTitle(QCoreApplication::translate("SpirometerSettingsWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SpirometerSettingsWidget: public Ui_SpirometerSettingsWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SPIROMETER_SETTINGS_WIDGET_H

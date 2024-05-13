/********************************************************************************
** Form generated from reading UI file 'audiometer_settings_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AUDIOMETER_SETTINGS_WIDGET_H
#define UI_AUDIOMETER_SETTINGS_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AudiometerSettingsWidget
{
public:
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *AudiometerSettingsWidget)
    {
        if (AudiometerSettingsWidget->objectName().isEmpty())
            AudiometerSettingsWidget->setObjectName(QString::fromUtf8("AudiometerSettingsWidget"));
        AudiometerSettingsWidget->resize(400, 300);
        verticalLayout = new QVBoxLayout(AudiometerSettingsWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(AudiometerSettingsWidget);

        QMetaObject::connectSlotsByName(AudiometerSettingsWidget);
    } // setupUi

    void retranslateUi(QWidget *AudiometerSettingsWidget)
    {
        AudiometerSettingsWidget->setWindowTitle(QCoreApplication::translate("AudiometerSettingsWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AudiometerSettingsWidget: public Ui_AudiometerSettingsWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AUDIOMETER_SETTINGS_WIDGET_H

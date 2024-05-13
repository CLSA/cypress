/********************************************************************************
** Form generated from reading UI file 'body_composition_settings_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BODY_COMPOSITION_SETTINGS_WIDGET_H
#define UI_BODY_COMPOSITION_SETTINGS_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BodyCompositionSettingsWidget
{
public:
    QPushButton *pushButton;

    void setupUi(QWidget *BodyCompositionSettingsWidget)
    {
        if (BodyCompositionSettingsWidget->objectName().isEmpty())
            BodyCompositionSettingsWidget->setObjectName(QString::fromUtf8("BodyCompositionSettingsWidget"));
        BodyCompositionSettingsWidget->resize(400, 300);
        pushButton = new QPushButton(BodyCompositionSettingsWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(60, 90, 80, 24));

        retranslateUi(BodyCompositionSettingsWidget);

        QMetaObject::connectSlotsByName(BodyCompositionSettingsWidget);
    } // setupUi

    void retranslateUi(QWidget *BodyCompositionSettingsWidget)
    {
        BodyCompositionSettingsWidget->setWindowTitle(QCoreApplication::translate("BodyCompositionSettingsWidget", "Form", nullptr));
        pushButton->setText(QCoreApplication::translate("BodyCompositionSettingsWidget", "Body Composition", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BodyCompositionSettingsWidget: public Ui_BodyCompositionSettingsWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BODY_COMPOSITION_SETTINGS_WIDGET_H

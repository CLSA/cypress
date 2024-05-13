/********************************************************************************
** Form generated from reading UI file 'choice_reaction_settings_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHOICE_REACTION_SETTINGS_WIDGET_H
#define UI_CHOICE_REACTION_SETTINGS_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "widgets/file_picker_widget.h"

QT_BEGIN_NAMESPACE

class Ui_ChoiceReactionSettingsWidget
{
public:
    QVBoxLayout *verticalLayout;
    FilePickerWidget *runnablePath;
    FilePickerWidget *runnableName;
    FilePickerWidget *outputPath;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *ChoiceReactionSettingsWidget)
    {
        if (ChoiceReactionSettingsWidget->objectName().isEmpty())
            ChoiceReactionSettingsWidget->setObjectName(QString::fromUtf8("ChoiceReactionSettingsWidget"));
        ChoiceReactionSettingsWidget->resize(400, 300);
        verticalLayout = new QVBoxLayout(ChoiceReactionSettingsWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        runnablePath = new FilePickerWidget(ChoiceReactionSettingsWidget);
        runnablePath->setObjectName(QString::fromUtf8("runnablePath"));

        verticalLayout->addWidget(runnablePath);

        runnableName = new FilePickerWidget(ChoiceReactionSettingsWidget);
        runnableName->setObjectName(QString::fromUtf8("runnableName"));

        verticalLayout->addWidget(runnableName);

        outputPath = new FilePickerWidget(ChoiceReactionSettingsWidget);
        outputPath->setObjectName(QString::fromUtf8("outputPath"));

        verticalLayout->addWidget(outputPath);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(ChoiceReactionSettingsWidget);

        QMetaObject::connectSlotsByName(ChoiceReactionSettingsWidget);
    } // setupUi

    void retranslateUi(QWidget *ChoiceReactionSettingsWidget)
    {
        ChoiceReactionSettingsWidget->setWindowTitle(QCoreApplication::translate("ChoiceReactionSettingsWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChoiceReactionSettingsWidget: public Ui_ChoiceReactionSettingsWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHOICE_REACTION_SETTINGS_WIDGET_H

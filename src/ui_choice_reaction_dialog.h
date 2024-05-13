/********************************************************************************
** Form generated from reading UI file 'choice_reaction_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHOICE_REACTION_DIALOG_H
#define UI_CHOICE_REACTION_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include "widgets/test_info_widget.h"

QT_BEGIN_NAMESPACE

class Ui_ChoiceReactionDialog
{
public:
    QVBoxLayout *verticalLayout;
    TestInfoWidget *testInfoWidget;
    QSpacerItem *verticalSpacer;

    void setupUi(QDialog *ChoiceReactionDialog)
    {
        if (ChoiceReactionDialog->objectName().isEmpty())
            ChoiceReactionDialog->setObjectName(QString::fromUtf8("ChoiceReactionDialog"));
        ChoiceReactionDialog->resize(400, 34);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ChoiceReactionDialog->sizePolicy().hasHeightForWidth());
        ChoiceReactionDialog->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(ChoiceReactionDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        testInfoWidget = new TestInfoWidget(ChoiceReactionDialog);
        testInfoWidget->setObjectName(QString::fromUtf8("testInfoWidget"));

        verticalLayout->addWidget(testInfoWidget);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(ChoiceReactionDialog);

        QMetaObject::connectSlotsByName(ChoiceReactionDialog);
    } // setupUi

    void retranslateUi(QDialog *ChoiceReactionDialog)
    {
        ChoiceReactionDialog->setWindowTitle(QCoreApplication::translate("ChoiceReactionDialog", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChoiceReactionDialog: public Ui_ChoiceReactionDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHOICE_REACTION_DIALOG_H

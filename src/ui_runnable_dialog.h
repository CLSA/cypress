/********************************************************************************
** Form generated from reading UI file 'runnable_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RUNNABLE_DIALOG_H
#define UI_RUNNABLE_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_RunnableDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QGroupBox *pathGroupBox;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *openButton;
    QSpacerItem *horizontalSpacer_2;
    QStatusBar *statusBar;

    void setupUi(QDialog *RunnableDialog)
    {
        if (RunnableDialog->objectName().isEmpty())
            RunnableDialog->setObjectName(QString::fromUtf8("RunnableDialog"));
        RunnableDialog->resize(600, 389);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(RunnableDialog->sizePolicy().hasHeightForWidth());
        RunnableDialog->setSizePolicy(sizePolicy);
        verticalLayout_2 = new QVBoxLayout(RunnableDialog);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        pathGroupBox = new QGroupBox(RunnableDialog);
        pathGroupBox->setObjectName(QString::fromUtf8("pathGroupBox"));
        sizePolicy.setHeightForWidth(pathGroupBox->sizePolicy().hasHeightForWidth());
        pathGroupBox->setSizePolicy(sizePolicy);
        horizontalLayout_2 = new QHBoxLayout(pathGroupBox);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        openButton = new QPushButton(pathGroupBox);
        openButton->setObjectName(QString::fromUtf8("openButton"));

        horizontalLayout_2->addWidget(openButton);

        horizontalSpacer_2 = new QSpacerItem(469, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        verticalLayout_2->addWidget(pathGroupBox);

        statusBar = new QStatusBar(RunnableDialog);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        statusBar->setSizeGripEnabled(false);

        verticalLayout_2->addWidget(statusBar);


        retranslateUi(RunnableDialog);

        QMetaObject::connectSlotsByName(RunnableDialog);
    } // setupUi

    void retranslateUi(QDialog *RunnableDialog)
    {
        RunnableDialog->setWindowTitle(QString());
        pathGroupBox->setTitle(QCoreApplication::translate("RunnableDialog", "Executable", nullptr));
        openButton->setText(QCoreApplication::translate("RunnableDialog", "Open", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RunnableDialog: public Ui_RunnableDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RUNNABLE_DIALOG_H

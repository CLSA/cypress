#pragma once
/********************************************************************************
** Form generated from reading UI file ''
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include "widgets/measure_widget.h"

QT_BEGIN_NAMESPACE

class Ui_FraxDialog
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_7;
    QLabel *participantId;
    QGroupBox *group;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *completeButton;
    QPushButton *measureButton;
    QPushButton *submitButton;
    MeasureWidget *measureWidget;

    void setupUi(QDialog *FraxDialog)
    {
        if (FraxDialog->objectName().isEmpty())
            FraxDialog->setObjectName(QString::fromUtf8("FraxDialog"));
        FraxDialog->resize(400, 300);
        verticalLayout = new QVBoxLayout(FraxDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox = new QGroupBox(FraxDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout_5 = new QVBoxLayout(groupBox);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        label_7 = new QLabel(groupBox);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_7->sizePolicy().hasHeightForWidth());
        label_7->setSizePolicy(sizePolicy);

        horizontalLayout_10->addWidget(label_7);

        participantId = new QLabel(groupBox);
        participantId->setObjectName(QString::fromUtf8("participantId"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(participantId->sizePolicy().hasHeightForWidth());
        participantId->setSizePolicy(sizePolicy1);

        horizontalLayout_10->addWidget(participantId);


        verticalLayout_5->addLayout(horizontalLayout_10);


        verticalLayout->addWidget(groupBox);

        group = new QGroupBox(FraxDialog);
        group->setObjectName(QString::fromUtf8("group"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(group->sizePolicy().hasHeightForWidth());
        group->setSizePolicy(sizePolicy2);
        horizontalLayout = new QHBoxLayout(group);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        completeButton = new QPushButton(group);
        completeButton->setObjectName(QString::fromUtf8("completeButton"));

        horizontalLayout->addWidget(completeButton);

        measureButton = new QPushButton(group);
        measureButton->setObjectName(QString::fromUtf8("measureButton"));
        measureButton->setEnabled(false);

        horizontalLayout->addWidget(measureButton);

        submitButton = new QPushButton(group);
        submitButton->setObjectName(QString::fromUtf8("submitButton"));
        submitButton->setEnabled(false);

        horizontalLayout->addWidget(submitButton);


        verticalLayout->addWidget(group);

        measureWidget = new MeasureWidget(FraxDialog);
        measureWidget->setObjectName(QString::fromUtf8("widget"));

        verticalLayout->addWidget(measureWidget);

        retranslateUi(FraxDialog);

        QMetaObject::connectSlotsByName(FraxDialog);
    } // setupUi

    void retranslateUi(QDialog *FraxDialog)
    {
        FraxDialog->setWindowTitle(QCoreApplication::translate("FraxDialog", "Dialog", nullptr));
        groupBox->setTitle(QString());
        label_7->setText(QCoreApplication::translate("FraxDialog", "Participant ID:", nullptr));
        participantId->setText(QString());
        group->setTitle(QString());
        completeButton->setText(QCoreApplication::translate("FraxDialog", "Complete", nullptr));
        measureButton->setText(QCoreApplication::translate("FraxDialog", "Measure", nullptr));
        submitButton->setText(QCoreApplication::translate("FraxDialog", "Submit", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FraxDialog: public Ui_FraxDialog {};
} // namespace Ui

QT_END_NAMESPACE


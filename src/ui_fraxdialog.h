/********************************************************************************
** Form generated from reading UI file 'fraxdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FRAXDIALOG_H
#define UI_FRAXDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_7;
    QLabel *participantId;
    QGroupBox *controlsBox;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *measureButton;
    QPushButton *submitButton;
    QGroupBox *groupBox_2;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(400, 300);
        verticalLayout = new QVBoxLayout(Dialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox = new QGroupBox(Dialog);
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

        controlsBox = new QGroupBox(Dialog);
        controlsBox->setObjectName(QString::fromUtf8("controlsBox"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(controlsBox->sizePolicy().hasHeightForWidth());
        controlsBox->setSizePolicy(sizePolicy2);
        horizontalLayout = new QHBoxLayout(controlsBox);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        measureButton = new QPushButton(controlsBox);
        measureButton->setObjectName(QString::fromUtf8("measureButton"));
        measureButton->setEnabled(false);

        horizontalLayout->addWidget(measureButton);

        submitButton = new QPushButton(controlsBox);
        submitButton->setObjectName(QString::fromUtf8("submitButton"));
        submitButton->setEnabled(false);

        horizontalLayout->addWidget(submitButton);


        verticalLayout->addWidget(controlsBox);

        groupBox_2 = new QGroupBox(Dialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy3);

        verticalLayout->addWidget(groupBox_2);


        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QCoreApplication::translate("Dialog", "Dialog", nullptr));
        groupBox->setTitle(QString());
        label_7->setText(QCoreApplication::translate("Dialog", "Participant ID:", nullptr));
        participantId->setText(QString());
        controlsBox->setTitle(QString());
        measureButton->setText(QCoreApplication::translate("Dialog", "Measure", nullptr));
        submitButton->setText(QCoreApplication::translate("Dialog", "Submit", nullptr));
        groupBox_2->setTitle(QString());
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRAXDIALOG_H

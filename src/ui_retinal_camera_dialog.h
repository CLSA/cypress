/********************************************************************************
** Form generated from reading UI file 'retinal_camera_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RETINAL_CAMERA_DIALOG_H
#define UI_RETINAL_CAMERA_DIALOG_H

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

class Ui_RetinalCameraDialog
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_7;
    QLabel *participantId;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *completeButton;
    QPushButton *measureButton;
    QPushButton *submitButton;
    QGroupBox *groupBox;

    void setupUi(QDialog *RetinalCameraDialog)
    {
        if (RetinalCameraDialog->objectName().isEmpty())
            RetinalCameraDialog->setObjectName(QString::fromUtf8("RetinalCameraDialog"));
        RetinalCameraDialog->resize(400, 300);
        verticalLayout = new QVBoxLayout(RetinalCameraDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox_3 = new QGroupBox(RetinalCameraDialog);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        verticalLayout_5 = new QVBoxLayout(groupBox_3);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        label_7 = new QLabel(groupBox_3);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_7->sizePolicy().hasHeightForWidth());
        label_7->setSizePolicy(sizePolicy);

        horizontalLayout_10->addWidget(label_7);

        participantId = new QLabel(groupBox_3);
        participantId->setObjectName(QString::fromUtf8("participantId"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(participantId->sizePolicy().hasHeightForWidth());
        participantId->setSizePolicy(sizePolicy1);

        horizontalLayout_10->addWidget(participantId);


        verticalLayout_5->addLayout(horizontalLayout_10);


        verticalLayout->addWidget(groupBox_3);

        groupBox_2 = new QGroupBox(RetinalCameraDialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy2);
        horizontalLayout = new QHBoxLayout(groupBox_2);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        completeButton = new QPushButton(groupBox_2);
        completeButton->setObjectName(QString::fromUtf8("completeButton"));

        horizontalLayout->addWidget(completeButton);

        measureButton = new QPushButton(groupBox_2);
        measureButton->setObjectName(QString::fromUtf8("measureButton"));
        measureButton->setEnabled(false);

        horizontalLayout->addWidget(measureButton);

        submitButton = new QPushButton(groupBox_2);
        submitButton->setObjectName(QString::fromUtf8("submitButton"));
        submitButton->setEnabled(false);

        horizontalLayout->addWidget(submitButton);


        verticalLayout->addWidget(groupBox_2);

        groupBox = new QGroupBox(RetinalCameraDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy3);

        verticalLayout->addWidget(groupBox);


        retranslateUi(RetinalCameraDialog);

        QMetaObject::connectSlotsByName(RetinalCameraDialog);
    } // setupUi

    void retranslateUi(QDialog *RetinalCameraDialog)
    {
        RetinalCameraDialog->setWindowTitle(QCoreApplication::translate("RetinalCameraDialog", "Retinal Camera", nullptr));
        groupBox_3->setTitle(QString());
        label_7->setText(QCoreApplication::translate("RetinalCameraDialog", "Participant ID:", nullptr));
        participantId->setText(QString());
        groupBox_2->setTitle(QString());
        completeButton->setText(QCoreApplication::translate("RetinalCameraDialog", "Complete", nullptr));
        measureButton->setText(QCoreApplication::translate("RetinalCameraDialog", "Measure", nullptr));
        submitButton->setText(QCoreApplication::translate("RetinalCameraDialog", "Submit", nullptr));
        groupBox->setTitle(QString());
    } // retranslateUi

};

namespace Ui {
    class RetinalCameraDialog: public Ui_RetinalCameraDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RETINAL_CAMERA_DIALOG_H

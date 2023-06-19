/********************************************************************************
** Form generated from reading UI file 'signature_pad_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIGNATURE_PAD_DIALOG_H
#define UI_SIGNATURE_PAD_DIALOG_H

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

class Ui_SignaturePadDialog
{
public:
    QVBoxLayout *verticalLayout_3;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_7;
    QLabel *participantId;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *completeButton;
    QPushButton *ResetButton;
    QPushButton *SubmitButton;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *label;

    void setupUi(QDialog *SignaturePadDialog)
    {
        if (SignaturePadDialog->objectName().isEmpty())
            SignaturePadDialog->setObjectName(QString::fromUtf8("SignaturePadDialog"));
        SignaturePadDialog->resize(581, 328);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(SignaturePadDialog->sizePolicy().hasHeightForWidth());
        SignaturePadDialog->setSizePolicy(sizePolicy);
        SignaturePadDialog->setMinimumSize(QSize(0, 0));
        verticalLayout_3 = new QVBoxLayout(SignaturePadDialog);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        groupBox_3 = new QGroupBox(SignaturePadDialog);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        verticalLayout_5 = new QVBoxLayout(groupBox_3);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        label_7 = new QLabel(groupBox_3);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_7->sizePolicy().hasHeightForWidth());
        label_7->setSizePolicy(sizePolicy1);

        horizontalLayout_10->addWidget(label_7);

        participantId = new QLabel(groupBox_3);
        participantId->setObjectName(QString::fromUtf8("participantId"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(participantId->sizePolicy().hasHeightForWidth());
        participantId->setSizePolicy(sizePolicy2);

        horizontalLayout_10->addWidget(participantId);


        verticalLayout_5->addLayout(horizontalLayout_10);


        verticalLayout_3->addWidget(groupBox_3);

        groupBox = new QGroupBox(SignaturePadDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy3);
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        horizontalSpacer = new QSpacerItem(353, 17, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        completeButton = new QPushButton(groupBox);
        completeButton->setObjectName(QString::fromUtf8("completeButton"));

        horizontalLayout_2->addWidget(completeButton);

        ResetButton = new QPushButton(groupBox);
        ResetButton->setObjectName(QString::fromUtf8("ResetButton"));
        ResetButton->setEnabled(true);

        horizontalLayout_2->addWidget(ResetButton);

        SubmitButton = new QPushButton(groupBox);
        SubmitButton->setObjectName(QString::fromUtf8("SubmitButton"));
        SubmitButton->setEnabled(false);

        horizontalLayout_2->addWidget(SubmitButton);


        verticalLayout->addLayout(horizontalLayout_2);


        verticalLayout_3->addWidget(groupBox);

        groupBox_2 = new QGroupBox(SignaturePadDialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy4);
        groupBox_2->setMinimumSize(QSize(400, 200));
        groupBox_2->setFlat(false);
        verticalLayout_2 = new QVBoxLayout(groupBox_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setSizeConstraint(QLayout::SetDefaultConstraint);
        label = new QLabel(groupBox_2);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label);


        verticalLayout_3->addWidget(groupBox_2);


        retranslateUi(SignaturePadDialog);

        QMetaObject::connectSlotsByName(SignaturePadDialog);
    } // setupUi

    void retranslateUi(QDialog *SignaturePadDialog)
    {
        SignaturePadDialog->setWindowTitle(QCoreApplication::translate("SignaturePadDialog", "Signature", nullptr));
#if QT_CONFIG(tooltip)
        SignaturePadDialog->setToolTip(QString());
#endif // QT_CONFIG(tooltip)
        groupBox_3->setTitle(QString());
        label_7->setText(QCoreApplication::translate("SignaturePadDialog", "Participant ID:", nullptr));
        participantId->setText(QString());
        groupBox->setTitle(QString());
        label_2->setText(QString());
        completeButton->setText(QCoreApplication::translate("SignaturePadDialog", "Complete", nullptr));
        ResetButton->setText(QCoreApplication::translate("SignaturePadDialog", "Reset", nullptr));
        SubmitButton->setText(QCoreApplication::translate("SignaturePadDialog", "Submit", nullptr));
        groupBox_2->setTitle(QString());
        label->setText(QCoreApplication::translate("SignaturePadDialog", "Signature capture in progress...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SignaturePadDialog: public Ui_SignaturePadDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIGNATURE_PAD_DIALOG_H

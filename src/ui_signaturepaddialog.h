/********************************************************************************
** Form generated from reading UI file 'signaturepaddialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIGNATUREPADDIALOG_H
#define UI_SIGNATUREPADDIALOG_H

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
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *ResetButton;
    QPushButton *SubmitButton;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *label;

    void setupUi(QDialog *SignaturePadDialog)
    {
        if (SignaturePadDialog->objectName().isEmpty())
            SignaturePadDialog->setObjectName(QString::fromUtf8("SignaturePadDialog"));
        SignaturePadDialog->resize(569, 277);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(SignaturePadDialog->sizePolicy().hasHeightForWidth());
        SignaturePadDialog->setSizePolicy(sizePolicy);
        SignaturePadDialog->setMinimumSize(QSize(0, 0));
        verticalLayout_3 = new QVBoxLayout(SignaturePadDialog);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setSizeConstraint(QLayout::SetFixedSize);
        groupBox = new QGroupBox(SignaturePadDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy1);
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        horizontalSpacer = new QSpacerItem(353, 17, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        ResetButton = new QPushButton(groupBox);
        ResetButton->setObjectName(QString::fromUtf8("ResetButton"));

        horizontalLayout_2->addWidget(ResetButton);

        SubmitButton = new QPushButton(groupBox);
        SubmitButton->setObjectName(QString::fromUtf8("SubmitButton"));

        horizontalLayout_2->addWidget(SubmitButton);


        verticalLayout->addLayout(horizontalLayout_2);


        verticalLayout_3->addWidget(groupBox);

        groupBox_2 = new QGroupBox(SignaturePadDialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy2);
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
        groupBox->setTitle(QString());
        label_2->setText(QString());
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

#endif // UI_SIGNATUREPADDIALOG_H

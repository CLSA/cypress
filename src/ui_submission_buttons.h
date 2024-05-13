/********************************************************************************
** Form generated from reading UI file 'submission_buttons.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SUBMISSION_BUTTONS_H
#define UI_SUBMISSION_BUTTONS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SubmissionButtons
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *submissionLayout;
    QPushButton *cancelButton;
    QSpacerItem *horizontalSpacer;
    QPushButton *finishButton;

    void setupUi(QWidget *SubmissionButtons)
    {
        if (SubmissionButtons->objectName().isEmpty())
            SubmissionButtons->setObjectName(QString::fromUtf8("SubmissionButtons"));
        SubmissionButtons->resize(400, 107);
        verticalLayout = new QVBoxLayout(SubmissionButtons);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        submissionLayout = new QHBoxLayout();
        submissionLayout->setObjectName(QString::fromUtf8("submissionLayout"));
        cancelButton = new QPushButton(SubmissionButtons);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        submissionLayout->addWidget(cancelButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        submissionLayout->addItem(horizontalSpacer);

        finishButton = new QPushButton(SubmissionButtons);
        finishButton->setObjectName(QString::fromUtf8("finishButton"));
        finishButton->setEnabled(false);

        submissionLayout->addWidget(finishButton);


        verticalLayout->addLayout(submissionLayout);


        retranslateUi(SubmissionButtons);

        QMetaObject::connectSlotsByName(SubmissionButtons);
    } // setupUi

    void retranslateUi(QWidget *SubmissionButtons)
    {
        SubmissionButtons->setWindowTitle(QCoreApplication::translate("SubmissionButtons", "Form", nullptr));
        cancelButton->setText(QCoreApplication::translate("SubmissionButtons", "Cancel", nullptr));
        finishButton->setText(QCoreApplication::translate("SubmissionButtons", "Finish", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SubmissionButtons: public Ui_SubmissionButtons {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SUBMISSION_BUTTONS_H

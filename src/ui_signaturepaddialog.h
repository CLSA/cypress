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
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_SignaturePadDialog
{
public:
    QVBoxLayout *verticalLayout;
    QGraphicsView *graphicsView;

    void setupUi(QDialog *SignaturePadDialog)
    {
        if (SignaturePadDialog->objectName().isEmpty())
            SignaturePadDialog->setObjectName(QString::fromUtf8("SignaturePadDialog"));
        SignaturePadDialog->resize(400, 300);
        verticalLayout = new QVBoxLayout(SignaturePadDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        graphicsView = new QGraphicsView(SignaturePadDialog);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));

        verticalLayout->addWidget(graphicsView);


        retranslateUi(SignaturePadDialog);

        QMetaObject::connectSlotsByName(SignaturePadDialog);
    } // setupUi

    void retranslateUi(QDialog *SignaturePadDialog)
    {
        SignaturePadDialog->setWindowTitle(QCoreApplication::translate("SignaturePadDialog", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SignaturePadDialog: public Ui_SignaturePadDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIGNATUREPADDIALOG_H

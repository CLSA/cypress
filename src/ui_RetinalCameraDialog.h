/********************************************************************************
** Form generated from reading UI file 'RetinalCameraDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RETINALCAMERADIALOG_H
#define UI_RETINALCAMERADIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_RetinalCameraDialog
{
public:
    QVBoxLayout *verticalLayout;

    void setupUi(QDialog *RetinalCameraDialog)
    {
        if (RetinalCameraDialog->objectName().isEmpty())
            RetinalCameraDialog->setObjectName(QString::fromUtf8("RetinalCameraDialog"));
        RetinalCameraDialog->resize(400, 300);
        verticalLayout = new QVBoxLayout(RetinalCameraDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));

        retranslateUi(RetinalCameraDialog);

        QMetaObject::connectSlotsByName(RetinalCameraDialog);
    } // setupUi

    void retranslateUi(QDialog *RetinalCameraDialog)
    {
        RetinalCameraDialog->setWindowTitle(QCoreApplication::translate("RetinalCameraDialog", "Retinal Camera", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RetinalCameraDialog: public Ui_RetinalCameraDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RETINALCAMERADIALOG_H

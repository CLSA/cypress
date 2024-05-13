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
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include "widgets/test_info_widget.h"

QT_BEGIN_NAMESPACE

class Ui_RetinalCameraDialog
{
public:
    QVBoxLayout *verticalLayout;
    TestInfoWidget *testInfoWidget;
    QSpacerItem *verticalSpacer;

    void setupUi(QDialog *RetinalCameraDialog)
    {
        if (RetinalCameraDialog->objectName().isEmpty())
            RetinalCameraDialog->setObjectName(QString::fromUtf8("RetinalCameraDialog"));
        RetinalCameraDialog->resize(400, 34);
        verticalLayout = new QVBoxLayout(RetinalCameraDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        testInfoWidget = new TestInfoWidget(RetinalCameraDialog);
        testInfoWidget->setObjectName(QString::fromUtf8("testInfoWidget"));

        verticalLayout->addWidget(testInfoWidget);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


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

#endif // UI_RETINAL_CAMERA_DIALOG_H

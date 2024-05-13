/********************************************************************************
** Form generated from reading UI file 'spirometer_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SPIROMETER_DIALOG_H
#define UI_SPIROMETER_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include "widgets/test_info_widget.h"

QT_BEGIN_NAMESPACE

class Ui_SpirometerDialog
{
public:
    QVBoxLayout *verticalLayout;
    TestInfoWidget *testInfoWidget;
    QSpacerItem *verticalSpacer;

    void setupUi(QDialog *SpirometerDialog)
    {
        if (SpirometerDialog->objectName().isEmpty())
            SpirometerDialog->setObjectName(QString::fromUtf8("SpirometerDialog"));
        SpirometerDialog->resize(324, 34);
        verticalLayout = new QVBoxLayout(SpirometerDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        testInfoWidget = new TestInfoWidget(SpirometerDialog);
        testInfoWidget->setObjectName(QString::fromUtf8("testInfoWidget"));

        verticalLayout->addWidget(testInfoWidget);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(SpirometerDialog);

        QMetaObject::connectSlotsByName(SpirometerDialog);
    } // setupUi

    void retranslateUi(QDialog *SpirometerDialog)
    {
        SpirometerDialog->setWindowTitle(QCoreApplication::translate("SpirometerDialog", "Spirometer", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SpirometerDialog: public Ui_SpirometerDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SPIROMETER_DIALOG_H

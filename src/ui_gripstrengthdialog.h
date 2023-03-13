/********************************************************************************
** Form generated from reading UI file 'gripstrengthdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRIPSTRENGTHDIALOG_H
#define UI_GRIPSTRENGTHDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QVBoxLayout>
#include "widgets/MeasureWidget.h"

QT_BEGIN_NAMESPACE

class Ui_GripStrengthDialog
{
public:
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_2;
    MeasureWidget *measureWidget;

    void setupUi(QDialog *GripStrengthDialog)
    {
        if (GripStrengthDialog->objectName().isEmpty())
            GripStrengthDialog->setObjectName(QString::fromUtf8("GripStrengthDialog"));
        GripStrengthDialog->resize(900, 600);
        GripStrengthDialog->setSizeGripEnabled(true);
        verticalLayout_3 = new QVBoxLayout(GripStrengthDialog);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, -1, -1, -1);
        measureWidget = new MeasureWidget(GripStrengthDialog);
        measureWidget->setObjectName(QString::fromUtf8("measureWidget"));

        verticalLayout_2->addWidget(measureWidget);


        verticalLayout_3->addLayout(verticalLayout_2);


        retranslateUi(GripStrengthDialog);

        QMetaObject::connectSlotsByName(GripStrengthDialog);
    } // setupUi

    void retranslateUi(QDialog *GripStrengthDialog)
    {
        GripStrengthDialog->setWindowTitle(QCoreApplication::translate("GripStrengthDialog", "Grip Strength", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GripStrengthDialog: public Ui_GripStrengthDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRIPSTRENGTHDIALOG_H

/********************************************************************************
** Form generated from reading UI file 'grip_strength_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRIP_STRENGTH_DIALOG_H
#define UI_GRIP_STRENGTH_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include "widgets/measurement_table.h"
#include "widgets/test_info_widget.h"

QT_BEGIN_NAMESPACE

class Ui_GripStrengthDialog
{
public:
    QVBoxLayout *verticalLayout;
    TestInfoWidget *testInfoWidget;
    QGroupBox *measurementInfo;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout;
    QLabel *averageLabel;
    QLabel *averageValue;
    QHBoxLayout *horizontalLayout_2;
    QLabel *maximumLabel;
    QLabel *maximumValue;
    QHBoxLayout *horizontalLayout_3;
    QLabel *cvLabel;
    QLabel *cvValue;
    MeasurementTable *measurementTable;

    void setupUi(QDialog *GripStrengthDialog)
    {
        if (GripStrengthDialog->objectName().isEmpty())
            GripStrengthDialog->setObjectName(QString::fromUtf8("GripStrengthDialog"));
        GripStrengthDialog->resize(585, 209);
        GripStrengthDialog->setSizeGripEnabled(true);
        verticalLayout = new QVBoxLayout(GripStrengthDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        testInfoWidget = new TestInfoWidget(GripStrengthDialog);
        testInfoWidget->setObjectName(QString::fromUtf8("testInfoWidget"));

        verticalLayout->addWidget(testInfoWidget);

        measurementInfo = new QGroupBox(GripStrengthDialog);
        measurementInfo->setObjectName(QString::fromUtf8("measurementInfo"));
        verticalLayout_3 = new QVBoxLayout(measurementInfo);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        averageLabel = new QLabel(measurementInfo);
        averageLabel->setObjectName(QString::fromUtf8("averageLabel"));

        horizontalLayout->addWidget(averageLabel);

        averageValue = new QLabel(measurementInfo);
        averageValue->setObjectName(QString::fromUtf8("averageValue"));

        horizontalLayout->addWidget(averageValue, 0, Qt::AlignmentFlag::AlignRight);


        verticalLayout_3->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        maximumLabel = new QLabel(measurementInfo);
        maximumLabel->setObjectName(QString::fromUtf8("maximumLabel"));

        horizontalLayout_2->addWidget(maximumLabel);

        maximumValue = new QLabel(measurementInfo);
        maximumValue->setObjectName(QString::fromUtf8("maximumValue"));

        horizontalLayout_2->addWidget(maximumValue, 0, Qt::AlignmentFlag::AlignRight);


        verticalLayout_3->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        cvLabel = new QLabel(measurementInfo);
        cvLabel->setObjectName(QString::fromUtf8("cvLabel"));

        horizontalLayout_3->addWidget(cvLabel);

        cvValue = new QLabel(measurementInfo);
        cvValue->setObjectName(QString::fromUtf8("cvValue"));

        horizontalLayout_3->addWidget(cvValue, 0, Qt::AlignmentFlag::AlignRight);


        verticalLayout_3->addLayout(horizontalLayout_3);


        verticalLayout->addWidget(measurementInfo);

        measurementTable = new MeasurementTable(GripStrengthDialog);
        measurementTable->setObjectName(QString::fromUtf8("measurementTable"));

        verticalLayout->addWidget(measurementTable);


        retranslateUi(GripStrengthDialog);

        QMetaObject::connectSlotsByName(GripStrengthDialog);
    } // setupUi

    void retranslateUi(QDialog *GripStrengthDialog)
    {
        GripStrengthDialog->setWindowTitle(QCoreApplication::translate("GripStrengthDialog", "Grip Strength", nullptr));
        measurementInfo->setTitle(QCoreApplication::translate("GripStrengthDialog", "Summary", nullptr));
        averageLabel->setText(QCoreApplication::translate("GripStrengthDialog", "Average", nullptr));
        averageValue->setText(QString());
        maximumLabel->setText(QCoreApplication::translate("GripStrengthDialog", "Maximum", nullptr));
        maximumValue->setText(QString());
        cvLabel->setText(QCoreApplication::translate("GripStrengthDialog", "CV", nullptr));
        cvValue->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class GripStrengthDialog: public Ui_GripStrengthDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRIP_STRENGTH_DIALOG_H

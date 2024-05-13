/********************************************************************************
** Form generated from reading UI file 'measurement_table.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MEASUREMENT_TABLE_H
#define UI_MEASUREMENT_TABLE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MeasurementTable
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *measurementsInfo;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *manualEntryToggle;
    QSpacerItem *horizontalSpacer;
    QPushButton *measureButton;
    QPushButton *addMeasureButton;
    QPushButton *submitButton;
    QTableWidget *measurementTable;

    void setupUi(QWidget *MeasurementTable)
    {
        if (MeasurementTable->objectName().isEmpty())
            MeasurementTable->setObjectName(QString::fromUtf8("MeasurementTable"));
        MeasurementTable->resize(456, 300);
        verticalLayout = new QVBoxLayout(MeasurementTable);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        measurementsInfo = new QGroupBox(MeasurementTable);
        measurementsInfo->setObjectName(QString::fromUtf8("measurementsInfo"));
        verticalLayout_2 = new QVBoxLayout(measurementsInfo);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        manualEntryToggle = new QPushButton(measurementsInfo);
        manualEntryToggle->setObjectName(QString::fromUtf8("manualEntryToggle"));

        horizontalLayout_3->addWidget(manualEntryToggle);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        measureButton = new QPushButton(measurementsInfo);
        measureButton->setObjectName(QString::fromUtf8("measureButton"));
        measureButton->setEnabled(false);

        horizontalLayout_3->addWidget(measureButton);

        addMeasureButton = new QPushButton(measurementsInfo);
        addMeasureButton->setObjectName(QString::fromUtf8("addMeasureButton"));

        horizontalLayout_3->addWidget(addMeasureButton);

        submitButton = new QPushButton(measurementsInfo);
        submitButton->setObjectName(QString::fromUtf8("submitButton"));
        submitButton->setEnabled(true);

        horizontalLayout_3->addWidget(submitButton);


        verticalLayout_2->addLayout(horizontalLayout_3);

        measurementTable = new QTableWidget(measurementsInfo);
        measurementTable->setObjectName(QString::fromUtf8("measurementTable"));
        measurementTable->setFrameShape(QFrame::NoFrame);
        measurementTable->setFrameShadow(QFrame::Plain);
        measurementTable->setGridStyle(Qt::SolidLine);

        verticalLayout_2->addWidget(measurementTable);


        verticalLayout->addWidget(measurementsInfo);


        retranslateUi(MeasurementTable);

        QMetaObject::connectSlotsByName(MeasurementTable);
    } // setupUi

    void retranslateUi(QWidget *MeasurementTable)
    {
        MeasurementTable->setWindowTitle(QCoreApplication::translate("MeasurementTable", "Form", nullptr));
        measurementsInfo->setTitle(QCoreApplication::translate("MeasurementTable", "Measurements", nullptr));
        manualEntryToggle->setText(QCoreApplication::translate("MeasurementTable", "Manual Entry", nullptr));
        measureButton->setText(QCoreApplication::translate("MeasurementTable", "Measure", nullptr));
        addMeasureButton->setText(QCoreApplication::translate("MeasurementTable", "Add Measure", nullptr));
        submitButton->setText(QCoreApplication::translate("MeasurementTable", "Submit", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MeasurementTable: public Ui_MeasurementTable {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MEASUREMENT_TABLE_H

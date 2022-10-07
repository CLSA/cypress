/********************************************************************************
** Form generated from reading UI file 'runnabledialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RUNNABLEDIALOG_H
#define UI_RUNNABLEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include "widgets/BarcodeWidget.h"
#include "widgets/MeasureWidget.h"

QT_BEGIN_NAMESPACE

class Ui_RunnableDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QGroupBox *idGroupBox;
    QHBoxLayout *horizontalLayout;
    BarcodeWidget *barcodeWidget;
    QGroupBox *pathGroupBox;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *openButton;
    QSpacerItem *horizontalSpacer_2;
    QGroupBox *measureGroupBox;
    QVBoxLayout *verticalLayout;
    MeasureWidget *measureWidget;
    QStatusBar *statusBar;

    void setupUi(QDialog *RunnableDialog)
    {
        if (RunnableDialog->objectName().isEmpty())
            RunnableDialog->setObjectName(QString::fromUtf8("RunnableDialog"));
        RunnableDialog->resize(600, 389);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(RunnableDialog->sizePolicy().hasHeightForWidth());
        RunnableDialog->setSizePolicy(sizePolicy);
        verticalLayout_2 = new QVBoxLayout(RunnableDialog);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        idGroupBox = new QGroupBox(RunnableDialog);
        idGroupBox->setObjectName(QString::fromUtf8("idGroupBox"));
        sizePolicy.setHeightForWidth(idGroupBox->sizePolicy().hasHeightForWidth());
        idGroupBox->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(idGroupBox);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        barcodeWidget = new BarcodeWidget(idGroupBox);
        barcodeWidget->setObjectName(QString::fromUtf8("barcodeWidget"));

        horizontalLayout->addWidget(barcodeWidget);


        verticalLayout_2->addWidget(idGroupBox);

        pathGroupBox = new QGroupBox(RunnableDialog);
        pathGroupBox->setObjectName(QString::fromUtf8("pathGroupBox"));
        sizePolicy.setHeightForWidth(pathGroupBox->sizePolicy().hasHeightForWidth());
        pathGroupBox->setSizePolicy(sizePolicy);
        horizontalLayout_2 = new QHBoxLayout(pathGroupBox);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        openButton = new QPushButton(pathGroupBox);
        openButton->setObjectName(QString::fromUtf8("openButton"));

        horizontalLayout_2->addWidget(openButton);

        horizontalSpacer_2 = new QSpacerItem(469, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        verticalLayout_2->addWidget(pathGroupBox);

        measureGroupBox = new QGroupBox(RunnableDialog);
        measureGroupBox->setObjectName(QString::fromUtf8("measureGroupBox"));
        verticalLayout = new QVBoxLayout(measureGroupBox);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        measureWidget = new MeasureWidget(measureGroupBox);
        measureWidget->setObjectName(QString::fromUtf8("measureWidget"));

        verticalLayout->addWidget(measureWidget);


        verticalLayout_2->addWidget(measureGroupBox);

        statusBar = new QStatusBar(RunnableDialog);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        statusBar->setSizeGripEnabled(false);

        verticalLayout_2->addWidget(statusBar);


        retranslateUi(RunnableDialog);

        QMetaObject::connectSlotsByName(RunnableDialog);
    } // setupUi

    void retranslateUi(QDialog *RunnableDialog)
    {
        RunnableDialog->setWindowTitle(QString());
        idGroupBox->setTitle(QCoreApplication::translate("RunnableDialog", "barcode", nullptr));
        pathGroupBox->setTitle(QCoreApplication::translate("RunnableDialog", "executable", nullptr));
        openButton->setText(QCoreApplication::translate("RunnableDialog", "Open", nullptr));
        measureGroupBox->setTitle(QCoreApplication::translate("RunnableDialog", "measure", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RunnableDialog: public Ui_RunnableDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RUNNABLEDIALOG_H

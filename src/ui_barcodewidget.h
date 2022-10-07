/********************************************************************************
** Form generated from reading UI file 'barcodewidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BARCODEWIDGET_H
#define UI_BARCODEWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BarcodeWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QLabel *idLabel;
    QLineEdit *barcodeLineEdit;
    QSpacerItem *horizontalSpacer;

    void setupUi(QWidget *BarcodeWidget)
    {
        if (BarcodeWidget->objectName().isEmpty())
            BarcodeWidget->setObjectName(QString::fromUtf8("BarcodeWidget"));
        BarcodeWidget->resize(400, 43);
        horizontalLayout = new QHBoxLayout(BarcodeWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        idLabel = new QLabel(BarcodeWidget);
        idLabel->setObjectName(QString::fromUtf8("idLabel"));

        horizontalLayout->addWidget(idLabel);

        barcodeLineEdit = new QLineEdit(BarcodeWidget);
        barcodeLineEdit->setObjectName(QString::fromUtf8("barcodeLineEdit"));
        barcodeLineEdit->setBaseSize(QSize(0, 0));
        barcodeLineEdit->setClearButtonEnabled(true);

        horizontalLayout->addWidget(barcodeLineEdit);

        horizontalSpacer = new QSpacerItem(80, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

#if QT_CONFIG(shortcut)
        idLabel->setBuddy(barcodeLineEdit);
#endif // QT_CONFIG(shortcut)

        retranslateUi(BarcodeWidget);

        QMetaObject::connectSlotsByName(BarcodeWidget);
    } // setupUi

    void retranslateUi(QWidget *BarcodeWidget)
    {
        BarcodeWidget->setWindowTitle(QCoreApplication::translate("BarcodeWidget", "Form", nullptr));
        idLabel->setText(QCoreApplication::translate("BarcodeWidget", "Participant ID:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BarcodeWidget: public Ui_BarcodeWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BARCODEWIDGET_H

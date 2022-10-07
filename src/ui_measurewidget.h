/********************************************************************************
** Form generated from reading UI file 'measurewidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MEASUREWIDGET_H
#define UI_MEASUREWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MeasureWidget
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *measureButton;
    QPushButton *saveButton;
    QPushButton *closeButton;
    QSpacerItem *horizontalSpacer;
    QTableView *testdataTableView;

    void setupUi(QWidget *MeasureWidget)
    {
        if (MeasureWidget->objectName().isEmpty())
            MeasureWidget->setObjectName(QString::fromUtf8("MeasureWidget"));
        MeasureWidget->resize(400, 300);
        verticalLayout = new QVBoxLayout(MeasureWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        measureButton = new QPushButton(MeasureWidget);
        measureButton->setObjectName(QString::fromUtf8("measureButton"));

        horizontalLayout->addWidget(measureButton);

        saveButton = new QPushButton(MeasureWidget);
        saveButton->setObjectName(QString::fromUtf8("saveButton"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(saveButton->sizePolicy().hasHeightForWidth());
        saveButton->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(saveButton);

        closeButton = new QPushButton(MeasureWidget);
        closeButton->setObjectName(QString::fromUtf8("closeButton"));

        horizontalLayout->addWidget(closeButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        testdataTableView = new QTableView(MeasureWidget);
        testdataTableView->setObjectName(QString::fromUtf8("testdataTableView"));
        testdataTableView->setFrameShadow(QFrame::Plain);
        testdataTableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        testdataTableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        testdataTableView->setAutoScroll(false);
        testdataTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        testdataTableView->setProperty("showDropIndicator", QVariant(false));
        testdataTableView->setDragDropOverwriteMode(false);
        testdataTableView->setAlternatingRowColors(true);
        testdataTableView->setSelectionMode(QAbstractItemView::NoSelection);
        testdataTableView->setGridStyle(Qt::SolidLine);
        testdataTableView->horizontalHeader()->setMinimumSectionSize(24);

        verticalLayout->addWidget(testdataTableView);


        retranslateUi(MeasureWidget);

        QMetaObject::connectSlotsByName(MeasureWidget);
    } // setupUi

    void retranslateUi(QWidget *MeasureWidget)
    {
        MeasureWidget->setWindowTitle(QCoreApplication::translate("MeasureWidget", "Form", nullptr));
        measureButton->setText(QCoreApplication::translate("MeasureWidget", "Measure", nullptr));
        saveButton->setText(QCoreApplication::translate("MeasureWidget", "Save", nullptr));
        closeButton->setText(QCoreApplication::translate("MeasureWidget", "Close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MeasureWidget: public Ui_MeasureWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MEASUREWIDGET_H

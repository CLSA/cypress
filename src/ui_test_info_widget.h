/********************************************************************************
** Form generated from reading UI file 'test_info_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TEST_INFO_WIDGET_H
#define UI_TEST_INFO_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TestInfoWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLabel *label;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_6;
    QLabel *label_4;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_5;
    QLabel *label_3;

    void setupUi(QWidget *TestInfoWidget)
    {
        if (TestInfoWidget->objectName().isEmpty())
            TestInfoWidget->setObjectName(QString::fromUtf8("TestInfoWidget"));
        TestInfoWidget->resize(400, 282);
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(TestInfoWidget->sizePolicy().hasHeightForWidth());
        TestInfoWidget->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(TestInfoWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        groupBox = new QGroupBox(TestInfoWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy1);
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setSizeConstraint(QLayout::SetMinimumSize);
        horizontalLayout_2->setContentsMargins(0, -1, -1, -1);
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_2->addWidget(label);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setSizeConstraint(QLayout::SetMinimumSize);
        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        horizontalLayout_3->addWidget(label_6);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_3->addWidget(label_4);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setSizeConstraint(QLayout::SetMinimumSize);
        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_4->addWidget(label_5);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_4->addWidget(label_3);


        verticalLayout->addLayout(horizontalLayout_4);


        horizontalLayout->addWidget(groupBox);


        retranslateUi(TestInfoWidget);

        QMetaObject::connectSlotsByName(TestInfoWidget);
    } // setupUi

    void retranslateUi(QWidget *TestInfoWidget)
    {
        TestInfoWidget->setWindowTitle(QCoreApplication::translate("TestInfoWidget", "Form", nullptr));
        groupBox->setTitle(QCoreApplication::translate("TestInfoWidget", "Test Information", nullptr));
        label_2->setText(QCoreApplication::translate("TestInfoWidget", "Barcode", nullptr));
        label->setText(QCoreApplication::translate("TestInfoWidget", "TextLabel", nullptr));
        label_6->setText(QCoreApplication::translate("TestInfoWidget", "Interviewer", nullptr));
        label_4->setText(QCoreApplication::translate("TestInfoWidget", "TextLabel", nullptr));
        label_5->setText(QCoreApplication::translate("TestInfoWidget", "Time Started", nullptr));
        label_3->setText(QCoreApplication::translate("TestInfoWidget", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TestInfoWidget: public Ui_TestInfoWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TEST_INFO_WIDGET_H

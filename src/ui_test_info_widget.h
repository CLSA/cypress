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
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *barcode;
    QLabel *barcodeLabel;
    QLabel *barcodeValue;
    QHBoxLayout *interviewer;
    QLabel *interviewerLabel;
    QLabel *interviewerValue;
    QHBoxLayout *horizontalLayout;
    QLabel *startedLabel;
    QLabel *startedValue;
    QHBoxLayout *deviceStatus;
    QLabel *deviceStatusLabel;
    QLabel *deviceStatusValue;
    QHBoxLayout *horizontalLayout_2;
    QLabel *statusLabel;
    QLabel *statusValue;

    void setupUi(QWidget *TestInfoWidget)
    {
        if (TestInfoWidget->objectName().isEmpty())
            TestInfoWidget->setObjectName(QString::fromUtf8("TestInfoWidget"));
        TestInfoWidget->resize(508, 270);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(TestInfoWidget->sizePolicy().hasHeightForWidth());
        TestInfoWidget->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(TestInfoWidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        groupBox = new QGroupBox(TestInfoWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setSpacing(9);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        barcode = new QHBoxLayout();
        barcode->setObjectName(QString::fromUtf8("barcode"));
        barcode->setSizeConstraint(QLayout::SetMinimumSize);
        barcode->setContentsMargins(0, -1, -1, -1);
        barcodeLabel = new QLabel(groupBox);
        barcodeLabel->setObjectName(QString::fromUtf8("barcodeLabel"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(barcodeLabel->sizePolicy().hasHeightForWidth());
        barcodeLabel->setSizePolicy(sizePolicy1);
        barcodeLabel->setMargin(0);

        barcode->addWidget(barcodeLabel);

        barcodeValue = new QLabel(groupBox);
        barcodeValue->setObjectName(QString::fromUtf8("barcodeValue"));
        sizePolicy1.setHeightForWidth(barcodeValue->sizePolicy().hasHeightForWidth());
        barcodeValue->setSizePolicy(sizePolicy1);
        barcodeValue->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        barcodeValue->setMargin(0);

        barcode->addWidget(barcodeValue);


        verticalLayout_2->addLayout(barcode);

        interviewer = new QHBoxLayout();
        interviewer->setObjectName(QString::fromUtf8("interviewer"));
        interviewer->setSizeConstraint(QLayout::SetMinimumSize);
        interviewerLabel = new QLabel(groupBox);
        interviewerLabel->setObjectName(QString::fromUtf8("interviewerLabel"));
        sizePolicy1.setHeightForWidth(interviewerLabel->sizePolicy().hasHeightForWidth());
        interviewerLabel->setSizePolicy(sizePolicy1);
        interviewerLabel->setMargin(0);

        interviewer->addWidget(interviewerLabel);

        interviewerValue = new QLabel(groupBox);
        interviewerValue->setObjectName(QString::fromUtf8("interviewerValue"));
        sizePolicy1.setHeightForWidth(interviewerValue->sizePolicy().hasHeightForWidth());
        interviewerValue->setSizePolicy(sizePolicy1);
        interviewerValue->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        interviewerValue->setMargin(0);

        interviewer->addWidget(interviewerValue);


        verticalLayout_2->addLayout(interviewer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        startedLabel = new QLabel(groupBox);
        startedLabel->setObjectName(QString::fromUtf8("startedLabel"));

        horizontalLayout->addWidget(startedLabel);

        startedValue = new QLabel(groupBox);
        startedValue->setObjectName(QString::fromUtf8("startedValue"));

        horizontalLayout->addWidget(startedValue);


        verticalLayout_2->addLayout(horizontalLayout);

        deviceStatus = new QHBoxLayout();
        deviceStatus->setObjectName(QString::fromUtf8("deviceStatus"));
        deviceStatus->setSizeConstraint(QLayout::SetMinimumSize);
        deviceStatusLabel = new QLabel(groupBox);
        deviceStatusLabel->setObjectName(QString::fromUtf8("deviceStatusLabel"));
        sizePolicy1.setHeightForWidth(deviceStatusLabel->sizePolicy().hasHeightForWidth());
        deviceStatusLabel->setSizePolicy(sizePolicy1);
        deviceStatusLabel->setMargin(0);

        deviceStatus->addWidget(deviceStatusLabel);

        deviceStatusValue = new QLabel(groupBox);
        deviceStatusValue->setObjectName(QString::fromUtf8("deviceStatusValue"));
        sizePolicy1.setHeightForWidth(deviceStatusValue->sizePolicy().hasHeightForWidth());
        deviceStatusValue->setSizePolicy(sizePolicy1);
        deviceStatusValue->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        deviceStatusValue->setMargin(0);

        deviceStatus->addWidget(deviceStatusValue);


        verticalLayout_2->addLayout(deviceStatus);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        statusLabel = new QLabel(groupBox);
        statusLabel->setObjectName(QString::fromUtf8("statusLabel"));

        horizontalLayout_2->addWidget(statusLabel);

        statusValue = new QLabel(groupBox);
        statusValue->setObjectName(QString::fromUtf8("statusValue"));

        horizontalLayout_2->addWidget(statusValue);


        verticalLayout_2->addLayout(horizontalLayout_2);


        verticalLayout->addWidget(groupBox);


        retranslateUi(TestInfoWidget);

        QMetaObject::connectSlotsByName(TestInfoWidget);
    } // setupUi

    void retranslateUi(QWidget *TestInfoWidget)
    {
        TestInfoWidget->setWindowTitle(QCoreApplication::translate("TestInfoWidget", "Form", nullptr));
        groupBox->setTitle(QCoreApplication::translate("TestInfoWidget", "Test Information", nullptr));
        barcodeLabel->setText(QCoreApplication::translate("TestInfoWidget", "Participant ID", nullptr));
        barcodeValue->setText(QCoreApplication::translate("TestInfoWidget", "---", nullptr));
        interviewerLabel->setText(QCoreApplication::translate("TestInfoWidget", "Interviewer", nullptr));
        interviewerValue->setText(QCoreApplication::translate("TestInfoWidget", "---", nullptr));
        startedLabel->setText(QCoreApplication::translate("TestInfoWidget", "Started", nullptr));
        startedValue->setText(QCoreApplication::translate("TestInfoWidget", "---", nullptr));
        deviceStatusLabel->setText(QCoreApplication::translate("TestInfoWidget", "Device", nullptr));
        deviceStatusValue->setText(QCoreApplication::translate("TestInfoWidget", "---", nullptr));
        statusLabel->setText(QCoreApplication::translate("TestInfoWidget", "Status", nullptr));
        statusValue->setText(QCoreApplication::translate("TestInfoWidget", "---", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TestInfoWidget: public Ui_TestInfoWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TEST_INFO_WIDGET_H

/********************************************************************************
** Form generated from reading UI file 'dicom_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DICOM_WIDGET_H
#define UI_DICOM_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DicomWidget
{
public:
    QVBoxLayout *verticalLayout_2;
    QGroupBox *dicomGroup;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *aeTitleLayout;
    QLabel *aeTitleLabel;
    QSpacerItem *horizontalSpacer;
    QLineEdit *aeTitleValue;
    QHBoxLayout *hostnameLayout;
    QLabel *hostnameLabel;
    QSpacerItem *horizontalSpacer_2;
    QLabel *hostnameValue;
    QHBoxLayout *portLayout;
    QLabel *portLabel;
    QSpacerItem *horizontalSpacer_3;
    QLineEdit *portValue;

    void setupUi(QWidget *DicomWidget)
    {
        if (DicomWidget->objectName().isEmpty())
            DicomWidget->setObjectName(QString::fromUtf8("DicomWidget"));
        DicomWidget->resize(353, 172);
        verticalLayout_2 = new QVBoxLayout(DicomWidget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        dicomGroup = new QGroupBox(DicomWidget);
        dicomGroup->setObjectName(QString::fromUtf8("dicomGroup"));
        verticalLayout = new QVBoxLayout(dicomGroup);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        aeTitleLayout = new QHBoxLayout();
        aeTitleLayout->setObjectName(QString::fromUtf8("aeTitleLayout"));
        aeTitleLabel = new QLabel(dicomGroup);
        aeTitleLabel->setObjectName(QString::fromUtf8("aeTitleLabel"));

        aeTitleLayout->addWidget(aeTitleLabel);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        aeTitleLayout->addItem(horizontalSpacer);

        aeTitleValue = new QLineEdit(dicomGroup);
        aeTitleValue->setObjectName(QString::fromUtf8("aeTitleValue"));
        aeTitleValue->setFocusPolicy(Qt::ClickFocus);

        aeTitleLayout->addWidget(aeTitleValue);


        verticalLayout->addLayout(aeTitleLayout);

        hostnameLayout = new QHBoxLayout();
        hostnameLayout->setObjectName(QString::fromUtf8("hostnameLayout"));
        hostnameLabel = new QLabel(dicomGroup);
        hostnameLabel->setObjectName(QString::fromUtf8("hostnameLabel"));

        hostnameLayout->addWidget(hostnameLabel);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hostnameLayout->addItem(horizontalSpacer_2);

        hostnameValue = new QLabel(dicomGroup);
        hostnameValue->setObjectName(QString::fromUtf8("hostnameValue"));

        hostnameLayout->addWidget(hostnameValue);


        verticalLayout->addLayout(hostnameLayout);

        portLayout = new QHBoxLayout();
        portLayout->setObjectName(QString::fromUtf8("portLayout"));
        portLabel = new QLabel(dicomGroup);
        portLabel->setObjectName(QString::fromUtf8("portLabel"));

        portLayout->addWidget(portLabel);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        portLayout->addItem(horizontalSpacer_3);

        portValue = new QLineEdit(dicomGroup);
        portValue->setObjectName(QString::fromUtf8("portValue"));
        portValue->setFocusPolicy(Qt::ClickFocus);

        portLayout->addWidget(portValue);


        verticalLayout->addLayout(portLayout);


        verticalLayout_2->addWidget(dicomGroup);


        retranslateUi(DicomWidget);

        QMetaObject::connectSlotsByName(DicomWidget);
    } // setupUi

    void retranslateUi(QWidget *DicomWidget)
    {
        DicomWidget->setWindowTitle(QCoreApplication::translate("DicomWidget", "Form", nullptr));
        dicomGroup->setTitle(QCoreApplication::translate("DicomWidget", "DICOM Settings", nullptr));
        aeTitleLabel->setText(QCoreApplication::translate("DicomWidget", "AE Title", nullptr));
        hostnameLabel->setText(QCoreApplication::translate("DicomWidget", "Hostname / IP", nullptr));
        hostnameValue->setText(QString());
        portLabel->setText(QCoreApplication::translate("DicomWidget", "Port", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DicomWidget: public Ui_DicomWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DICOM_WIDGET_H

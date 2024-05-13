/********************************************************************************
** Form generated from reading UI file 'body_composition_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BODY_COMPOSITION_DIALOG_H
#define UI_BODY_COMPOSITION_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_BodyCompositionDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QGroupBox *serialPortGroupBox;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QLabel *serialPortLabel;
    QComboBox *deviceComboBox;
    QSpacerItem *horizontalSpacer;
    QPushButton *connectButton;
    QPushButton *disconnectButton;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *completeButton;
    QPushButton *measureButton;
    QPushButton *submitButton;
    QGroupBox *inputsGroupBox;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label;
    QLineEdit *ageLineEdit;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_5;
    QRadioButton *maleRadio;
    QRadioButton *femaleRadio;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_3;
    QLineEdit *heightLineEdit;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_6;
    QRadioButton *metricRadio;
    QRadioButton *imperialRadio;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_4;
    QRadioButton *standardRadio;
    QRadioButton *athleticRadio;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_2;
    QLineEdit *tareWeightLineEdit;
    QHBoxLayout *horizontalLayout_9;
    QPushButton *setButton;
    QPushButton *resetButton;
    QPushButton *confirmButton;

    void setupUi(QDialog *BodyCompositionDialog)
    {
        if (BodyCompositionDialog->objectName().isEmpty())
            BodyCompositionDialog->setObjectName(QString::fromUtf8("BodyCompositionDialog"));
        BodyCompositionDialog->resize(587, 649);
        verticalLayout_2 = new QVBoxLayout(BodyCompositionDialog);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        serialPortGroupBox = new QGroupBox(BodyCompositionDialog);
        serialPortGroupBox->setObjectName(QString::fromUtf8("serialPortGroupBox"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(serialPortGroupBox->sizePolicy().hasHeightForWidth());
        serialPortGroupBox->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(serialPortGroupBox);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        serialPortLabel = new QLabel(serialPortGroupBox);
        serialPortLabel->setObjectName(QString::fromUtf8("serialPortLabel"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(serialPortLabel->sizePolicy().hasHeightForWidth());
        serialPortLabel->setSizePolicy(sizePolicy1);

        horizontalLayout_3->addWidget(serialPortLabel);

        deviceComboBox = new QComboBox(serialPortGroupBox);
        deviceComboBox->setObjectName(QString::fromUtf8("deviceComboBox"));

        horizontalLayout_3->addWidget(deviceComboBox);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        connectButton = new QPushButton(serialPortGroupBox);
        connectButton->setObjectName(QString::fromUtf8("connectButton"));
        connectButton->setEnabled(false);

        horizontalLayout_3->addWidget(connectButton);

        disconnectButton = new QPushButton(serialPortGroupBox);
        disconnectButton->setObjectName(QString::fromUtf8("disconnectButton"));
        disconnectButton->setEnabled(false);

        horizontalLayout_3->addWidget(disconnectButton);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        completeButton = new QPushButton(serialPortGroupBox);
        completeButton->setObjectName(QString::fromUtf8("completeButton"));

        horizontalLayout->addWidget(completeButton);

        measureButton = new QPushButton(serialPortGroupBox);
        measureButton->setObjectName(QString::fromUtf8("measureButton"));
        measureButton->setEnabled(false);

        horizontalLayout->addWidget(measureButton);

        submitButton = new QPushButton(serialPortGroupBox);
        submitButton->setObjectName(QString::fromUtf8("submitButton"));
        submitButton->setEnabled(false);

        horizontalLayout->addWidget(submitButton);


        verticalLayout->addLayout(horizontalLayout);


        verticalLayout_2->addWidget(serialPortGroupBox);

        inputsGroupBox = new QGroupBox(BodyCompositionDialog);
        inputsGroupBox->setObjectName(QString::fromUtf8("inputsGroupBox"));
        sizePolicy.setHeightForWidth(inputsGroupBox->sizePolicy().hasHeightForWidth());
        inputsGroupBox->setSizePolicy(sizePolicy);
        verticalLayout_4 = new QVBoxLayout(inputsGroupBox);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label = new QLabel(inputsGroupBox);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_4->addWidget(label);

        ageLineEdit = new QLineEdit(inputsGroupBox);
        ageLineEdit->setObjectName(QString::fromUtf8("ageLineEdit"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(ageLineEdit->sizePolicy().hasHeightForWidth());
        ageLineEdit->setSizePolicy(sizePolicy2);
        ageLineEdit->setClearButtonEnabled(true);

        horizontalLayout_4->addWidget(ageLineEdit);


        verticalLayout_4->addLayout(horizontalLayout_4);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_5 = new QLabel(inputsGroupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_2->addWidget(label_5);

        maleRadio = new QRadioButton(inputsGroupBox);
        maleRadio->setObjectName(QString::fromUtf8("maleRadio"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(maleRadio->sizePolicy().hasHeightForWidth());
        maleRadio->setSizePolicy(sizePolicy3);
        maleRadio->setChecked(true);
        maleRadio->setAutoExclusive(false);

        horizontalLayout_2->addWidget(maleRadio);

        femaleRadio = new QRadioButton(inputsGroupBox);
        femaleRadio->setObjectName(QString::fromUtf8("femaleRadio"));
        sizePolicy3.setHeightForWidth(femaleRadio->sizePolicy().hasHeightForWidth());
        femaleRadio->setSizePolicy(sizePolicy3);
        femaleRadio->setAutoExclusive(false);

        horizontalLayout_2->addWidget(femaleRadio);


        verticalLayout_4->addLayout(horizontalLayout_2);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_3 = new QLabel(inputsGroupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_7->addWidget(label_3);

        heightLineEdit = new QLineEdit(inputsGroupBox);
        heightLineEdit->setObjectName(QString::fromUtf8("heightLineEdit"));
        sizePolicy2.setHeightForWidth(heightLineEdit->sizePolicy().hasHeightForWidth());
        heightLineEdit->setSizePolicy(sizePolicy2);
        heightLineEdit->setClearButtonEnabled(true);

        horizontalLayout_7->addWidget(heightLineEdit);


        verticalLayout_4->addLayout(horizontalLayout_7);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        label_6 = new QLabel(inputsGroupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        horizontalLayout_8->addWidget(label_6);

        metricRadio = new QRadioButton(inputsGroupBox);
        metricRadio->setObjectName(QString::fromUtf8("metricRadio"));
        metricRadio->setChecked(true);
        metricRadio->setAutoExclusive(false);

        horizontalLayout_8->addWidget(metricRadio);

        imperialRadio = new QRadioButton(inputsGroupBox);
        imperialRadio->setObjectName(QString::fromUtf8("imperialRadio"));
        imperialRadio->setAutoExclusive(false);

        horizontalLayout_8->addWidget(imperialRadio);


        verticalLayout_4->addLayout(horizontalLayout_8);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_4 = new QLabel(inputsGroupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_5->addWidget(label_4);

        standardRadio = new QRadioButton(inputsGroupBox);
        standardRadio->setObjectName(QString::fromUtf8("standardRadio"));
        sizePolicy3.setHeightForWidth(standardRadio->sizePolicy().hasHeightForWidth());
        standardRadio->setSizePolicy(sizePolicy3);
        standardRadio->setChecked(true);
        standardRadio->setAutoExclusive(false);

        horizontalLayout_5->addWidget(standardRadio);

        athleticRadio = new QRadioButton(inputsGroupBox);
        athleticRadio->setObjectName(QString::fromUtf8("athleticRadio"));
        sizePolicy3.setHeightForWidth(athleticRadio->sizePolicy().hasHeightForWidth());
        athleticRadio->setSizePolicy(sizePolicy3);
        athleticRadio->setAutoExclusive(false);

        horizontalLayout_5->addWidget(athleticRadio);


        verticalLayout_4->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_2 = new QLabel(inputsGroupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_6->addWidget(label_2);

        tareWeightLineEdit = new QLineEdit(inputsGroupBox);
        tareWeightLineEdit->setObjectName(QString::fromUtf8("tareWeightLineEdit"));
        sizePolicy2.setHeightForWidth(tareWeightLineEdit->sizePolicy().hasHeightForWidth());
        tareWeightLineEdit->setSizePolicy(sizePolicy2);
        tareWeightLineEdit->setMaxLength(32767);
        tareWeightLineEdit->setClearButtonEnabled(true);

        horizontalLayout_6->addWidget(tareWeightLineEdit);


        verticalLayout_4->addLayout(horizontalLayout_6);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        setButton = new QPushButton(inputsGroupBox);
        setButton->setObjectName(QString::fromUtf8("setButton"));

        horizontalLayout_9->addWidget(setButton);

        resetButton = new QPushButton(inputsGroupBox);
        resetButton->setObjectName(QString::fromUtf8("resetButton"));

        horizontalLayout_9->addWidget(resetButton);

        confirmButton = new QPushButton(inputsGroupBox);
        confirmButton->setObjectName(QString::fromUtf8("confirmButton"));

        horizontalLayout_9->addWidget(confirmButton);


        verticalLayout_4->addLayout(horizontalLayout_9);


        verticalLayout_2->addWidget(inputsGroupBox);


        retranslateUi(BodyCompositionDialog);

        QMetaObject::connectSlotsByName(BodyCompositionDialog);
    } // setupUi

    void retranslateUi(QDialog *BodyCompositionDialog)
    {
        BodyCompositionDialog->setWindowTitle(QCoreApplication::translate("BodyCompositionDialog", "Body Composition Measurement", nullptr));
        serialPortGroupBox->setTitle(QString());
        serialPortLabel->setText(QCoreApplication::translate("BodyCompositionDialog", "Serial Port:", nullptr));
        connectButton->setText(QCoreApplication::translate("BodyCompositionDialog", "Connect", nullptr));
        disconnectButton->setText(QCoreApplication::translate("BodyCompositionDialog", "Disconnect", nullptr));
        completeButton->setText(QCoreApplication::translate("BodyCompositionDialog", "Complete", nullptr));
        measureButton->setText(QCoreApplication::translate("BodyCompositionDialog", "Measure", nullptr));
        submitButton->setText(QCoreApplication::translate("BodyCompositionDialog", "Submit", nullptr));
        inputsGroupBox->setTitle(QCoreApplication::translate("BodyCompositionDialog", "Inputs", nullptr));
        label->setText(QCoreApplication::translate("BodyCompositionDialog", "Age:", nullptr));
        ageLineEdit->setInputMask(QString());
        label_5->setText(QCoreApplication::translate("BodyCompositionDialog", "Gender:", nullptr));
        maleRadio->setText(QCoreApplication::translate("BodyCompositionDialog", "Male", nullptr));
        femaleRadio->setText(QCoreApplication::translate("BodyCompositionDialog", "Female", nullptr));
        label_3->setText(QCoreApplication::translate("BodyCompositionDialog", "Height:", nullptr));
        heightLineEdit->setInputMask(QString());
        heightLineEdit->setText(QString());
        label_6->setText(QCoreApplication::translate("BodyCompositionDialog", "Units:", nullptr));
        metricRadio->setText(QCoreApplication::translate("BodyCompositionDialog", "cm/kg", nullptr));
        imperialRadio->setText(QCoreApplication::translate("BodyCompositionDialog", "in/lb", nullptr));
        label_4->setText(QCoreApplication::translate("BodyCompositionDialog", "Body Type:", nullptr));
        standardRadio->setText(QCoreApplication::translate("BodyCompositionDialog", "Standard", nullptr));
        athleticRadio->setText(QCoreApplication::translate("BodyCompositionDialog", "Athletic", nullptr));
        label_2->setText(QCoreApplication::translate("BodyCompositionDialog", "Tare:", nullptr));
        tareWeightLineEdit->setInputMask(QString());
        tareWeightLineEdit->setText(QString());
        setButton->setText(QCoreApplication::translate("BodyCompositionDialog", "Set", nullptr));
        resetButton->setText(QCoreApplication::translate("BodyCompositionDialog", "Reset", nullptr));
        confirmButton->setText(QCoreApplication::translate("BodyCompositionDialog", "Confirm", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BodyCompositionDialog: public Ui_BodyCompositionDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BODY_COMPOSITION_DIALOG_H

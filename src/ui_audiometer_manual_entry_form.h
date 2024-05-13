/********************************************************************************
** Form generated from reading UI file 'audiometer_manual_entry_form.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AUDIOMETER_MANUAL_ENTRY_FORM_H
#define UI_AUDIOMETER_MANUAL_ENTRY_FORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AudiometerManualEntryForm
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_4;
    QLabel *sideLabel;
    QComboBox *sideComboBox;
    QHBoxLayout *horizontalLayout;
    QLabel *testLabel;
    QComboBox *testComboBox;
    QHBoxLayout *horizontalLayout_2;
    QLabel *levelLabel;
    QLineEdit *levelLineEdit;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer;
    QPushButton *okButton;
    QPushButton *cancelButton;
    QSpacerItem *verticalSpacer;

    void setupUi(QDialog *AudiometerManualEntryForm)
    {
        if (AudiometerManualEntryForm->objectName().isEmpty())
            AudiometerManualEntryForm->setObjectName(QString::fromUtf8("AudiometerManualEntryForm"));
        AudiometerManualEntryForm->resize(440, 146);
        verticalLayout = new QVBoxLayout(AudiometerManualEntryForm);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        sideLabel = new QLabel(AudiometerManualEntryForm);
        sideLabel->setObjectName(QString::fromUtf8("sideLabel"));

        horizontalLayout_4->addWidget(sideLabel);

        sideComboBox = new QComboBox(AudiometerManualEntryForm);
        sideComboBox->setObjectName(QString::fromUtf8("sideComboBox"));

        horizontalLayout_4->addWidget(sideComboBox);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        testLabel = new QLabel(AudiometerManualEntryForm);
        testLabel->setObjectName(QString::fromUtf8("testLabel"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(testLabel->sizePolicy().hasHeightForWidth());
        testLabel->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(testLabel);

        testComboBox = new QComboBox(AudiometerManualEntryForm);
        testComboBox->setObjectName(QString::fromUtf8("testComboBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(testComboBox->sizePolicy().hasHeightForWidth());
        testComboBox->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(testComboBox);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        levelLabel = new QLabel(AudiometerManualEntryForm);
        levelLabel->setObjectName(QString::fromUtf8("levelLabel"));
        sizePolicy.setHeightForWidth(levelLabel->sizePolicy().hasHeightForWidth());
        levelLabel->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(levelLabel);

        levelLineEdit = new QLineEdit(AudiometerManualEntryForm);
        levelLineEdit->setObjectName(QString::fromUtf8("levelLineEdit"));

        horizontalLayout_2->addWidget(levelLineEdit);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        okButton = new QPushButton(AudiometerManualEntryForm);
        okButton->setObjectName(QString::fromUtf8("okButton"));

        horizontalLayout_3->addWidget(okButton);

        cancelButton = new QPushButton(AudiometerManualEntryForm);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        horizontalLayout_3->addWidget(cancelButton);


        verticalLayout->addLayout(horizontalLayout_3);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(AudiometerManualEntryForm);

        QMetaObject::connectSlotsByName(AudiometerManualEntryForm);
    } // setupUi

    void retranslateUi(QDialog *AudiometerManualEntryForm)
    {
        AudiometerManualEntryForm->setWindowTitle(QCoreApplication::translate("AudiometerManualEntryForm", "Dialog", nullptr));
        sideLabel->setText(QCoreApplication::translate("AudiometerManualEntryForm", "Side", nullptr));
        testLabel->setText(QCoreApplication::translate("AudiometerManualEntryForm", "Test", nullptr));
        levelLabel->setText(QCoreApplication::translate("AudiometerManualEntryForm", "Level", nullptr));
        okButton->setText(QCoreApplication::translate("AudiometerManualEntryForm", "Ok", nullptr));
        cancelButton->setText(QCoreApplication::translate("AudiometerManualEntryForm", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AudiometerManualEntryForm: public Ui_AudiometerManualEntryForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AUDIOMETER_MANUAL_ENTRY_FORM_H

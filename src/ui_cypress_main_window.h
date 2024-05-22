/********************************************************************************
** Form generated from reading UI file 'cypress_main_window.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CYPRESS_MAIN_WINDOW_H
#define UI_CYPRESS_MAIN_WINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CypressMainWindow
{
public:
    QAction *actionCIMT;
    QAction *actionECG;
    QAction *actionBPM;
    QAction *actionAudiometer;
    QAction *actionCDTT;
    QAction *actionAudiometer_2;
    QAction *actionCDTT_2;
    QAction *actionCCB;
    QAction *actionGrip_Strength;
    QAction *actionVividI;
    QAction *actionECG_2;
    QAction *actionDEXA;
    QAction *actionBody_Comp;
    QAction *actionFrax;
    QAction *actionRetinal_Cam;
    QAction *actionSignature;
    QAction *actionTonometer;
    QAction *actionWeigh_Scale;
    QAction *actionSpirometer;
    QAction *actionBlood_Pressure;
    QAction *actionTest_Locally;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox_5;
    QVBoxLayout *verticalLayout_6;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *launchGripStrength;
    QPushButton *launchSpirometer;
    QPushButton *launchRetinalCamera;
    QPushButton *launchCDTT;
    QPushButton *launchEcg;
    QPushButton *launchChoiceReaction;
    QSpacerItem *horizontalSpacer_3;
    QGroupBox *groupBox_4;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *launchAudiometer;
    QPushButton *launchBpm;
    QPushButton *launchWeightScale;
    QSpacerItem *horizontalSpacer;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout;
    QPushButton *launchUltrasound;
    QPushButton *launchDxa1;
    QPushButton *launchDxa2;
    QPushButton *launchFrax;
    QSpacerItem *horizontalSpacer_2;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_3;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *CypressMainWindow)
    {
        if (CypressMainWindow->objectName().isEmpty())
            CypressMainWindow->setObjectName(QString::fromUtf8("CypressMainWindow"));
        CypressMainWindow->resize(800, 600);
        actionCIMT = new QAction(CypressMainWindow);
        actionCIMT->setObjectName(QString::fromUtf8("actionCIMT"));
        actionECG = new QAction(CypressMainWindow);
        actionECG->setObjectName(QString::fromUtf8("actionECG"));
        actionBPM = new QAction(CypressMainWindow);
        actionBPM->setObjectName(QString::fromUtf8("actionBPM"));
        actionAudiometer = new QAction(CypressMainWindow);
        actionAudiometer->setObjectName(QString::fromUtf8("actionAudiometer"));
        actionCDTT = new QAction(CypressMainWindow);
        actionCDTT->setObjectName(QString::fromUtf8("actionCDTT"));
        actionAudiometer_2 = new QAction(CypressMainWindow);
        actionAudiometer_2->setObjectName(QString::fromUtf8("actionAudiometer_2"));
        actionCDTT_2 = new QAction(CypressMainWindow);
        actionCDTT_2->setObjectName(QString::fromUtf8("actionCDTT_2"));
        actionCCB = new QAction(CypressMainWindow);
        actionCCB->setObjectName(QString::fromUtf8("actionCCB"));
        actionGrip_Strength = new QAction(CypressMainWindow);
        actionGrip_Strength->setObjectName(QString::fromUtf8("actionGrip_Strength"));
        actionVividI = new QAction(CypressMainWindow);
        actionVividI->setObjectName(QString::fromUtf8("actionVividI"));
        actionECG_2 = new QAction(CypressMainWindow);
        actionECG_2->setObjectName(QString::fromUtf8("actionECG_2"));
        actionDEXA = new QAction(CypressMainWindow);
        actionDEXA->setObjectName(QString::fromUtf8("actionDEXA"));
        actionBody_Comp = new QAction(CypressMainWindow);
        actionBody_Comp->setObjectName(QString::fromUtf8("actionBody_Comp"));
        actionFrax = new QAction(CypressMainWindow);
        actionFrax->setObjectName(QString::fromUtf8("actionFrax"));
        actionRetinal_Cam = new QAction(CypressMainWindow);
        actionRetinal_Cam->setObjectName(QString::fromUtf8("actionRetinal_Cam"));
        actionSignature = new QAction(CypressMainWindow);
        actionSignature->setObjectName(QString::fromUtf8("actionSignature"));
        actionTonometer = new QAction(CypressMainWindow);
        actionTonometer->setObjectName(QString::fromUtf8("actionTonometer"));
        actionWeigh_Scale = new QAction(CypressMainWindow);
        actionWeigh_Scale->setObjectName(QString::fromUtf8("actionWeigh_Scale"));
        actionSpirometer = new QAction(CypressMainWindow);
        actionSpirometer->setObjectName(QString::fromUtf8("actionSpirometer"));
        actionBlood_Pressure = new QAction(CypressMainWindow);
        actionBlood_Pressure->setObjectName(QString::fromUtf8("actionBlood_Pressure"));
        actionTest_Locally = new QAction(CypressMainWindow);
        actionTest_Locally->setObjectName(QString::fromUtf8("actionTest_Locally"));
        centralwidget = new QWidget(CypressMainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setEnabled(true);
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        groupBox_5 = new QGroupBox(groupBox);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        verticalLayout_6 = new QVBoxLayout(groupBox_5);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        launchGripStrength = new QPushButton(groupBox_5);
        launchGripStrength->setObjectName(QString::fromUtf8("launchGripStrength"));
        launchGripStrength->setEnabled(false);

        horizontalLayout_2->addWidget(launchGripStrength);

        launchSpirometer = new QPushButton(groupBox_5);
        launchSpirometer->setObjectName(QString::fromUtf8("launchSpirometer"));
        launchSpirometer->setEnabled(false);

        horizontalLayout_2->addWidget(launchSpirometer);

        launchRetinalCamera = new QPushButton(groupBox_5);
        launchRetinalCamera->setObjectName(QString::fromUtf8("launchRetinalCamera"));
        launchRetinalCamera->setEnabled(false);

        horizontalLayout_2->addWidget(launchRetinalCamera);

        launchCDTT = new QPushButton(groupBox_5);
        launchCDTT->setObjectName(QString::fromUtf8("launchCDTT"));
        launchCDTT->setEnabled(false);

        horizontalLayout_2->addWidget(launchCDTT);

        launchEcg = new QPushButton(groupBox_5);
        launchEcg->setObjectName(QString::fromUtf8("launchEcg"));
        launchEcg->setEnabled(false);

        horizontalLayout_2->addWidget(launchEcg);

        launchChoiceReaction = new QPushButton(groupBox_5);
        launchChoiceReaction->setObjectName(QString::fromUtf8("launchChoiceReaction"));
        launchChoiceReaction->setEnabled(false);

        horizontalLayout_2->addWidget(launchChoiceReaction);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);


        verticalLayout_6->addLayout(horizontalLayout_2);


        verticalLayout_2->addWidget(groupBox_5);

        groupBox_4 = new QGroupBox(groupBox);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        verticalLayout_5 = new QVBoxLayout(groupBox_4);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        launchAudiometer = new QPushButton(groupBox_4);
        launchAudiometer->setObjectName(QString::fromUtf8("launchAudiometer"));
        launchAudiometer->setEnabled(false);

        horizontalLayout_4->addWidget(launchAudiometer);

        launchBpm = new QPushButton(groupBox_4);
        launchBpm->setObjectName(QString::fromUtf8("launchBpm"));
        launchBpm->setEnabled(false);

        horizontalLayout_4->addWidget(launchBpm);

        launchWeightScale = new QPushButton(groupBox_4);
        launchWeightScale->setObjectName(QString::fromUtf8("launchWeightScale"));
        launchWeightScale->setEnabled(false);

        horizontalLayout_4->addWidget(launchWeightScale);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);


        verticalLayout_5->addLayout(horizontalLayout_4);


        verticalLayout_2->addWidget(groupBox_4);

        groupBox_3 = new QGroupBox(groupBox);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        verticalLayout_4 = new QVBoxLayout(groupBox_3);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        launchUltrasound = new QPushButton(groupBox_3);
        launchUltrasound->setObjectName(QString::fromUtf8("launchUltrasound"));
        launchUltrasound->setEnabled(false);

        horizontalLayout->addWidget(launchUltrasound);

        launchDxa1 = new QPushButton(groupBox_3);
        launchDxa1->setObjectName(QString::fromUtf8("launchDxa1"));
        launchDxa1->setEnabled(false);

        horizontalLayout->addWidget(launchDxa1);

        launchDxa2 = new QPushButton(groupBox_3);
        launchDxa2->setObjectName(QString::fromUtf8("launchDxa2"));
        launchDxa2->setEnabled(false);

        horizontalLayout->addWidget(launchDxa2);

        launchFrax = new QPushButton(groupBox_3);
        launchFrax->setObjectName(QString::fromUtf8("launchFrax"));
        launchFrax->setEnabled(false);

        horizontalLayout->addWidget(launchFrax);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout_4->addLayout(horizontalLayout);


        verticalLayout_2->addWidget(groupBox_3);


        verticalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        verticalLayout_3 = new QVBoxLayout(groupBox_2);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        scrollArea = new QScrollArea(groupBox_2);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 756, 226));
        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout_3->addWidget(scrollArea);


        verticalLayout->addWidget(groupBox_2);

        CypressMainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(CypressMainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 21));
        CypressMainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(CypressMainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        CypressMainWindow->setStatusBar(statusbar);

        retranslateUi(CypressMainWindow);

        QMetaObject::connectSlotsByName(CypressMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *CypressMainWindow)
    {
        CypressMainWindow->setWindowTitle(QCoreApplication::translate("CypressMainWindow", "CLSA Cypress", nullptr));
        actionCIMT->setText(QCoreApplication::translate("CypressMainWindow", "CIMT", nullptr));
        actionECG->setText(QCoreApplication::translate("CypressMainWindow", "ECG", nullptr));
        actionBPM->setText(QCoreApplication::translate("CypressMainWindow", "BPM", nullptr));
        actionAudiometer->setText(QCoreApplication::translate("CypressMainWindow", "Audiometer", nullptr));
        actionCDTT->setText(QCoreApplication::translate("CypressMainWindow", "CDTT", nullptr));
        actionAudiometer_2->setText(QCoreApplication::translate("CypressMainWindow", "Audiometer", nullptr));
        actionCDTT_2->setText(QCoreApplication::translate("CypressMainWindow", "CDTT", nullptr));
        actionCCB->setText(QCoreApplication::translate("CypressMainWindow", "CCB", nullptr));
        actionGrip_Strength->setText(QCoreApplication::translate("CypressMainWindow", "Grip Strength ", nullptr));
        actionVividI->setText(QCoreApplication::translate("CypressMainWindow", "GE VividI ", nullptr));
        actionECG_2->setText(QCoreApplication::translate("CypressMainWindow", "ECG (MAC800)", nullptr));
        actionDEXA->setText(QCoreApplication::translate("CypressMainWindow", "DEXA", nullptr));
        actionBody_Comp->setText(QCoreApplication::translate("CypressMainWindow", "Body Comp", nullptr));
        actionFrax->setText(QCoreApplication::translate("CypressMainWindow", "Frax", nullptr));
        actionRetinal_Cam->setText(QCoreApplication::translate("CypressMainWindow", "Retinal Cam", nullptr));
        actionSignature->setText(QCoreApplication::translate("CypressMainWindow", "Signature", nullptr));
        actionTonometer->setText(QCoreApplication::translate("CypressMainWindow", "Tonometer", nullptr));
        actionWeigh_Scale->setText(QCoreApplication::translate("CypressMainWindow", "Weigh Scale", nullptr));
        actionSpirometer->setText(QCoreApplication::translate("CypressMainWindow", "Spirometer", nullptr));
        actionBlood_Pressure->setText(QCoreApplication::translate("CypressMainWindow", "Blood Pressure", nullptr));
        actionTest_Locally->setText(QCoreApplication::translate("CypressMainWindow", "Test Locally", nullptr));
        groupBox->setTitle(QCoreApplication::translate("CypressMainWindow", "Devices", nullptr));
        groupBox_5->setTitle(QCoreApplication::translate("CypressMainWindow", "Executable", nullptr));
        launchGripStrength->setText(QCoreApplication::translate("CypressMainWindow", "Grip Strength", nullptr));
        launchSpirometer->setText(QCoreApplication::translate("CypressMainWindow", "Spirometer", nullptr));
        launchRetinalCamera->setText(QCoreApplication::translate("CypressMainWindow", "Retinal Camera", nullptr));
        launchCDTT->setText(QCoreApplication::translate("CypressMainWindow", "CDTT", nullptr));
        launchEcg->setText(QCoreApplication::translate("CypressMainWindow", "ECG", nullptr));
        launchChoiceReaction->setText(QCoreApplication::translate("CypressMainWindow", "Choice Reaction", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("CypressMainWindow", "Serial Port", nullptr));
        launchAudiometer->setText(QCoreApplication::translate("CypressMainWindow", "Audiometer", nullptr));
        launchBpm->setText(QCoreApplication::translate("CypressMainWindow", "BPM", nullptr));
        launchWeightScale->setText(QCoreApplication::translate("CypressMainWindow", "Weight", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("CypressMainWindow", "DICOM", nullptr));
        launchUltrasound->setText(QCoreApplication::translate("CypressMainWindow", "CIMT", nullptr));
        launchDxa1->setText(QCoreApplication::translate("CypressMainWindow", "DXA (Hip)", nullptr));
        launchDxa2->setText(QCoreApplication::translate("CypressMainWindow", "DXA 2", nullptr));
        launchFrax->setText(QCoreApplication::translate("CypressMainWindow", "FRAX", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("CypressMainWindow", "Log", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CypressMainWindow: public Ui_CypressMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CYPRESS_MAIN_WINDOW_H

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
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
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
    QHBoxLayout *horizontalLayout;
    QPushButton *launchBpm;
    QPushButton *launchSpirometer;
    QPushButton *launchWeightScale;
    QPushButton *launchDxa1;
    QPushButton *launchDxa2;
    QPushButton *launchUltrasound;
    QPushButton *launchEcg;
    QPushButton *launchFrax;
    QSpacerItem *horizontalSpacer_2;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *launchAudiometer;
    QSpacerItem *horizontalSpacer;
    QGroupBox *groupBox_3;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *launchGripStrength;
    QPushButton *launchRetinalCamera;
    QPushButton *launchChoiceReaction;
    QPushButton *launchCDTT;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *verticalSpacer;
    QMenuBar *menubar;
    QMenu *menuDevice_Settings;
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
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        launchBpm = new QPushButton(groupBox);
        launchBpm->setObjectName(QString::fromUtf8("launchBpm"));

        horizontalLayout->addWidget(launchBpm);

        launchSpirometer = new QPushButton(groupBox);
        launchSpirometer->setObjectName(QString::fromUtf8("launchSpirometer"));

        horizontalLayout->addWidget(launchSpirometer);

        launchWeightScale = new QPushButton(groupBox);
        launchWeightScale->setObjectName(QString::fromUtf8("launchWeightScale"));

        horizontalLayout->addWidget(launchWeightScale);

        launchDxa1 = new QPushButton(groupBox);
        launchDxa1->setObjectName(QString::fromUtf8("launchDxa1"));
        launchDxa1->setEnabled(true);

        horizontalLayout->addWidget(launchDxa1);

        launchDxa2 = new QPushButton(groupBox);
        launchDxa2->setObjectName(QString::fromUtf8("launchDxa2"));

        horizontalLayout->addWidget(launchDxa2);

        launchUltrasound = new QPushButton(groupBox);
        launchUltrasound->setObjectName(QString::fromUtf8("launchUltrasound"));

        horizontalLayout->addWidget(launchUltrasound);

        launchEcg = new QPushButton(groupBox);
        launchEcg->setObjectName(QString::fromUtf8("launchEcg"));

        horizontalLayout->addWidget(launchEcg);

        launchFrax = new QPushButton(groupBox);
        launchFrax->setObjectName(QString::fromUtf8("launchFrax"));

        horizontalLayout->addWidget(launchFrax);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setEnabled(true);
        horizontalLayout_2 = new QHBoxLayout(groupBox_2);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        launchAudiometer = new QPushButton(groupBox_2);
        launchAudiometer->setObjectName(QString::fromUtf8("launchAudiometer"));
        launchAudiometer->setEnabled(true);

        horizontalLayout_2->addWidget(launchAudiometer);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        verticalLayout->addWidget(groupBox_2);

        groupBox_3 = new QGroupBox(centralwidget);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setEnabled(true);
        horizontalLayout_3 = new QHBoxLayout(groupBox_3);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        launchGripStrength = new QPushButton(groupBox_3);
        launchGripStrength->setObjectName(QString::fromUtf8("launchGripStrength"));
        launchGripStrength->setEnabled(false);

        horizontalLayout_3->addWidget(launchGripStrength);

        launchRetinalCamera = new QPushButton(groupBox_3);
        launchRetinalCamera->setObjectName(QString::fromUtf8("launchRetinalCamera"));
        launchRetinalCamera->setEnabled(false);

        horizontalLayout_3->addWidget(launchRetinalCamera);

        launchChoiceReaction = new QPushButton(groupBox_3);
        launchChoiceReaction->setObjectName(QString::fromUtf8("launchChoiceReaction"));

        horizontalLayout_3->addWidget(launchChoiceReaction);

        launchCDTT = new QPushButton(groupBox_3);
        launchCDTT->setObjectName(QString::fromUtf8("launchCDTT"));
        launchCDTT->setEnabled(true);

        horizontalLayout_3->addWidget(launchCDTT);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);


        verticalLayout->addWidget(groupBox_3);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        CypressMainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(CypressMainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 22));
        menuDevice_Settings = new QMenu(menubar);
        menuDevice_Settings->setObjectName(QString::fromUtf8("menuDevice_Settings"));
        CypressMainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(CypressMainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        CypressMainWindow->setStatusBar(statusbar);

        menubar->addAction(menuDevice_Settings->menuAction());
        menuDevice_Settings->addAction(actionAudiometer_2);
        menuDevice_Settings->addAction(actionCDTT_2);
        menuDevice_Settings->addAction(actionCCB);
        menuDevice_Settings->addAction(actionGrip_Strength);
        menuDevice_Settings->addAction(actionVividI);
        menuDevice_Settings->addAction(actionECG_2);
        menuDevice_Settings->addAction(actionDEXA);
        menuDevice_Settings->addAction(actionBody_Comp);
        menuDevice_Settings->addAction(actionFrax);
        menuDevice_Settings->addAction(actionRetinal_Cam);
        menuDevice_Settings->addAction(actionSignature);
        menuDevice_Settings->addAction(actionTonometer);
        menuDevice_Settings->addAction(actionWeigh_Scale);
        menuDevice_Settings->addAction(actionSpirometer);
        menuDevice_Settings->addAction(actionBlood_Pressure);
        menuDevice_Settings->addSeparator();
        menuDevice_Settings->addAction(actionTest_Locally);

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
        groupBox->setTitle(QCoreApplication::translate("CypressMainWindow", "Room 1", nullptr));
        launchBpm->setText(QCoreApplication::translate("CypressMainWindow", "BPM", nullptr));
        launchSpirometer->setText(QCoreApplication::translate("CypressMainWindow", "Spirometer", nullptr));
        launchWeightScale->setText(QCoreApplication::translate("CypressMainWindow", "Weight", nullptr));
        launchDxa1->setText(QCoreApplication::translate("CypressMainWindow", "DXA (Hip)", nullptr));
        launchDxa2->setText(QCoreApplication::translate("CypressMainWindow", "DXA 2", nullptr));
        launchUltrasound->setText(QCoreApplication::translate("CypressMainWindow", "CIMT", nullptr));
        launchEcg->setText(QCoreApplication::translate("CypressMainWindow", "ECG", nullptr));
        launchFrax->setText(QCoreApplication::translate("CypressMainWindow", "FRAX", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("CypressMainWindow", "Room 2", nullptr));
        launchAudiometer->setText(QCoreApplication::translate("CypressMainWindow", "Audiometer", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("CypressMainWindow", "Room 3", nullptr));
        launchGripStrength->setText(QCoreApplication::translate("CypressMainWindow", "Grip Strength", nullptr));
        launchRetinalCamera->setText(QCoreApplication::translate("CypressMainWindow", "Retinal Camera", nullptr));
        launchChoiceReaction->setText(QCoreApplication::translate("CypressMainWindow", "Choice Reaction", nullptr));
        launchCDTT->setText(QCoreApplication::translate("CypressMainWindow", "CDTT", nullptr));
        menuDevice_Settings->setTitle(QCoreApplication::translate("CypressMainWindow", "Device Settings", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CypressMainWindow: public Ui_CypressMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CYPRESS_MAIN_WINDOW_H

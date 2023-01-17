/********************************************************************************
** Form generated from reading UI file 'dxadialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DXADIALOG_H
#define UI_DXADIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DXADialog
{
public:
    QVBoxLayout *verticalLayout_5;
    QVBoxLayout *page;
    QGroupBox *form;
    QVBoxLayout *verticalLayout_6;
    QVBoxLayout *fields;
    QVBoxLayout *display;
    QHBoxLayout *participant;
    QLabel *participantLabel;
    QLabel *participantValue;
    QHBoxLayout *scan;
    QLabel *scanLabel;
    QLabel *scanValue;
    QHBoxLayout *status;
    QLabel *statusLabel;
    QLabel *statusValue;
    QVBoxLayout *buttons;
    QHBoxLayout *buttonRow;
    QSpacerItem *horizontalSpacer;
    QPushButton *collectButton;
    QPushButton *submitButton;
    QTabWidget *tabs;
    QWidget *filesTab;
    QHBoxLayout *horizontalLayout;
    QTextBrowser *filesList;
    QWidget *logTab;
    QVBoxLayout *verticalLayout_2;
    QTextBrowser *logBrowser;
    QPushButton *openFileExplorer;
    QWidget *optionsTab;

    void setupUi(QWidget *DXADialog)
    {
        if (DXADialog->objectName().isEmpty())
            DXADialog->setObjectName(QString::fromUtf8("DXADialog"));
        DXADialog->resize(467, 445);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(DXADialog->sizePolicy().hasHeightForWidth());
        DXADialog->setSizePolicy(sizePolicy);
        verticalLayout_5 = new QVBoxLayout(DXADialog);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        page = new QVBoxLayout();
        page->setObjectName(QString::fromUtf8("page"));
        form = new QGroupBox(DXADialog);
        form->setObjectName(QString::fromUtf8("form"));
        verticalLayout_6 = new QVBoxLayout(form);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        fields = new QVBoxLayout();
        fields->setObjectName(QString::fromUtf8("fields"));
        display = new QVBoxLayout();
        display->setObjectName(QString::fromUtf8("display"));
        participant = new QHBoxLayout();
        participant->setObjectName(QString::fromUtf8("participant"));
        participantLabel = new QLabel(form);
        participantLabel->setObjectName(QString::fromUtf8("participantLabel"));
        participantLabel->setMinimumSize(QSize(100, 0));
        participantLabel->setMaximumSize(QSize(100, 16777215));
        participantLabel->setFrameShape(QFrame::NoFrame);

        participant->addWidget(participantLabel);

        participantValue = new QLabel(form);
        participantValue->setObjectName(QString::fromUtf8("participantValue"));

        participant->addWidget(participantValue);


        display->addLayout(participant);

        scan = new QHBoxLayout();
        scan->setObjectName(QString::fromUtf8("scan"));
        scanLabel = new QLabel(form);
        scanLabel->setObjectName(QString::fromUtf8("scanLabel"));
        scanLabel->setMinimumSize(QSize(100, 0));
        scanLabel->setMaximumSize(QSize(100, 16777215));

        scan->addWidget(scanLabel);

        scanValue = new QLabel(form);
        scanValue->setObjectName(QString::fromUtf8("scanValue"));

        scan->addWidget(scanValue);


        display->addLayout(scan);

        status = new QHBoxLayout();
        status->setObjectName(QString::fromUtf8("status"));
        statusLabel = new QLabel(form);
        statusLabel->setObjectName(QString::fromUtf8("statusLabel"));
        statusLabel->setMinimumSize(QSize(100, 0));
        statusLabel->setMaximumSize(QSize(100, 16777215));

        status->addWidget(statusLabel);

        statusValue = new QLabel(form);
        statusValue->setObjectName(QString::fromUtf8("statusValue"));

        status->addWidget(statusValue);


        display->addLayout(status);


        fields->addLayout(display);


        verticalLayout_6->addLayout(fields);

        buttons = new QVBoxLayout();
        buttons->setObjectName(QString::fromUtf8("buttons"));
        buttonRow = new QHBoxLayout();
        buttonRow->setObjectName(QString::fromUtf8("buttonRow"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        buttonRow->addItem(horizontalSpacer);

        collectButton = new QPushButton(form);
        collectButton->setObjectName(QString::fromUtf8("collectButton"));
        collectButton->setEnabled(true);

        buttonRow->addWidget(collectButton);

        submitButton = new QPushButton(form);
        submitButton->setObjectName(QString::fromUtf8("submitButton"));
        submitButton->setEnabled(false);

        buttonRow->addWidget(submitButton);


        buttons->addLayout(buttonRow);


        verticalLayout_6->addLayout(buttons);


        page->addWidget(form);

        tabs = new QTabWidget(DXADialog);
        tabs->setObjectName(QString::fromUtf8("tabs"));
        tabs->setTabPosition(QTabWidget::North);
        tabs->setTabShape(QTabWidget::Rounded);
        tabs->setElideMode(Qt::ElideNone);
        tabs->setDocumentMode(false);
        tabs->setMovable(true);
        filesTab = new QWidget();
        filesTab->setObjectName(QString::fromUtf8("filesTab"));
        horizontalLayout = new QHBoxLayout(filesTab);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        filesList = new QTextBrowser(filesTab);
        filesList->setObjectName(QString::fromUtf8("filesList"));

        horizontalLayout->addWidget(filesList);

        tabs->addTab(filesTab, QString());
        logTab = new QWidget();
        logTab->setObjectName(QString::fromUtf8("logTab"));
        logTab->setLayoutDirection(Qt::LeftToRight);
        verticalLayout_2 = new QVBoxLayout(logTab);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        logBrowser = new QTextBrowser(logTab);
        logBrowser->setObjectName(QString::fromUtf8("logBrowser"));
        logBrowser->setFrameShape(QFrame::StyledPanel);
        logBrowser->setFrameShadow(QFrame::Plain);
        logBrowser->setAutoFormatting(QTextEdit::AutoBulletList);

        verticalLayout_2->addWidget(logBrowser);

        openFileExplorer = new QPushButton(logTab);
        openFileExplorer->setObjectName(QString::fromUtf8("openFileExplorer"));

        verticalLayout_2->addWidget(openFileExplorer);

        tabs->addTab(logTab, QString());
        optionsTab = new QWidget();
        optionsTab->setObjectName(QString::fromUtf8("optionsTab"));
        tabs->addTab(optionsTab, QString());

        page->addWidget(tabs);


        verticalLayout_5->addLayout(page);


        retranslateUi(DXADialog);

        tabs->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(DXADialog);
    } // setupUi

    void retranslateUi(QWidget *DXADialog)
    {
        DXADialog->setWindowTitle(QCoreApplication::translate("DXADialog", "CLSA Cypress", nullptr));
        form->setTitle(QCoreApplication::translate("DXADialog", "DEXA", nullptr));
        participantLabel->setText(QCoreApplication::translate("DXADialog", "Participant ID: ", nullptr));
        participantValue->setText(QCoreApplication::translate("DXADialog", "100125263", nullptr));
        scanLabel->setText(QCoreApplication::translate("DXADialog", "Scan:", nullptr));
        scanValue->setText(QCoreApplication::translate("DXADialog", "Hip (B)", nullptr));
        statusLabel->setText(QCoreApplication::translate("DXADialog", "Status:", nullptr));
        statusValue->setText(QCoreApplication::translate("DXADialog", "Incomplete (1/5)", nullptr));
        collectButton->setText(QCoreApplication::translate("DXADialog", "Collect", nullptr));
        submitButton->setText(QCoreApplication::translate("DXADialog", "Submit", nullptr));
        tabs->setTabText(tabs->indexOf(filesTab), QCoreApplication::translate("DXADialog", "Files", nullptr));
        openFileExplorer->setText(QCoreApplication::translate("DXADialog", "View Files", nullptr));
        tabs->setTabText(tabs->indexOf(logTab), QCoreApplication::translate("DXADialog", "Log", nullptr));
        tabs->setTabText(tabs->indexOf(optionsTab), QCoreApplication::translate("DXADialog", "Options", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DXADialog: public Ui_DXADialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DXADIALOG_H

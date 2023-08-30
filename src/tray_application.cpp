#include "tray_application.h"

#include <QStyle>
#include <QMainWindow>

TrayApplication::TrayApplication(QMainWindow* mainWidget, QStyle* style, QObject *parent)
    : QObject{parent}, m_mainWidget(mainWidget)
{
    m_trayIcon = new QSystemTrayIcon(this);
    m_trayMenu = new QMenu();

    QAction *quitAction = new QAction(tr("Quit"), this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
    m_trayMenu->addAction(quitAction);

    m_trayIcon->setIcon(style->standardIcon(QStyle::SP_ComputerIcon));
    m_trayIcon->setToolTip("CLSA Cypress");
    m_trayIcon->setContextMenu(m_trayMenu);
    m_trayIcon->show();

    connect(m_trayIcon, &QSystemTrayIcon::activated, this, &TrayApplication::onTrayActivated);
}


void TrayApplication::show()
{
    m_trayIcon->show();
}

void TrayApplication::onTrayActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        m_mainWidget->showNormal();
        break;
    default:
        break;
    }
}

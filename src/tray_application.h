#ifndef TRAYAPPLICATION_H
#define TRAYAPPLICATION_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QCloseEvent>
#include <QWidget>
#include <QCoreApplication>

class TrayApplication : public QObject
{
    Q_OBJECT
public:
    explicit TrayApplication(QWidget* mainWidget, QObject *parent = nullptr);

    void show();

private slots:
    void onTrayActivated(QSystemTrayIcon::ActivationReason reason);

private:
    QWidget* m_mainWidget;
    QSystemTrayIcon* m_trayIcon;
    QMenu* m_trayMenu;
};

#endif // TRAYAPPLICATION_H

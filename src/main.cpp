#include "cypress_application.h"
#include "cypress_session.h"
#include "cypress_settings.h"
#include "cypress_main_window.h"

#include "tray_application.h"

#include "managers/blood_pressure/bptru_200_driver.h"

#include "auxiliary/Constants.h"
#include "auxiliary/json_settings.h"

#include <QApplication>
#include <QStyle>
#include <QException>
#include <QSettings>
#include <QLocale>
#include <QTranslator>
#include <QMessageBox>
#include <QDebug>
#include <QProcess>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMetaType>
#include <QDir>
#include <QSysInfo>
#include <QHostInfo>
#include <QNetworkInterface>

#include <stdio.h>
#include <stdlib.h>



// Custom message handler to write to a log file and stdout
void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString txt = qFormatLogMessage(type, context, msg);
    static FILE *f = fopen("./log.txt", "a");
    static QTextStream ts( stdout );

    if (QtMsgType::QtSystemMsg) {
        fprintf(f, "SYSTEM [%s] %s\n",
            qPrintable(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")),
            qPrintable(txt)
        );
    }
    else if (QtMsgType::QtInfoMsg) {
        fprintf(f, "INFO [%s] %s\n",
            qPrintable(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")),
            qPrintable(txt)
        );
    }
    else if (QtMsgType::QtWarningMsg) {
        fprintf(f, "WARNING [%s] %s\n",
            qPrintable(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")),
            qPrintable(txt)
        );
    }
    else if (QtMsgType::QtCriticalMsg) {
        fprintf(f, "CRITICAL [%s] %s\n",
            qPrintable(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")),
            qPrintable(txt)
        );
    }
    else if (QtMsgType::QtFatalMsg) {
        fprintf(f, "FATAL [%s] %s\n",
            qPrintable(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")),
            qPrintable(txt)
        );
    }

    fflush(f);

    // print to console
    ts << msg << "\n";
    ts.flush();
}

// Removes old log files (>= 30 days old)
void cleanupLogs()
{
    QString path = "./log.txt";
    QFileInfo fileInfo(path);

    qDebug() << "log file created: " << fileInfo.birthTime().toString();
    if (fileInfo.exists()) {
        QFile file(path);

        if (fileInfo.birthTime().date().addDays(30) < QDate::currentDate()) {
            file.remove();
        }

        if (fileInfo.size() >= 100000000) { // 100mb
            file.remove();
        }
    }
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(messageHandler);
    cleanupLogs();

    // Setup Qt and defaults
    QGuiApplication::setOrganizationName("CLSA");
    QGuiApplication::setOrganizationDomain("clsa-elcv.ca");
    QGuiApplication::setApplicationName("Cypress");
    QGuiApplication::setApplicationVersion("1.0.1");

    QGuiApplication::setQuitOnLastWindowClosed(false);

    qRegisterMetaType<Constants::MeasureType>("Constants::MeasureType");
    qRegisterMetaType<QSharedPointer<CypressSession>>("QSharedPointer<CypressSession>");
    qRegisterMetaType<QSharedPointer<CypressSession>>("QSharedPointer<TestBase>");
    qRegisterMetaType<CypressSession*>("CypressSession*");
    qRegisterMetaType<BPMMessage>("BPMMessage");
    qRegisterMetaType<QQueue<BPMMessage>>("QQueue<BPMMessage>");

    QApplication app(argc, argv);
    CypressMainWindow* mainWindow = new CypressMainWindow;
    TrayApplication trayApp(mainWindow, app.style());

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "cypress_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }

    QJsonObject status = Cypress::getInstance().getStatus();
    QJsonObject deviceStatus = status;

    qDebug().noquote() << JsonSettings::prettyPrintJson(deviceStatus);

    Cypress::getInstance();

    return app.exec();
}

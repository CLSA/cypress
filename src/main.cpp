#include "cypress_application.h"
#include "cypress_session.h"
#include "cypress_settings.h"
#include "cypress_main_window.h"

#include "tray_application.h"

#include "managers/blood_pressure/bptru_200_driver.h"

#include "auxiliary/Constants.h"
#include "auxiliary/Utilities.h"

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

const QString orgName = "CLSA";
const QString orgDomain = "clsa-elcv.ca";
const QString appName = "Cypress";
const QString appVersion = "v0.1.0";

// Custom message handler to write to a log file and stdout
void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(context)

    QString txt;
    switch (type) {
    case QtDebugMsg:
        txt = QString("debug: ");
        break;
    case QtInfoMsg:
        txt = QString("info: ");
        break;
    case QtWarningMsg:
        txt = QString("warning: ");
        break;
    case QtCriticalMsg:
        txt = QString("critical: ");
        break;
    case QtFatalMsg:
        txt = QString("fatal: ");
    }

    txt += msg;
    txt.prepend(QDateTime::currentDateTime().toString("[dd.MM.yyyy hh:mm:ss] "));

    QString path = QDir::currentPath() + "/logs";
    QDir dir;
    bool success { dir.mkpath(path) };
    if (!success)
    {
        qFatal(("could not create log file directory at " + QDir::currentPath()).toStdString().c_str());
    }

    QFile outFile(path + "/log.txt");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);

    QTextStream ts(&outFile);
    ts << txt << "\n";

    fprintf(stdout, "%s\n", txt.toStdString().c_str());
    fflush(stdout);
}

// Removes old log files (>= 30 days old)
void cleanupLogs()
{
    QString path = QDir::currentPath() + "/logs";
    QDir dir(path);
    if (!dir.exists())
        return;

    QStringList logFiles = dir.entryList(QStringList() << "log*.txt", QDir::Files, QDir::Name);
    for(const QString &logFile : logFiles) {
        QFileInfo fileInfo(dir.absoluteFilePath(logFile));
        if (fileInfo.lastModified().date().addDays(30) < QDate::currentDate()) {
            dir.remove(logFile);
        }
    }
}

void restartApplication(const QStringList& arguments) {
    QProcess process;
    process.startDetached(arguments[0], arguments.mid(1));
    QCoreApplication::quit();
}

void logSystemInfo()
{
    qInfo() << "\nHost Information";
    qInfo() << QHostInfo::localHostName();
    qInfo() << QSysInfo::productType() + " "
               + QSysInfo::productVersion() + " ("
               + QSysInfo::kernelType() + " "
               + QSysInfo::kernelVersion() + " "
               + QSysInfo::currentCpuArchitecture() + ")";
}

void logNetworkInfo()
{
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    qInfo() << "\nNetwork Interfaces";
    for(const QNetworkInterface &interface : interfaces) {
        QList<QNetworkAddressEntry> entries = interface.addressEntries();
        for(const QNetworkAddressEntry &entry : entries) {
            if(entry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                qInfo() << entry.ip().toString();
            }
        }
    }
}

void logAppInfo()
{
    qInfo() << "\nCypress Information";
    qInfo() << "Name:" << QCoreApplication::applicationName();
    qInfo() << "Version: " << QCoreApplication::applicationVersion();
    qInfo() << "Working Directory: " << QCoreApplication::applicationDirPath();
    qInfo() << "Debug Mode: " << CypressSettings::isDebugMode();
    qInfo() << "Sim Mode: " << CypressSettings::isSimMode();

    qInfo() << "Server: " << CypressSettings::readSetting("address").toString() << ":"
            << CypressSettings::readSetting("port").toString();
}

QString getStatusCheckString(const QString& arg1, bool arg2)
{
    return QString("%1 %2")
        .arg(arg1, -20, ' ')
        .arg("[ " + (arg2 ? QString("true ") : QString("false")) + " ]", 20, ' ');
}

bool checkAlive()
{
    bool isAlive { true };

    qInfo() << getStatusCheckString("pine available", isAlive);

    return isAlive;
}

bool updateAvailable()
{
    bool isUpdateAvailable { false };

    qInfo() << getStatusCheckString("update available", isUpdateAvailable);

    return isUpdateAvailable;
}

bool downloadUpdate()
{
    return false;
}

int main(int argc, char *argv[])
{
    //qInstallMessageHandler(messageHandler);
    //cleanupLogs();

    // Setup Qt and defaults
    QGuiApplication::setOrganizationName(orgName);
    QGuiApplication::setOrganizationDomain(orgDomain);

    QGuiApplication::setApplicationName(appName);
    QGuiApplication::setApplicationVersion(appVersion);
    QGuiApplication::setApplicationVersion("1.0.0");
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

    logAppInfo();

    logSystemInfo();

    logNetworkInfo();

    // Make sure the network is up and that the Pine server is alive
    if (!checkAlive()) {
        qCritical() << "Could not connect to the Pine server";
        return -1;
    }

    // Check for and install any updates
    if (updateAvailable()) {
        if (downloadUpdate()) {
            restartApplication(QCoreApplication::arguments());
        } else {
            qCritical() << "Could not download a new update";
        }
    }

    qInfo() << "\nDevices Installed";
    QJsonObject status = Cypress::getInstance().getStatus();
    QJsonObject deviceStatus = status["availableInstruments"].toObject();
    for (auto it = deviceStatus.constBegin(); it != deviceStatus.constEnd(); ++it) {
        QString deviceName = it.key();
        bool available = it.value().toBool();

        qInfo() << getStatusCheckString(deviceName, available);
    }

    Cypress::getInstance();

    return app.exec();
}

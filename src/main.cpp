#include "cypress_application.h"

#include "managers/audiometer/audiometer_manager.h"
#include "managers/blood_pressure/blood_pressure_manager.h"
#include "managers/body_composition/body_composition_manager.h"
#include "managers/cdtt/cdtt_manager.h"
#include "managers/choice_reaction/choice_reaction_manager.h"


#include "cypress_main_window.h"
#include "tray_application.h"

#include "auxiliary/Constants.h"

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

    QStringList logFiles = dir.entryList(QStringList() << "log_*.txt", QDir::Files, QDir::Name);
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
    for(const QNetworkInterface &interface : interfaces) {
        qInfo() << interface.humanReadableName();
        QList<QNetworkAddressEntry> entries = interface.addressEntries();
        for(const QNetworkAddressEntry &entry : entries) {
            if(entry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                qInfo() << "\t" << entry.ip().toString();
            }
        }
    }
}

void logAppInfo()
{
    qInfo() << QCoreApplication::applicationName();
    qInfo() << QCoreApplication::applicationVersion();
    qInfo() << QCoreApplication::applicationDirPath();
}

QString getStatusCheckString(const QString& arg1, bool arg2)
{
    return QString("%1 %2").arg(arg1, -17).arg("[ " + (arg2 ? QString("true ") : QString("false")) + " ]");
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
    qInfo() << "downloading update...";
    return false;
}

int main(int argc, char *argv[])
{
    // Setup Qt and defaults
    QGuiApplication::setOrganizationName(orgName);
    QGuiApplication::setOrganizationDomain(orgDomain);

    QGuiApplication::setApplicationName(appName);
    QGuiApplication::setApplicationVersion(appVersion);
    QGuiApplication::setApplicationVersion("1.0");
    QGuiApplication::setQuitOnLastWindowClosed(false);

    qRegisterMetaType<Constants::MeasureType>("Constants::MeasureType");
    qRegisterMetaType<CypressSession>("CypressSession");

    qInstallMessageHandler(messageHandler);

    cleanupLogs();

    //if (!QSystemTrayIcon::isSystemTrayAvailable()) {
    //    qWarning("System tray is not available!");
    //    return -1;
    //}

    // Init app
    QApplication app(argc, argv);
    //CypressMainWindow* mainWindow = new CypressMainWindow;
    //TrayApplication trayApp(mainWindow, app.style());

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "cypress_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }

    CypressSettings::getInstance().setDefaultSettings();

    logAppInfo();
    logSystemInfo();
    logNetworkInfo();

    QCommandLineParser parser;
    parser.setApplicationDescription("Managers medical device data collection.");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption isDebug("d",
        QCoreApplication::translate("main", "run app in debug mode")
    );
    parser.addOption(isDebug);
    parser.process(app);


    if (parser.positionalArguments().length())
    {
        qInfo() << "args: ", parser.positionalArguments();
    }

    if (parser.isSet(isDebug))
    {
        qInfo("debug");
    }
    else
    {
        qInfo("production");
    }

    // Make sure the network is up and that the Pine server is alive
    if (!checkAlive())
    {
        qCritical() << "could not connect to the Pine server";
        return -1;
    }

    // Check for and install any updates
    if (updateAvailable())
    {
        if (downloadUpdate())
        {
            restartApplication(QCoreApplication::arguments());
        }
        else {
            qCritical() << "could not download a new update";
        }
    }

    Cypress::getInstance();

    return app.exec();
}

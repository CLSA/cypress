#include "cypress_application.h"

#include "managers/audiometer/audiometer_manager.h"
#include "managers/blood_pressure/blood_pressure_manager.h"
#include "managers/body_composition/body_composition_manager.h"
#include "managers/cdtt/cdtt_manager.h"
#include "managers/choice_reaction/choice_reaction_manager.h"

#include "auxiliary/Constants.h"

#include <QApplication>
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

QMap<QString, QVariant> defaultSettings = {{
    {"pine/host",                    ""},
    {"pine/port",                    ""},
    {"pine/username",                ""},
    {"pine/password",                ""},

    {"client/server/host",           "0.0.0.0"},
    {"client/server/port",           9000},
    {"client/server/args",           ""},

    {"grip_strength/working_dir",    "C:/Program Files/Tracker 5"},
    {"grip_strength/executable",     "WTS.exe"},
    {"grip_strength/database",       "Tracker 5 Data"},

    {"retina/patient_id", 		     "11111111-2222-3333-4444-555555555555"},
    {"retina/working_dir",           "C:/Program Files/TOPCON/IMAGEnet R4/"},
    {"retina/executable",            "imagenet.exe"},
    {"retina/database/driver", 	     "QODBC"},
    {"retina/database/host",         "localhost"},
    {"retina/database/database",     "IMAGEnet_R4"},
    {"retina/database/port",         1433},
    {"retina/database/username",     ""},
    {"retina/database/password",     ""},

    {"vividi/dicom/aeTitle",         "DICOMSTORAGESCP"},
    {"vividi/dicom/hostname",        "0.0.0.0"},
    {"vividi/dicom/port",            1100},
    {"vividi/dicom/asc_config",      ""},
    {"vividi/dicom/log_config",      ""},
    {"vividi/dicom/executable",      ""},

    {"dxa/dicom/aeTitle",            ""},
    {"dxa/dicom/host", 		         "0.0.0.0"},
    {"dxa/dicom/port", 		         9000},
    {"dxa/dicom/asc_config",         ""},
    {"dxa/dicom/log_config",         ""},
    {"dxa/dicom/executable",         "dcmrecv.exe"},

    {"dxa/patscandb/driver",         "QODBC"},
    {"dxa/patscandb/host",           ""},
    {"dxa/patscandb/port",           ""},
    {"dxa/patscandb/user", 	         ""},
    {"dxa/patscandb/password", 	     ""},
    {"dxa/patscandb/databaseName", 	 ""},

    {"dxa/refscandb/driver",         "QODBC"},
    {"dxa/refscandb/host",           ""},
    {"dxa/refscandb/port", 	         ""},
    {"dxa/refscandb/user", 	         ""},
    {"dxa/refscandb/password", 	     ""},
    {"dxa/refscandb/database", 	     ""},
}};

// .ini file can be found at C:\Users\<User>\AppData\Roaming\CLSA\Cypress.ini
bool setDefaultSettings()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");
    try {
        QMap<QString, QVariant>::const_iterator i;
        for (i = defaultSettings.constBegin(); i != defaultSettings.constEnd(); ++i) {
            if (!settings.contains(i.key())) {
                settings.setValue(i.key(), i.value());
            }
        }
    }
    catch (QException& e)
    {
        qDebug() << e.what();
        return false;
    }

    return true;
}


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

    // Clean up system

    cleanupLogs();

    // Init app
    QApplication app(argc, argv);
    QSettings(QSettings::IniFormat, QSettings::UserScope, orgName, appName);

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

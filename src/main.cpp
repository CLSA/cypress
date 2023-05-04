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

#include "CypressApplication.h"
//#include "managers/SettingsManager.h"
#include "auxiliary/CommandLineParser.h"
#include "auxiliary/Constants.h"

#include "logging/debugdialog.h"

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


void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(context);
    Q_UNUSED(type);

    static DebugDialog *debugDialog = nullptr;

    if (!debugDialog)
    {
        debugDialog = new DebugDialog();
        debugDialog->setWindowTitle("Debug");
        debugDialog->setWindowFlags(Qt::WindowFullscreenButtonHint);
        debugDialog->setMinimumWidth(400);
        debugDialog->show();
    }

    QDateTime currentDateTime = QDateTime::currentDateTime();

    // Create a new message with the timestamp and original message
    QString newMsg = currentDateTime.toString(Qt::ISODateWithMs) + " [" + QString::number(type) + "] " + msg;

    debugDialog->appendDebugMessage(newMsg);
}

void restartApplication(const QStringList& arguments) {
    QProcess process;
    process.startDetached(arguments[0], arguments.mid(1));
    QCoreApplication::quit();
}

int main(int argc, char *argv[])
{
    QGuiApplication::setOrganizationName(orgName);
    QGuiApplication::setOrganizationDomain(orgDomain);
    QGuiApplication::setApplicationName(appName);
    QGuiApplication::setApplicationVersion(appVersion);

    QApplication app(argc, argv);
    QStringList appArguments = QCoreApplication::arguments();

    qRegisterMetaType<Constants::MeasureType>("Constants::MeasureType");

    //if (updateAvailable())
    //{
    //    if (downloadUpdate())
    //    {
    //        restartApplication(appArguments);
    //    }
    //}

    //QTranslator translator;
    //const QStringList uiLanguages = QLocale::system().uiLanguages();
    //for (const QString &locale : uiLanguages) {
    //    const QString baseName = "cypress_" + QLocale(locale).name();
    //    if (translator.load(":/i18n/" + baseName)) {
    //        app.installTranslator(&translator);
    //        break;
    //    }
    //}

    // process command line args
    //
    //CommandLineParser parser;
    //QString errMessage;
    //switch(parser.parseCommandLine(app, &errMessage))
    //{
    //    case CommandLineParser::parseHelpRequested:
    //        parser.displayHelp(parser);
    //        return EXIT_SUCCESS;
    //    case CommandLineParser::parseVersionRequested:
    //        parser.displayHelp(parser);
    //        return EXIT_SUCCESS;
    //    case CommandLineParser::parseOk:
    //        break;
    //    case CommandLineParser::parseError:
    //    case CommandLineParser::parseInputFileError:
    //    case CommandLineParser::parseOutputPathError:
    //    case CommandLineParser::parseMissingArg:
    //    case CommandLineParser::parseMeasureTypeError:
    //    case CommandLineParser::parseRunModeError:
    //        parser.displayError(errMessage, parser);
    //        return EXIT_FAILURE;
    //}

    CypressApplication& cypress = CypressApplication::getInstance();
    //cypress.setArgs();

    return app.exec();
}

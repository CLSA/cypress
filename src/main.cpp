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

#include "CypressApplication.h"
#include "managers/SettingsManager.h"
#include "auxiliary/CommandLineParser.h"

#include "logging/debugdialog.h"

const QString orgName = "CLSA";
const QString orgDomain = "clsa-elcv.ca";
const QString appName = "Cypress";
const QString appVersion = "v0.1.0";

QMap<QString, QVariant> defaultSettings = {{
    {"pine/host", ""},
    {"pine/port", ""},

    {"rest_api/host", 	"0.0.0.0"},
    {"rest_api/port", 	8000},
    {"rest_api/args", 	""},

    {"dicom/host", 		  "0.0.0.0"},
    {"dicom/port", 		  9000},
    {"dicom/asc_config",  "C:/work/clsa/cypress/dcmtk-3.6.7/dcmnet/etc/storescp.cfg"},
    {"dicom/log_config",  "./logger.cfg"},
    {"dicom/out_dir", 	  "C:/Users/antho/Documents/PACS"},
    {"dicom/working_dir", "C:/ProgramData/chocolatey/bin"},
    {"dicom/program", 	  "C:/ProgramData/chocolatey/bin/dcmrecv.exe"},

    {"grip_strength/workingDir", 	"C:/Program Files (x86)/Tracker 5"},
    {"grip_strength/runnableName", 	"C:/Program Files (x86)/Tracker 5/WTS.exe"},
    {"grip_strength/databaseDir", 	"C:/Program Files (x86)/Tracker 5/Data/Tracker 5 Data"},
    {"grip_strength/databaseHost", 	"localhost"},
    {"grip_strength/databaseName",  "Tracker 5"},
    {"grip_strength/databasePort",  ""},
    {"grip_strength/databaseUser",  "Anthony"},
    {"grip_strength/databasePassword", ""},

    {"retinal_camera/patient_id", 		    "11111111-2222-3333-4444-555555555555"},
    {"retinal_camera/working_dir", 			"C:/Program Files/TOPCON/IMAGEnet R4/"},
    {"retinal_camera/executable", 			"imagenet.exe"},
    {"retinal_camera/database/version", 	"MSSQL Server 2008 R2 Express"},
    {"retinal_camera/database/driver", 	    "QODBC"},
    {"retinal_camera/database/host", 		"DSN=IMAGEnet_R4;"},
    {"retinal_camera/database/port", 		1433},
    {"retinal_camera/database/database", 	"IMAGEnet_R4"},

    {"dxa/patscandb/version", 	"MS Access"},
    {"dxa/patscandb/driver", 	"QODBC"},
    {"dxa/patscandb/host", 		""},
    {"dxa/patscandb/port", 		""},
    {"dxa/patscandb/user", 		""},
    {"dxa/patscandb/password", 	""},
    {"dxa/patscandb/database", 	""},

    {"dxa/refscandb/version", 	"MS Access"},
    {"dxa/refscandb/driver", 	"QODBC"},
    {"dxa/refscandb/host", 		""},
    {"dxa/refscandb/port", 		""},
    {"dxa/refscandb/user", 		""},
    {"dxa/refscandb/password", 	""},
    {"dxa/refscandb/database", 	""},
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

    debugDialog->appendDebugMessage(msg);
}

int main(int argc, char *argv[])
{
    QGuiApplication::setOrganizationName(orgName);
    QGuiApplication::setOrganizationDomain(orgDomain);
    QGuiApplication::setApplicationName(appName);
    QGuiApplication::setApplicationVersion(appVersion);

    QApplication app(argc, argv);
    QApplication::setQuitOnLastWindowClosed(false);

    qInstallMessageHandler(customMessageHandler);

    SettingsManager settingsManager;

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "cypress_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }

    // process command line args
    //
    CommandLineParser parser;
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

    CypressApplication cypress;
    //cypress.setArgs(parser.getArgs());
    cypress.initialize();

    return app.exec();
}

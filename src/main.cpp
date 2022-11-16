#include <QApplication>
#include <QSettings>
#include <QLocale>
#include <QTranslator>
#include <QMessageBox>
#include <QDebug>
#include <QProcess>

#include "CypressApplication.h"
#include "server/Server.h"
#include "auxiliary/CommandLineParser.h"

const QString orgName = "CLSA";
const QString orgDomain = "clsa-elcv.ca";
const QString appName = "Cypress";
const QString appVersion = "v0.1.0";

const QMap<QString, QVariant> defaultSettings = {{
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

    {"grip_strength/exe", 		"WTS.exe"},
    {"grip_strength/dir", 		"C:/Program Files (x86}/Tracker 5"},
    {"grip_strength/tests", 	"C:/work/clsa/cypress/GripStrengthData/ZGripTest_After_Test.DB"},
    {"grip_strength/results", 	"C:/work/clsa/cypress/GripStrengthData/ZGripTestData_After_Test.DB"},

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

    {"retinal_scan/patient_id", 		"11111111-2222-3333-4444-555555555555"},
    {"retinal_scan/exe", 				""},
    {"retinal_scan/database/version", 	"MSSQL Server 2008 R2 Express"},
    {"retinal_scan/database/driver", 	"QODBC"},
    {"retinal_scan/database/host", 		"127.0.0.1"},
    {"retinal_scan/database/port", 		9000},
    {"retinal_scan/database/user", 		"Anthony"},
    {"retinal_scan/database/password", 	""},
    {"retinal_scan/database/database", 	"dbo"},
}};

void displayError(const QString errMessage, CommandLineParser& parser)
{
    QMessageBox::warning(
        nullptr,
        QGuiApplication::applicationDisplayName(),
        "<html><head/><body><h2>" + errMessage + "</h2><pre>" + parser.helpText() + "</pre></body></html>"
    );
}

void displayHelp(CommandLineParser& parser)
{
    QMessageBox::warning(
        nullptr,
        QGuiApplication::applicationDisplayName(),
        "<html><head/><body><pre>" + parser.helpText() + "</pre></body></html>"
    );
}

// .ini file can be found at C:\Users\<User>\AppData\Roaming\CLSA\Cypress.ini
void syncDefaultSettings(QSettings& settings)
{
    QMap<QString, QVariant>::const_iterator i;
    for (i = defaultSettings.constBegin(); i != defaultSettings.constEnd(); ++i) {
        if (!settings.contains(i.key())) {
            settings.setValue(i.key(), i.value());
        }
    }
}

// WIP extern
Server* server = new Server();

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName(orgName);
    QCoreApplication::setOrganizationDomain(orgDomain);
    QCoreApplication::setApplicationName(appName);
    QCoreApplication::setApplicationVersion(appVersion);

    QApplication app(argc, argv);
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");
    syncDefaultSettings(settings);

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
    QString errMessage;
    switch(parser.parseCommandLine(app, &errMessage))
    {
        case CommandLineParser::parseHelpRequested:
            displayHelp(parser);
            return EXIT_SUCCESS;
        case CommandLineParser::parseVersionRequested:
            displayHelp(parser);
            return EXIT_SUCCESS;
        case CommandLineParser::parseOk:
            break;

        case CommandLineParser::parseError:
        case CommandLineParser::parseInputFileError:
        case CommandLineParser::parseOutputPathError:
        case CommandLineParser::parseMissingArg:
        case CommandLineParser::parseMeasureTypeError:
        case CommandLineParser::parseRunModeError:
            displayError(errMessage, parser);
            return EXIT_FAILURE;
    }

    CypressApplication cypress;
    cypress.setArgs(parser.getArgs());
    cypress.initialize();

    // Runs in separate thread
    server->start();

    return app.exec();
}

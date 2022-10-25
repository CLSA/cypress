#include <QApplication>
#include <QSettings>
#include <QLocale>
#include <QTranslator>
#include <QMessageBox>
#include <QDebug>

#include "CypressApplication.h"
#include "rest/server/server.h"
#include "./auxiliary/CommandLineParser.h"


void displayError(const QString errMessage, CommandLineParser& parser) {
    QMessageBox::warning(
        nullptr,
        QGuiApplication::applicationDisplayName(),
        "<html><head/><body><h2>" + errMessage + "</h2><pre>" + parser.helpText() + "</pre></body></html>"
    );
}

void displayHelp(CommandLineParser& parser) {
    QMessageBox::warning(
        nullptr,
        QGuiApplication::applicationDisplayName(),
        "<html><head/><body><pre>" + parser.helpText() + "</pre></body></html>"
    );
}

// On windows, .ini file can be found at C:\Users\<User>\AppData\Roaming\CLSA\Cypress.ini
//
void initializeDefaultSettings(QSettings& settings) {
    // .ini file already exists on system, don't overwrite
    //if (settings.value("dicom/port").toInt())
    //{
    //    qDebug() << ".ini file exists";
    //    return;
    //}

    settings.setValue("pine/host", "");
    settings.setValue("pine/port", "");

    settings.setValue("rest_api/host", "127.0.0.1");
    settings.setValue("rest_api/port", 9000);
    settings.setValue("rest_api/args", "");

    settings.setValue("dicom/port", 9000);
    settings.setValue("dicom/host", "127.0.0.1");
    settings.setValue("dicom/asc_config", "./storescp.cfg");
    settings.setValue("dicom/log_config", "./logger.cfg");
    settings.setValue("dicom/out_dir", "C:/Users/Anthony/Documents/PACS");
    settings.setValue("dicom/args", "--series-date-subdir --short-unique-names --filename-extension .dcm");

    settings.setValue("instruments/grip_strength/exe", "WTS.exe");
    settings.setValue("instruments/grip_strength/dir", "C:/Program Files (x86)/Tracker 5");
    settings.setValue("instruments/grip_strength/tests", "C:/work/clsa/cypress/GripStrengthData/ZGripTest_After_Test.DB");
    settings.setValue("instruments/grip_strength/results", "C:/work/clsa/cypress/GripStrengthData/ZGripTestData_After_Test.DB");

    // default patient id to use (db is cleared each time)
    settings.setValue("instruments/retinal_scan/patient_id", "11111111-2222-3333-4444-555555555555");
    settings.setValue("instruments/retinal_scan/exe", "C:/Program Files/Mozilla Firefox/firefox.exe");
    settings.setValue("instruments/retinal_scan/database/version", "MSSQL Server 2008 R2 Express");
    settings.setValue("instruments/retinal_scan/database/driver", "QODBC");
    settings.setValue("instruments/retinal_scan/database/host", "127.0.0.1");
    settings.setValue("instruments/retinal_scan/database/port", 9000);
    settings.setValue("instruments/retinal_scan/database/user", "Anthony");
    settings.setValue("instruments/retinal_scan/database/password", "");
    settings.setValue("instruments/retinal_scan/database/database", "dbo");
}

// WIP extern
Server* server = new Server();

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("CLSA");
    QCoreApplication::setOrganizationDomain("clsa-elcv.ca");
    QCoreApplication::setApplicationName("Cypress");
    QCoreApplication::setApplicationVersion("1.0.0");

    QApplication app(argc, argv);
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");
    initializeDefaultSettings(settings);

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
    //CommandLineParser parser;
    //QString errMessage;
    //switch(parser.parseCommandLine(app, &errMessage))
    //{
    //    case CommandLineParser::parseHelpRequested:
    //        displayHelp(parser);
    //        return EXIT_SUCCESS;
    //    case CommandLineParser::parseVersionRequested:
    //        displayHelp(parser);
    //        return EXIT_SUCCESS;
    //    case CommandLineParser::parseOk:
    //        break;

    //    case CommandLineParser::parseError:
    //    case CommandLineParser::parseInputFileError:
    //    case CommandLineParser::parseOutputPathError:
    //    case CommandLineParser::parseMissingArg:
    //    case CommandLineParser::parseMeasureTypeError:
    //    case CommandLineParser::parseRunModeError:
    //        displayError(errMessage, parser);
    //        return EXIT_FAILURE;
    //}

    CypressApplication cypress;

    // Runs in separate thread
    server->start();


    return app.exec();
}

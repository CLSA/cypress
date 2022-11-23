#include <QApplication>
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
#include "server/Server.h"
#include "auxiliary/CommandLineParser.h"

const QString orgName = "CLSA";
const QString orgDomain = "clsa-elcv.ca";
const QString appName = "Cypress";
const QString appVersion = "v0.1.0";

Server* restApiServer = new Server();

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName(orgName);
    QCoreApplication::setOrganizationDomain(orgDomain);
    QCoreApplication::setApplicationName(appName);
    QCoreApplication::setApplicationVersion(appVersion);

    QApplication app(argc, argv);
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
    QString errMessage;
    switch(parser.parseCommandLine(app, &errMessage))
    {
        case CommandLineParser::parseHelpRequested:
            parser.displayHelp(parser);
            return EXIT_SUCCESS;
        case CommandLineParser::parseVersionRequested:
            parser.displayHelp(parser);
            return EXIT_SUCCESS;
        case CommandLineParser::parseOk:
            break;
        case CommandLineParser::parseError:
        case CommandLineParser::parseInputFileError:
        case CommandLineParser::parseOutputPathError:
        case CommandLineParser::parseMissingArg:
        case CommandLineParser::parseMeasureTypeError:
        case CommandLineParser::parseRunModeError:
            parser.displayError(errMessage, parser);
            return EXIT_FAILURE;
    }

    CypressApplication cypress;
    cypress.setArgs(parser.getArgs());
    cypress.initialize();

    restApiServer->start();

    return app.exec();
}

#include <QApplication>
#include <QSettings>
#include <QLocale>
#include <QTranslator>
#include <QMessageBox>
#include <QDebug>
<<<<<<< HEAD
#include "./server/server.h"

#include "./auxiliary/Constants.h"
=======
>>>>>>> aeed5d7 (moved server code to its own subproject to be run independently to prevent blocking main loop, applications now launch on network request, added default request (404) handler, regex for matching request URIs to the appropriate handler)

#include "CypressApplication.h"
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

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("CLSA");
    QCoreApplication::setOrganizationDomain("clsa-elcv.ca");
    QCoreApplication::setApplicationName("Cypress");
    QCoreApplication::setApplicationVersion("1.0.0");
    QApplication app(argc, argv);
    QSettings settings;

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

    // Runs in separate thread
    //Server restApiServer;
    //const QThread& thread = restApiServer.getThread();



    CypressApplication cypress;
    try
    {
        cypress.setArgs(parser.getArgs());
        cypress.initialize();
    }
    catch (std::exception& e)
    {
        qDebug() << e.what();
        QMessageBox::critical(0, QGuiApplication::applicationDisplayName(),
                              "<html><head/><body><h2>" + QString(e.what()) + "</h2></body></html>");
        return EXIT_FAILURE;
    }

    //restApiServer.start();
    return app.exec();
}

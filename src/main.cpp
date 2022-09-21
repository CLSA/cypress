#include "CypressApplication.h"

#include "./auxiliary/CommandLineParser.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QMessageBox>
#include <QDebug>

#include "./auxiliary/Constants.h"


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
    try
    {
        qDebug() << "initialize start";
        cypress.setArgs(parser.getArgs());
        cypress.initialize();
        qDebug() << "initialize end";
    }
    catch (std::exception& e)
    {
        qDebug() << e.what();
        QMessageBox::critical(0, QGuiApplication::applicationDisplayName(),
                              "<html><head/><body><h2>" + QString(e.what()) + "</h2></body></html>");
        return EXIT_FAILURE;
    }

    return app.exec();
}

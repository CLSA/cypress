#ifndef COMMAND_LINE_PARSER_H
#define COMMAND_LINE_PARSER_H

#include <QCommandLineParser>
#include <QCoreApplication>
#include <QMessageBox>
#include <QGuiApplication>
#include <QVariant>

class CommandLineParser : QObject
{
    Q_OBJECT
public:
    explicit CommandLineParser(QObject* parent = Q_NULLPTR);

    enum ParseResult {
        parseOk,
        parseError,
        parseMissingArg,
        parseRunModeError,
        parseInputFileError,
        parseOutputPathError,
        parseMeasureTypeError,
        parseVersionRequested,
        parseHelpRequested
    };
    Q_ENUM(ParseResult)

    QString helpText(){ return m_parser.helpText(); }

    ParseResult parseCommandLine( const QCoreApplication&, QString*);

    QVariantMap getArgs(){ return m_args; }

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
private:
    QCommandLineParser m_parser;
    QVariantMap m_args;
};

#endif // COMMAND_LINE_PARSER_H

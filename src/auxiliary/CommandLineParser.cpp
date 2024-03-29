#include "CommandLineParser.h"

#include "Constants.h"
#include <QCommandLineOption>
#include <QDebug>
#include <QDir>
#include <QFileInfo>

CommandLineParser::CommandLineParser(QObject* parent) : QObject(parent)
{
   m_args["verbose"] = QVariant(true);
   m_args["runMode"] = QVariant::fromValue(Constants::RunMode::modeUnknown);
   m_args["inputFileName"] = QVariant();
   m_args["outputFileName"] = QVariant();
   m_args["measureType"] = QVariant::fromValue(Constants::MeasureType::typeUnknown);
}

CommandLineParser::ParseResult CommandLineParser::parseCommandLine(
        const QCoreApplication &app,
         QString *errMessage)
{
    // process command line args
    // if the run mode arg is 'test' and no UI is required
    // do not open the window, just write dummy output data to json
    //

    const QCommandLineOption helpOption = m_parser.addHelpOption();
    const QCommandLineOption versionOption = m_parser.addVersionOption();

    // expect a full path to an input.json file
    // which contains the minimum necessary input data to
    // run the device (eg., barcode, gender etc.)

    QCommandLineOption inputOption(
      QStringList() << "i" << "input",
      QCoreApplication::translate(
        "main", "Read json input from <file>"),"file");
    m_parser.addOption(inputOption);

    QCommandLineOption outputOption(
      QStringList() << "o" << "output",
      QCoreApplication::translate(
        "main", "Write json output to <file>"),"file");
    m_parser.addOption(outputOption);

    QCommandLineOption typeOption(
      QStringList() << "t" << "type",
      QCoreApplication::translate(
        "main", "Measure type <type>"),"type","none");
    m_parser.addOption(typeOption);

    QCommandLineOption modeOption(
      QStringList() << "m" << "mode",
      QCoreApplication::translate(
        "main", "Run mode <default,live,simulate>"),"mode","default");
    m_parser.addOption(modeOption);

    QCommandLineOption verboseOption(
       QStringList() << "d" << "verbose",
       QCoreApplication::translate(
         "main","Verbose mode. Prints out verbose debug information."));
    m_parser.addOption(verboseOption);

    m_parser.process(app);

    if (m_parser.isSet(versionOption))
        return parseVersionRequested;

    if (m_parser.isSet(helpOption))
        return parseHelpRequested;

    // Default when not run from command line we assume verbose is desired
    //
    if(1<QCoreApplication::arguments().size())
    {
      qDebug() << "non-empty app args:" << QString::number(QCoreApplication::arguments().size());
      m_args["verbose"] = m_parser.isSet(verboseOption);
      qDebug() << (m_args["verbose"].toBool() ? "verbose option set" : "verbose option not set");
    }

    // Catch the first parser error
    //
    ParseResult result = parseOk;

    // Default mode is "default"
    // - simulate runs the app with GUI interaction but without
    //   connecting to physical hardware.  A default instrument response
    //   (eg., valid hearing test results etc.) is provided
    // - live runs the app with GUI interaction and expects valid input file
    //   and valid path for an output file to be written.  Connection
    //   to a device (hardware and / or software) is required
    // - default is similar to live but an input file and  output file path are
    //   not required.  A dummy barcode ID is used if no input file is present,
    //   and a default output file is written to the working directory in
    //   response to write / save requests.
    //
    if(m_parser.isSet(modeOption))
    {
        QString s = m_parser.value(modeOption).toLower();
        s.replace(0,1,s[0].toUpper());
        Constants::RunMode runMode = Constants::getRunMode(s);
        if(Constants::RunMode::modeUnknown != runMode)
        {
          m_args["runMode"] = QVariant::fromValue(runMode);
          if(m_args["verbose"].toBool())
            qDebug() << "Run mode option set with " << s;
        }
        else
        {
          result = parseRunModeError;
          *errMessage = "Invalid run mode: " + s;
        }
    }
    else
    {
        qDebug() << "run mode option not set";
    }

    bool hasValidInput = false;
    bool hasValidOutput = false;

    if(m_parser.isSet(inputOption) && parseOk == result)
    {
        QString s = m_parser.value(inputOption);
        QFileInfo info(s);
        if(info.exists(s))
        {
            hasValidInput = true;
            m_args["inputFileName"] = s;
            if(m_args["verbose"].toBool())
              qDebug() << "input file option set with " << s;
        }
        else
        {
            qCritical() << "ERROR: input file does not exist: " <<  s;
            result = parseInputFileError;
            *errMessage = "Invalid input file " + s;
        }
    }
    else
    {
        qDebug() << "input file option not set";
    }

    // if command line parsing determined an error do not continue
    // and report on the next potential error
    //
    if(m_parser.isSet(outputOption) && parseOk == result)
    {
        QString s = m_parser.value(outputOption);
        QFileInfo info(s);
        if(info.dir().exists())
        {
            hasValidOutput = true;
            m_args["outputFileName"] = s;
            if(m_args["verbose"].toBool())
              qDebug() << "output file option set with " << s;
        }
        else
        {
            qCritical() << "ERROR: output file path does not exist: " <<  info.dir().canonicalPath();
            result = parseOutputPathError;
            *errMessage = "Invalid output file path " + info.dir().canonicalPath();
        }
    }
    else
    {
        qDebug() << "output file option not set";
    }

    if(m_parser.isSet(typeOption) && parseOk == result)
    {
        QString s = m_parser.value(typeOption);
        qDebug() << "measure type option parsing " << s;
        // determine which manager and dialog is needed based on measure type
        //
        if(s.contains("_"))
        {
            QStringList list;
            foreach(const auto word, s.split("_"))
            {
                list << word.at(0).toUpper() + word.mid(1);
            }
            s = list.join("_");
        }
        else
            s = s.at(0).toUpper() + s.mid(1);

        Constants::MeasureType measureType = Constants::getMeasureType(s);
        if(Constants::MeasureType::typeUnknown != measureType)
        {
            m_args["measureType"] = QVariant::fromValue(measureType);
            if(m_args["verbose"].toBool())
              qDebug() << "measure type option set with " << s;
        }
        else
        {
            qCritical() << "ERROR: input measure type does not exist: " <<  s;
            result = parseMeasureTypeError;
            *errMessage = "Invalid input measure type " + s;
        }
    }
    else
    {
        qDebug() << "measure type option not set";
    }

    if(parseOk == result)
    {
        if(Constants::RunMode::modeLive == m_args["runMode"].value<Constants::RunMode>())
        {
            if(!(hasValidInput && hasValidOutput))
            {
               result = parseError;
               *errMessage = "One or more expected arguments are missng";
            }
        }
        else if(Constants::RunMode::modeHeadless == m_args["runMode"].value<Constants::RunMode>())
        {
            if(!(hasValidInput && hasValidOutput))
            {
               result = parseError;
               *errMessage = "One or more expected arguments are missng";
            }
        }
    }

    qDebug() << "parser result:" << result;
    return result;
}

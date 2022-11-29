#include "CypressApplication.h"

#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QException>
#include <QDebug>

#include "auxiliary/Constants.h"
#include "dialogs/DialogFactory.h"
#include "dialogs/DialogBase.h"
#include "server/Server.h"

QScopedPointer<Server> CypressApplication::restApiServer(new Server());

CypressApplication::CypressApplication(QObject *parent) : QObject(parent)
{
}

CypressApplication::~CypressApplication()
{
}

bool CypressApplication::startTest(Constants::MeasureType type)
{
    qDebug() << "start test";
    QVariantMap args;

    //args["inputFileName"] = "";
    //args["outputFileName"] = "";
    //args["measureType"] = 9;
    //args["runMode"] = "default";
    //args["verbose"] = true;

    try
    {
        setArgs(args);

        DialogFactory* factory = DialogFactory::instance();

        m_dialog.reset(factory->instantiate(type));
        if(m_dialog.isNull()) {
            QMessageBox::warning(nullptr, "Error", "Could not find a supported instrument");
            return false;
        }

        //m_dialog->setInputFileName(m_inputFileName);
        //m_dialog->setOutputFileName(m_outputFileName);
        //m_dialog->setRunMode(m_mode);
        //m_dialog->setVerbose(m_verbose);
        m_dialog->initialize();
        m_dialog->show();
    }
    catch (QException& e)
    {
        qDebug() << e.what();
        return false;
    }

    return true;
}

void CypressApplication::setArgs(const QVariantMap& args)
{
    if(args.contains("inputFileName"))
      m_inputFileName = args["inputFileName"].toString();
    if(args.contains("outputFileName"))
      m_outputFileName = args["outputFileName"].toString();
    if(args.contains("measureType"))
      m_type = args["measureType"].value<Constants::MeasureType>();
    if(args.contains("runMode"))
      m_mode = args["runMode"].value<Constants::RunMode>();
    if(args.contains("verbose"))
      m_verbose = args["verbose"].toBool();

    qDebug() << m_type;
}

void CypressApplication::initialize()
{
    if (m_verbose) {
        qDebug() << "InputFileName: " << m_inputFileName;
        qDebug() << "OutputFileName: " << m_outputFileName;
        qDebug() << "Mode: " << m_mode;
        qDebug() << "Type: " << m_type;
    }

    CypressApplication::restApiServer->start();
    connect(restApiServer.get(), &Server::startTest, this, &CypressApplication::startTest);
}

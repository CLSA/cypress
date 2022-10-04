#include "CypressApplication.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <stdexcept>

#include "./auxiliary/Constants.h"
#include "./dialogs/DialogFactory.h"
#include "./dialogs/DialogBase.h"

CypressApplication::CypressApplication(QObject *parent) : QObject(parent)
{
}

CypressApplication::~CypressApplication()
{
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
}

void CypressApplication::initialize()
{
    DialogFactory *factory = DialogFactory::instance();

    m_dialog.reset(factory->instantiate(m_type));
    if(m_dialog.isNull()) {
        QMessageBox::warning(nullptr, "Error", "Could not find a supported instrument");
    }

    m_dialog->setInputFileName(m_inputFileName);
    m_dialog->setOutputFileName(m_outputFileName);
    m_dialog->setRunMode(m_mode);
    m_dialog->setVerbose(m_verbose);
    m_dialog->initialize();
    m_dialog->show();

    if (!m_verbose)
        return;
    qDebug() << "Input: " << m_inputFileName;
    qDebug() << "Output: " << m_outputFileName;
    qDebug() << "Mode: " << m_mode;
    qDebug() << "Type: " << m_type;
}

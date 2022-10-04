#include "DialogBase.h"

#include "./auxiliary/JsonSettings.h"
#include "./managers/ManagerBase.h"

#include <QCoreApplication>
#include <QCloseEvent>
#include <QDebug>
#include <QDir>
#include <QJsonObject>
#include <QJsonDocument>
#include <QSettings>
#include <QCloseEvent>
#include <QMessageBox>

DialogBase::DialogBase(QWidget *parent)
    : QDialog(parent)
    , m_verbose(false)
{
}

void DialogBase::initialize()
{
  initializeModel();
  initializeConnections();
  readInput();
}

// run should only be called AFTER the user inputs a valid barcode
//
void DialogBase::run()
{
    m_manager->setVerbose(m_verbose);
    m_manager->setRunMode(m_mode);

    // Read the .json file for cached device data
    QDir dir = QCoreApplication::applicationDirPath();
    QSettings settings(dir.filePath(m_manager->getGroup() + ".json"), JsonSettings::JsonFormat);

    m_manager->loadSettings(settings);
    m_manager->setInputData(m_inputData);
    m_manager->start();
}

void DialogBase::closeEvent(QCloseEvent *event)
{
    event->ignore();
    if (QMessageBox::No == QMessageBox::question(this, "Close Confirmation", "Are you sure you want to exit? Any unsaved changes will be lost.", QMessageBox::Yes | QMessageBox::No)) {
        return;
    }

    if(m_verbose)
        qDebug() << "Close event called";

    QDir dir = QCoreApplication::applicationDirPath();
    QSettings settings(dir.filePath(m_manager->getGroup() + ".json"), JsonSettings::JsonFormat);

    m_manager->saveSettings(&settings);
    m_manager->finish();

    event->accept();
}

void DialogBase::readInput()
{
    // TODO: if the run mode is not debug, an input file name is mandatory, throw an error
    if(m_inputFileName.isEmpty())
    {
        if(Constants::RunMode::modeSimulate == m_mode)
        {
          m_inputData["barcode"] = Constants::DefaultBarcode;
        }
        else
        {
          if(m_verbose)
            qDebug() << "ERROR: no input json file";
        }
        return;
    }

    QFileInfo info(m_inputFileName);
    if(info.exists())
    {
      QFile file;
      file.setFileName(m_inputFileName);
      file.open(QIODevice::ReadOnly | QIODevice::Text);
      QString val = file.readAll();
      file.close();

      QJsonDocument jsonDoc = QJsonDocument::fromJson(val.toUtf8());
      m_inputData = jsonDoc.object().toVariantMap();
      if(m_inputData.contains("barcode"))
          this->setVerificationBarcode(m_inputData["barcode"].toString());
    }
    else
    {
      if(m_verbose)
        qDebug() << m_inputFileName << " file does not exist";
    }
}

QString DialogBase::getDefaultOutputFileName() {
    QString barcode = this->getVerificationBarcode();

    QStringList list;
    list
        << QDate().currentDate().toString("yyyyMMdd")
        << barcode
        << m_manager->getGroup()
        << "test.json";

    return list.join("_");
}

QString DialogBase::getOutputFile()
{
    QDir defaultDir = QCoreApplication::applicationDirPath();
    QString outputFileName = m_outputFileName;
    QFileInfo outputFileInfo;

    if (outputFileName.isEmpty() || m_outputFileName.isNull()) {
        const QString fileName = getDefaultOutputFileName();
        outputFileName = defaultDir.filePath(fileName);
    }

    outputFileInfo.setFile(outputFileName);
    if (m_verbose)
        qDebug() << "DialogBase: outputFile: " << outputFileInfo.absoluteFilePath();

    if (outputFileInfo.isDir())
    {
        QString fileName = getDefaultOutputFileName();
        outputFileInfo.setFile(outputFileInfo.absoluteDir().filePath(fileName));
    }

    return outputFileInfo.absoluteFilePath();
}

void DialogBase::writeOutput()
{
   /* Use the output filename if it has a valid path
    * If the path is invalid, use the directory where the application exe resides
    * If the output filename is empty default output .json file is of the form
    * <participant ID>_<now>_<devicename>.json
    */
    QJsonObject jsonObj = m_manager->toJsonObject();
    QString barcode = this->getVerificationBarcode();
    jsonObj.insert("verification_barcode", QJsonValue(barcode));

    QString fileName = getOutputFile();
    QFile saveFile(fileName);

    if(m_verbose) qDebug() << "DialogBase: write";

    QJsonDocument::JsonFormat format = QJsonDocument::JsonFormat::Indented;
    if (m_mode == Constants::RunMode::modeLive) {
        format = QJsonDocument::JsonFormat::Compact;
    }

    saveFile.open(QIODevice::WriteOnly);
    saveFile.write(QJsonDocument(jsonObj).toJson(format));
    saveFile.close();

    if(m_verbose)
        qDebug() << "DialogBase: wrote to file " << fileName;
}

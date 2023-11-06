#include "spirometer_manager.h"
#include "cypress_application.h"

#include "managers/emr/emr_plugin_writer.h"

#include <QFileDialog>
#include <QJsonDocument>
#include <QSettings>
#include <QStandardItemModel>

SpirometerManager::SpirometerManager(QSharedPointer<SpirometerSession> session)
    : ManagerBase(session)
{
    //TODO: check if these upstream inputs are optional or required
    //
    //m_inputKeyList << "asthma"; // true/false
    //m_inputKeyList << "copd";   // true/false
    //m_inputKeyList << "ethnicity"; // caucasian, asian, african, hispanic, other_ethnic

    m_test.reset(new SpirometerTest);
    m_test->setExpectedMeasurementCount(4);
}

bool SpirometerManager::isInstalled()
{
    return false;
}

bool SpirometerManager::isAvailable()
{
    return false;
}



bool SpirometerManager::isDefined(const QString& value, const SpirometerManager::FileType &fileType) const
{
    //if(value.isEmpty())
    //    return false;

    //bool ok = false;
    //if(fileType == SpirometerManager::FileType::EasyWareExe)
    //{
    //    QFileInfo info(value);
    //    if(info.exists() /* && "exe" == info.completeSuffix()*/)
    //    {
    //        ok = true;
    //    }
    //}
    //else if(fileType == SpirometerManager::FileType::EMRDataPath)
    //{
    //    if(QDir(value).exists())
    //    {
    //        ok = true;
    //    }
    //}
    //return ok;
    return false;
}

void SpirometerManager::start()
{
    emit started(m_test.get());
    emit canMeasure();

    //if (Cypress::getInstance().isSimulation()) {
    //    return;
    //}

    //// connect signals and slots to QProcess one time only
    ////
    //connect(&m_process, &QProcess::started,
    //    this, [this]() {
    //        qDebug() << "process started: " << m_process.arguments().join(" ");
    //    });

    //connect(&m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
    //    this, &SpirometerManager::readOutput);

    //connect(&m_process, &QProcess::errorOccurred,
    //    this, [](QProcess::ProcessError error)
    //    {
    //        QStringList s = QVariant::fromValue(error).toString().split(QRegExp("(?=[A-Z])"), Qt::SkipEmptyParts);
    //        qDebug() << "ERROR: process error occured: " << s.join(" ").toLower();
    //    });

    //connect(&m_process, &QProcess::stateChanged,
    //    this, [](QProcess::ProcessState state) {
    //        QStringList s = QVariant::fromValue(state).toString().split(QRegExp("(?=[A-Z])"), Qt::SkipEmptyParts);
    //        qDebug() << "process state: " << s.join(" ").toLower();
    //    });

    //configureProcess();
    //emit dataChanged();
}

void SpirometerManager::measure()
{
    m_test->reset();

    if (Cypress::getInstance().isSimulation())
    {
        m_test->simulate(QVariantMap({
            {"barcode", m_session->getBarcode()},
            {"smoker", m_session->getInputData()["smoker"].toBool()},
            {"gender", m_session->getInputData()["gender"].toString()},
            {"height", m_session->getInputData()["height"].toDouble()},
            {"weight", m_session->getInputData()["weight"].toDouble()},
            {"date_of_birth", m_session->getInputData()["date_of_birth"].toString()},
        }));
    }

    emit measured(m_test.get());
    emit canFinish();

    //qDebug() << "Starting process from measure";
    //clearData();

    //// launch the process
    //m_process.start();
}

void SpirometerManager::finish()
{
    int answer_id = m_session->getAnswerId();

    QJsonObject testJson = m_test->toJsonObject();
    QJsonObject sessionObj = m_session->getJsonObject();
    testJson.insert("session", sessionObj);

    QJsonObject responseJson;
    responseJson.insert("value", testJson);

    QJsonDocument jsonDoc(responseJson);
    QByteArray serializedData = jsonDoc.toJson();

    qDebug() << responseJson;

    QString answerUrl = CypressSettings::getInstance().getAnswerUrl(answer_id);
    sendHTTPSRequest("PATCH", answerUrl, "application/json", serializedData);

    emit success("Measurements have been sent to Pine");

    //if(QProcess::NotRunning != m_process.state())
    //{
    //   m_process.kill();
    //}

    //restoreDatabases();
    //removeXmlFiles();

    ////// delete pdf output file
    //////
    //QString pdfFilePath = getOutputPdfPath();
    //if(QFile::exists(pdfFilePath))
    //{
    //    qDebug() << "remove pdf" << pdfFilePath;
    //    QFile::remove(pdfFilePath);
    //}

    //m_test.reset();

    //QJsonObject results = m_test.toJsonObject();
}

void SpirometerManager::select()
{
    QFileDialog dialog;
    // which do we need to select first ?
    QString caption;
    bool selectingRunnable = false;
    if(!isDefined(m_runnableName, SpirometerManager::FileType::EasyWareExe))
    {
        QStringList filters;
        filters << "Applications (*.exe)" << "Any files (*)";
        caption = tr("Select EasyWarePro.exe File");
        selectingRunnable = true;
        dialog.setNameFilters(filters);
        dialog.setFileMode(QFileDialog::ExistingFile);
    }
    else if(!isDefined(m_dataPath, SpirometerManager::FileType::EMRDataPath))
    {
        dialog.setFileMode(QFileDialog::FileMode::DirectoryOnly);
        caption = tr("Select EMR data transfer directory");
    }
    else
        return;

    dialog.setWindowTitle(caption);
    if(dialog.exec() == QDialog::Accepted)
    {
      QStringList files = dialog.selectedFiles();
      QString fileName = files.first();
      FileType type =
        (selectingRunnable ? FileType::EasyWareExe : FileType::EMRDataPath);
      if(isDefined(fileName,type))
      {
        if(selectingRunnable)
        {
          selectRunnable(fileName);
        }
        else
        {
          selectDataPath(fileName);
        }
      }
   }
}

void SpirometerManager::selectRunnable(const QString& exeName)
{
    if(isDefined(exeName, SpirometerManager::FileType::EasyWareExe))
    {
        QFileInfo info(exeName);
        m_runnableName = exeName;
        m_runnablePath = info.absolutePath();
        emit runnableSelected();
        configureProcess();
    }
    else
        emit canSelectRunnable();
}

void SpirometerManager::selectDataPath(const QString& path)
{
    if(isDefined(path, SpirometerManager::FileType::EMRDataPath))
    {
        m_dataPath = path;
        emit dataPathSelected();
        configureProcess();
    }
    else
        emit canSelectDataPath();
}

void SpirometerManager::readOutput()
{
    //if(QProcess::NormalExit != m_process.exitStatus())
    //{
    //  qDebug() << "ERROR: process failed to finish correctly: cannot read output";
    //  return;
    //}
    //else
    //  qDebug() << "process finished successfully";

    //m_test.fromFile(getEMROutXmlName());
    //if(m_test.isValid())
    //{
    //
    //}
    //else
    //  qDebug() << "ERROR: EMR plugin produced invalid xml test results";

}

bool SpirometerManager::clearData()
{
    m_test.reset();
    return false;
}

void SpirometerManager::backupDatabases() const
{
    // Create database copy
    QString fromFile = getEWPDbName();
    QString toFile = getEWPDbCopyName();
    if(QFile::exists(fromFile))
    {
        qDebug() << "copied" << fromFile << "->" << toFile;
        // Create the copy
        QFile::copy(fromFile, toFile);
    }

    // Create database options copy
    fromFile = getEWPOptionsDbName();
    toFile = getEWPOptionsDbCopyName();
    if(QFile::exists(fromFile))
    {
        qDebug() << "copied" << fromFile << "->" << toFile;
        // Create the copy
        QFile::copy(fromFile, toFile);
    }
}

void SpirometerManager::restoreDatabases() const
{
    // Reset copied database
    QString toFile = getEWPDbName();
    QString fromFile = getEWPDbCopyName();
    if(QFile::exists(fromFile))
    {
        // Remove the current db file if it exists
        if(QFile::exists(toFile))
        {
            QFile::remove(toFile);
        }

        // Rename copy to be the current database
        QFile::rename(fromFile, toFile);
    }

    // Reset copied database options
    toFile = getEWPOptionsDbName();
    fromFile = getEWPOptionsDbCopyName();
    if(QFile::exists(fromFile))
    {
        // Remove the current db file if it exists
        if(QFile::exists(toFile))
        {
            QFile::remove(toFile);
        }

        // Rename copy to be the current database
        QFile::rename(fromFile, toFile);
    }
}

void SpirometerManager::configureProcess()
{
    QDir working(m_runnablePath);
    if(isDefined(m_runnableName, SpirometerManager::FileType::EasyWareExe) &&
       isDefined(m_dataPath, SpirometerManager::FileType::EMRDataPath) &&
       working.exists())
    {
        qDebug() << "OK: configuring command";

        m_process.setProgram(m_runnableName);
        m_process.setWorkingDirectory(m_runnablePath);

        removeXmlFiles();
        backupDatabases();

        // write the inputs to EMR xml
        //
        qDebug() << "creating plugin xml";
        EMRPluginWriter writer;
        //writer.setInputData(m_inputData);
        QDir xmlPath(m_dataPath);
        writer.write(xmlPath.filePath("CypressIn.xml"));

        emit canMeasure();
    }
    else
        qDebug() << "failed to configure process";
}



void SpirometerManager::removeXmlFiles() const
{
    // delete CypressOut.xml if it exists
    //
    QString fileName = getEMROutXmlName();
    if(QFile::exists(fileName))
    {
        qDebug() << "removed" << fileName;
        QFile::remove(fileName);
    }

    // delete CypressIn.xml if it exists
    //
    fileName = getEMRInXmlName();
    if(QFile::exists(fileName))
    {
        qDebug() << "removed" << fileName;
        QFile::remove(fileName);
    }
}

QString SpirometerManager::getOutputPdfPath() const
{
    //if(m_test.isValid() &&
    //   m_test.hasMetaData("pdf_report_path"))
    //{
    //  return m_test.getMetaDataAsString("pdf_report_path");
    //}
    //else
    //  return QString();
    return "";
}

bool SpirometerManager::outputPdfExists() const
{
    bool pdfExists = false;
    QString outPdfPath = getOutputPdfPath();
    if(!outPdfPath.isEmpty())
    {
      pdfExists = QFileInfo::exists(outPdfPath);
    }
    return pdfExists;
}


// Set up device
bool SpirometerManager::setUp() {
    return true;
}

// Clean up the device for next time
bool SpirometerManager::cleanUp() {
    return true;
}

// set input parameters for the test
void SpirometerManager::setInputData(const QVariantMap& inputData)
{
    Q_UNUSED(inputData)
}

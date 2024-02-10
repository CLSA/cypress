#include "spirometer_manager.h"
#include "managers/emr/emr_plugin_writer.h"

#include "cypress_settings.h"
#include "auxiliary/network_utils.h"

#include <QFileDialog>
#include <QJsonDocument>
#include <QMessageBox>
#include <QSettings>
#include <QStandardItemModel>

SpirometerManager::SpirometerManager(QSharedPointer<SpirometerSession> session)
    : ManagerBase(session)
{
    // full path to EasyWarePro.exe
    m_runnableName = CypressSettings::readSetting("spirometer/runnableName").toString();

    // full path to working directory
    m_runnablePath = CypressSettings::readSetting("spirometer/runnablePath").toString(); // path to EasyWarePro.exe directory

    // Path to the EMR plugin data transfer directory
    m_dataPath = CypressSettings::readSetting("spirometer/exchangePath").toString();

    // OnyxIn.xml
    m_inFileName = CypressSettings::readSetting("spirometer/inFileName").toString();

    // OnyxOut.xml
    m_outFileName = CypressSettings::readSetting("spirometer/outFileName").toString();

    if (m_debug) {
        qDebug() << "SpirometerManager";

        qDebug() << session->getSessionId();
        qDebug() << session->getBarcode();
        qDebug() << session->getInterviewer();
        qDebug() << session->getInputData();

        qDebug() << m_runnableName;
        qDebug() << m_runnablePath;
        qDebug() << m_dataPath;
        qDebug() << m_inFileName;
        qDebug() << m_outFileName;
    }

    m_test.reset(new SpirometerTest);
    m_test->setExpectedMeasurementCount(4);
}

bool SpirometerManager::isInstalled()
{
    bool isDebugMode = CypressSettings::isDebugMode();
    bool isSimMode = CypressSettings::isSimMode();

    if (isSimMode)
        return false;

    // path to EasyWarePro.exe
    QString runnableName = CypressSettings::readSetting("spirometer/runnableName").toString();

    // full path to runnable directory
    QString runnablePath = CypressSettings::readSetting("spirometer/runnablePath").toString();

    // Path to the EMR plugin data transfer directory
    QString dataPath = CypressSettings::readSetting("spirometer/exchangePath").toString();

    // OnyxIn.xml
    QString inFileName = CypressSettings::readSetting("spirometer/inFileName").toString();

    // OnyxOut.xml
    QString outFileName = CypressSettings::readSetting("spirometer/outFileName").toString();

    if (runnableName.isEmpty() || runnableName.isNull()) {
        if (isDebugMode)
            qDebug() << "SpirometerManager::runnablePath - runnableName is not defined";

        return false;
    }

    if (runnablePath.isEmpty() || runnablePath.isNull()) {
        if (isDebugMode)
            qDebug() << "SpirometerManager::runnablePath - runnablePath is not defined";

        return false;
    }

    if (dataPath.isEmpty() || dataPath.isNull()) {
        if (isDebugMode)
            qDebug() << "SpirometerManager::isInstalled - dataPath is not defined";

        return false;
    }

    if (inFileName.isEmpty() || inFileName.isNull()) {
        if (isDebugMode)
            qDebug() << "SpirometerManager::isInstalled - inFileName is not defined";

        return false;
    }

    if (outFileName.isEmpty() || outFileName.isNull()) {
        if (isDebugMode)
            qDebug() << "SpirometerManager::isInstalled - outFileName is not defined";

        return false;
    }

    QFileInfo runnableInfo(runnableName);
    if (!runnableInfo.exists()) {
        if (isDebugMode)
            qDebug() << "SpirometerManager::isInstalled - EasyWarePro.exe does not exist at"
                     << runnableName;

        return false;
    }

    if (!runnableInfo.isExecutable()) {
        if (isDebugMode)
            qDebug() << "SpirometerManager::isInstalled - EasyWarePro.exe is not executable at"
                     << runnableName;
        return false;
    }

    QFileInfo runnableDir(runnablePath);
    if (!runnableDir.isDir()) {
        if (isDebugMode)
            qDebug() << "SpirometerManager::isInstalled - Runnable dir is not executable at"
                     << runnablePath;
        return false;
    }

    if (!runnableDir.isReadable()) {
        if (isDebugMode)
            qDebug() << "SpirometerManager::isInstalled - directory does not exist at"
                     << runnableDir;
        return false;
    }

    return true;
}

bool SpirometerManager::start()
{
    if (m_debug)
        qDebug() << "SpirometerManager::start";

    if (m_sim) {
        emit started(m_test);
        emit dataChanged(m_test);
        emit canMeasure();

        return false;
    }

    if (!setUp()) {
        emit error("Something went wrong during setup");
        return false;
    }

    measure();

    return true;
}

void SpirometerManager::measure()
{
    if (m_debug)
        qDebug() << "SpirometerManager::measure";

    if (m_sim) {
        m_test->simulate(QVariantMap({
            {"barcode", m_session->getBarcode()},
            {"smoker", m_session->getInputData()["smoker"].toBool()},
            {"gender", m_session->getInputData()["gender"].toString()},
            {"height", m_session->getInputData()["height"].toDouble()},
            {"weight", m_session->getInputData()["weight"].toDouble()},
            {"date_of_birth", m_session->getInputData()["date_of_birth"].toString()},
        }));

        emit dataChanged(m_test);
        emit canFinish();

        return;
    }

    if (m_process.state() != QProcess::NotRunning) {
        emit error("CardioSoft is already running");
        return;
    }

    clearData();
    m_process.start();

    if (!m_process.waitForStarted()) {
        emit error("Could not launch the spirometer application (EasyOnPC)");
        return;
    }
}

void SpirometerManager::readOutput()
{
    if (m_debug)
        qDebug() << "SpirometerManager::readOutput";

    if (QProcess::NormalExit != m_process.exitStatus()) {
        emit error("Process failed to finish correctly, cannot read output");
        return;
    }

    if (!QFileInfo::exists(getEMROutXmlName())) {
        emit error("Cannot find the output file");
        return;
    }

    QSharedPointer<SpirometerTest> test = qSharedPointerCast<SpirometerTest>(m_test);
    test->fromFile(getEMROutXmlName());

    finish();
}

void SpirometerManager::finish()
{
    QSharedPointer<SpirometerTest> test = qSharedPointerCast<SpirometerTest>(m_test);
    std::unique_ptr<QJsonObject> testJson = test->toJsonObjectHeap();
    testJson->insert("session", m_session->getJsonObject());

    std::unique_ptr<QJsonObject> responseJson = std::make_unique<QJsonObject>();
    responseJson->insert("value", *testJson);

    QJsonDocument jsonDoc(*responseJson);
    std::unique_ptr<QByteArray> serializedData = std::make_unique<QByteArray>(jsonDoc.toJson());

    int answerId = m_session->getAnswerId();
    QString answerUrl = CypressSettings::getAnswerUrl(answerId);
    bool ok = NetworkUtils::sendHTTPSRequest(
        Poco::Net::HTTPRequest::HTTP_PATCH,
        answerUrl.toStdString(),
        "application/json",
        *serializedData
    );

    cleanUp();

    if (ok)
        emit success("Save successful. You may close this window.");
    else
        emit error("Something went wrong");
}

bool SpirometerManager::clearData()
{
    if (m_debug)
        qDebug() << "SpirometerManager::clearData";

    m_test->reset();

    emit dataChanged(m_test);

    return true;
}

void SpirometerManager::backupDatabases() const
{
    if (m_debug)
        qDebug() << "SpirometerManager::backupDatabases";

    QString fromFile = getEWPDbName();
    QString toFile = getEWPDbCopyName();

    if (QFile::exists(fromFile)) {
        if (m_debug)
            qDebug() << "copied" << fromFile << "->" << toFile;

        QFile::copy(fromFile, toFile);
    }

    fromFile = getEWPOptionsDbName();
    toFile = getEWPOptionsDbCopyName();

    if (QFile::exists(fromFile)) {
        if (m_debug)
            qDebug() << "copied" << fromFile << "->" << toFile;

        QFile::copy(fromFile, toFile);
    }
}

void SpirometerManager::restoreDatabases() const
{
    if (m_debug)
        qDebug() << "SpirometerManager::restoreDatabases";

    QString toFile = getEWPDbName();
    QString fromFile = getEWPDbCopyName();

    if (QFile::exists(fromFile)) {
        if(QFile::exists(toFile))
            QFile::remove(toFile);

        QFile::rename(fromFile, toFile);
    }

    toFile = getEWPOptionsDbName();
    fromFile = getEWPOptionsDbCopyName();
    if(QFile::exists(fromFile))
    {
        if(QFile::exists(toFile))
            QFile::remove(toFile);

        QFile::rename(fromFile, toFile);
    }
}

void SpirometerManager::configureProcess()
{
    if (m_debug)
        qDebug() << "SpirometerManager::configureProcess";

    connect(&m_process, &QProcess::started, this, [this]() {
        if (m_debug)
            qDebug() << "SpirometerManager::process started: " << m_process.arguments().join(" ");
    });

    connect(&m_process,
            QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this,
            &SpirometerManager::readOutput);

    connect(&m_process, &QProcess::errorOccurred, this, [=](QProcess::ProcessError error) {
        QStringList s = QVariant::fromValue(error).toString().split(QRegExp("(?=[A-Z])"),
                                                                    Qt::SkipEmptyParts);
        if (m_debug)
            qDebug() << "SpirometerManager::process error occured: " << s.join(" ").toLower();
    });

    connect(&m_process, &QProcess::stateChanged, this, [=](QProcess::ProcessState state) {
        QStringList s = QVariant::fromValue(state).toString().split(QRegExp("(?=[A-Z])"),
                                                                    Qt::SkipEmptyParts);
        if (m_debug)
            qDebug() << "SpiromterManager::process state: " << s.join(" ").toLower();
    });

    if (m_debug)
        qDebug() << "OK: configuring command";

    m_process.setProgram(m_runnableName);
    m_process.setWorkingDirectory(m_runnablePath);

    if (m_debug)
        qDebug() << "SpirometerManager::configureProcess - creating plugin xml";

    // write the inputs to EMR xml
    //
    EMRPluginWriter writer;
    QDir xmlPath(m_dataPath);

    writer.setInputData(m_session->getInputData().toVariantMap());
    writer.write(xmlPath.filePath(m_inFileName));

    emit canMeasure();
}



void SpirometerManager::removeXmlFiles() const
{
    if (m_debug)
        qDebug() << "SpirometerManager::removeXmlFiles";

    // delete OnyxOut.xml if it exists
    //
    QString fileName = getEMROutXmlName();
    if (QFile::exists(fileName))
        QFile::remove(fileName);

    // delete OnyxIn.xml if it exists
    //
    fileName = getEMRInXmlName();
    if (QFile::exists(fileName))
        QFile::remove(fileName);
}

QString SpirometerManager::getOutputPdfPath() const
{
    if (m_debug)
        qDebug() << "SpirometerManager::getOutputPdfPath";

    SpirometerTest* test = static_cast<SpirometerTest*>(m_test.get());
    if (test->isValid() && test->hasMetaData("pdf_report_path"))
        return test->getMetaDataAsString("pdf_report_path");

    return QString();
}

bool SpirometerManager::outputPdfExists() const
{
    if (m_debug)
        qDebug() << "SpirometerManager::outputPdfExists";

    QString outPdfPath = getOutputPdfPath();
    if (outPdfPath.isEmpty() || outPdfPath.isNull())
        return false;

    return QFileInfo::exists(outPdfPath);
}

// Set up device
bool SpirometerManager::setUp()
{
    if (m_debug)
        qDebug() << "SpirometerManager::setUp";

    removeXmlFiles();
    backupDatabases();

    configureProcess();

    return true;
}

// Clean up the device for next time
bool SpirometerManager::cleanUp() {
    if (m_debug)
        qDebug() << "SpirometerManager::cleanUp";

    removeXmlFiles();
    restoreDatabases();

    clearData();

    return true;
}

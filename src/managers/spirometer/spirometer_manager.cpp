#include "spirometer_manager.h"
#include "managers/emr/emr_plugin_writer.h"

#include "cypress_settings.h"
#include "auxiliary/network_utils.h"
#include "auxiliary/file_utils.h"

#include <QFileDialog>
#include <QJsonDocument>
#include <QMessageBox>
#include <QSettings>
#include <QStandardItemModel>

SpirometerManager::SpirometerManager(QSharedPointer<SpirometerSession> session)
    : ManagerBase(session)
{
    // Full path to EasyWarePro.exe
    m_runnableName = CypressSettings::readSetting("spirometer/runnableName").toString();

    // Full path to working directory
    m_runnablePath = CypressSettings::readSetting("spirometer/runnablePath").toString(); // path to EasyWarePro.exe directory

    // Path to the EMR plugin data transfer directory
    m_dataPath = CypressSettings::readSetting("spirometer/exchangePath").toString();

    // EMR Input File (OnyxIn.xml)
    m_inFileName = CypressSettings::readSetting("spirometer/inFileName").toString();

    // EMR Output File OnyxOut.xml
    m_outFileName = CypressSettings::readSetting("spirometer/outFileName").toString();

    qInfo() << "SpirometerManager::SpirometerManager";
    qInfo() << session->getSessionId();
    qInfo() << session->getBarcode();
    qInfo() << session->getInterviewer();
    qInfo() << session->getInputData();

    qInfo() << m_runnableName;
    qInfo() << m_runnablePath;
    qInfo() << m_dataPath;
    qInfo() << m_inFileName;
    qInfo() << m_outFileName;

    m_test.reset(new SpirometerTest);
}

bool SpirometerManager::isInstalled()
{
    qInfo() << "SpirometerManager::isInstalled";

    if (CypressSettings::isSimMode())
        return false;

    // path to EasyWarePro.exe
    const QString runnableName = CypressSettings::readSetting("spirometer/runnableName").toString();

    // full path to runnable directory
    const QString runnablePath = CypressSettings::readSetting("spirometer/runnablePath").toString();

    // Path to the EMR plugin data transfer directory
    const QString dataPath = CypressSettings::readSetting("spirometer/exchangePath").toString();

    // OnyxIn.xml
    const QString inFileName = CypressSettings::readSetting("spirometer/inFileName").toString();

    // OnyxOut.xml
    const QString outFileName = CypressSettings::readSetting("spirometer/outFileName").toString();

    if (runnableName.isEmpty() || runnableName.isNull()) {
        qInfo() << "SpirometerManager::isInstalled: runnableName is not defined";
        return false;
    }

    if (runnablePath.isEmpty() || runnablePath.isNull()) {
        qInfo() << "SpirometerManager::isInstalled: runnablePath is not defined";
        return false;
    }

    if (dataPath.isEmpty() || dataPath.isNull()) {
        qInfo() << "SpirometerManager::isInstalled: dataPath is not defined";
        return false;
    }

    if (inFileName.isEmpty() || inFileName.isNull()) {
        qInfo() << "SpirometerManager::isInstalled: inFileName is not defined";
        return false;
    }

    if (outFileName.isEmpty() || outFileName.isNull()) {
        qInfo() << "SpirometerManager::isInstalled: outFileName is not defined";
        return false;
    }

    const QFileInfo runnableInfo(runnableName);
    if (!runnableInfo.exists()) {
        qInfo() << "SpirometerManager::isInstalled: exe does not exist at" << runnableName;
        return false;
    }
    if (!runnableInfo.isExecutable()) {
        qInfo() << "SpirometerManager::isInstalled: exe is not executable at" << runnableName;
        return false;
    }

    const QFileInfo runnableDir(runnablePath);
    if (!runnableDir.isDir()) {
        qInfo() << "SpirometerManager::isInstalled: directory does not exist at" << runnablePath;
        return false;
    }
    if (!runnableDir.isReadable()) {
        qInfo() << "SpirometerManager::isInstalled: directory is missing read permissions at" << runnableDir;
        return false;
    }

    return true;
}

bool SpirometerManager::start()
{
    qDebug() << "SpirometerManager::start";

    if (!setUp()) {
        emit error("Something went wrong during setup");
        return false;
    }

    measure();

    return true;
}

void SpirometerManager::measure()
{
    qInfo() << "SpirometerManager::measure";

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
    qInfo() << "SpirometerManager::readOutput";

    if (QProcess::NormalExit != m_process.exitStatus()) {
        emit error("Process failed to finish correctly, cannot read output");
        return;
    }

    if (!QFileInfo::exists(getEMROutXmlName())) {
        emit error("Cannot find the output file");
        return;
    }

    auto test = qSharedPointerCast<SpirometerTest>(m_test);
    test->fromFile(getEMROutXmlName());

    finish();
}

void SpirometerManager::finish()
{
    qInfo() << "SpirometerManager::finish";

    auto test = qSharedPointerCast<SpirometerTest>(m_test);

    const int answerId = m_session->getAnswerId();
    const QString pineOrigin = m_session->getOrigin();
    const QString answerUrl = pineOrigin + "/answer/" + QString::number(answerId);

    QJsonObject responseJson {};
    QJsonObject testJson = test->toJsonObject();
    QJsonObject fileJson {};

    const QFileInfo emrOutputXMLFile(getEMROutXmlName());
    if (emrOutputXMLFile.exists()) {
        const QString filePath = emrOutputXMLFile.absoluteFilePath();
        const QString fileSize = FileUtils::getHumanReadableFileSize(filePath);

        fileJson.insert("data_xml", fileSize);

        NetworkUtils::sendHTTPSRequest(
            Poco::Net::HTTPRequest::HTTP_PATCH,
            (answerUrl + "?filename=data.xml").toStdString(),
            "application/octet-stream",
            FileUtils::readFile(filePath)
        );
    }
    else {
        qDebug() << "could not find emrOutputXMLFile" << getEMROutXmlName();
    }

    if (outputPdfExists()) {
        const QString pdfOutputFilePath = getOutputPdfPath();
        const QFileInfo pdfOutputInfo(pdfOutputFilePath);

        qDebug() << "sending pdf output file: " << pdfOutputInfo.absoluteFilePath();

        const QString fileSize = FileUtils::getHumanReadableFileSize(pdfOutputFilePath);

        fileJson.insert("report_pdf", fileSize);

        NetworkUtils::sendHTTPSRequest(
            Poco::Net::HTTPRequest::HTTP_PATCH,
            (answerUrl + QString::number(answerId) + "?filename=report.pdf").toStdString(),
            "application/octet-stream",
            FileUtils::readFile(pdfOutputInfo.absoluteFilePath())
        );
    }
    else {
        qDebug() << "could not find outputPDF: " << getOutputPdfPath();
    }

    testJson.insert("session", m_session->getJsonObject());
    testJson.insert("files", fileJson);
    responseJson.insert("value", testJson);

    const QJsonDocument jsonDoc(responseJson);
    const QByteArray serializedData = jsonDoc.toJson();

    bool ok = NetworkUtils::sendHTTPSRequest(
        Poco::Net::HTTPRequest::HTTP_PATCH,
        answerUrl.toStdString(),
        "application/json",
        serializedData
    );

    cleanUp();

    if (ok) {
        emit success("Save successful. You may close this window.");
    }
    else {
        emit error("Something went wrong");
    }
}

bool SpirometerManager::clearData()
{
    qInfo() << "SpirometerManager::clearData";

    m_test->reset();

    emit dataChanged(m_test);

    return true;
}

void SpirometerManager::backupDatabases() const
{
    qInfo() << "SpirometerManager::backupDatabases";

    QString fromFile = getEWPDbName();
    QString toFile = getEWPDbCopyName();

    if (QFile::exists(fromFile)) {
        qDebug() << "copied" << fromFile << "->" << toFile;
        QFile::copy(fromFile, toFile);
    }

    fromFile = getEWPOptionsDbName();
    toFile = getEWPOptionsDbCopyName();

    if (QFile::exists(fromFile)) {
        qDebug() << "copied" << fromFile << "->" << toFile;
        QFile::copy(fromFile, toFile);
    }
}

void SpirometerManager::restoreDatabases() const
{
    qInfo() << "SpirometerManager::restoreDatabases";

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
    qInfo() << "SpirometerManager::configureProcess";

    connect(&m_process, &QProcess::started, this, [this]() {
        qDebug() << "SpirometerManager::process started: " << m_process.arguments().join(" ");
    });

    connect(&m_process,
            QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this,
            &SpirometerManager::readOutput);

    connect(&m_process, &QProcess::errorOccurred, this, [=](QProcess::ProcessError error) {
        QStringList s = QVariant::fromValue(error).toString().split(QRegExp("(?=[A-Z])"),
                                                                    Qt::SkipEmptyParts);
        qDebug() << "SpirometerManager::process error occured: " << s.join(" ").toLower();
    });

    connect(&m_process, &QProcess::stateChanged, this, [=](QProcess::ProcessState state) {
        QStringList s = QVariant::fromValue(state).toString().split(QRegExp("(?=[A-Z])"),
                                                                    Qt::SkipEmptyParts);
        qDebug() << "SpiromterManager::process state: " << s.join(" ").toLower();
    });

    qDebug() << "OK: configuring command";

    m_process.setProgram(m_runnableName);
    m_process.setWorkingDirectory(m_runnablePath);

    qDebug() << "SpirometerManager::configureProcess - creating plugin xml";

    // write the inputs to EMR xml
    //
    EMRPluginWriter writer;
    const QDir xmlPath(m_dataPath);

    writer.setInputData(m_session->getInputData().toVariantMap());
    writer.write(xmlPath.filePath(m_inFileName));

    emit canMeasure();
}



void SpirometerManager::removeXmlFiles() const
{
    qInfo() << "SpirometerManager::removeXmlFiles";

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
    qInfo() << "SpirometerManager::getOutputPdfPath";

    auto test = qSharedPointerCast<SpirometerTest>(m_test);
    if (test->hasMetaData("pdf_report_path"))
        return test->getMetaDataAsString("pdf_report_path");

    return "";
}

bool SpirometerManager::outputPdfExists() const
{
    qInfo() << "SpirometerManager::outputPdfExists";

    const QString outPdfPath = getOutputPdfPath();
    if (outPdfPath.isEmpty() || outPdfPath.isNull())
        return false;

    return QFileInfo::exists(outPdfPath);
}

// Set up device
bool SpirometerManager::setUp()
{
    qInfo() << "SpirometerManager::setUp";

    removeXmlFiles();
    backupDatabases();

    configureProcess();

    return true;
}

// Clean up the device for next time
bool SpirometerManager::cleanUp() {
    qInfo() << "SpirometerManager::cleanUp";

    removeXmlFiles();
    restoreDatabases();

    clearData();

    return true;
}

#include "cdtt_manager.h"
#include "data/cdtt/tests/cdtt_test.h"
#include "cypress_settings.h"

#include "auxiliary/network_utils.h"
#include "auxiliary/file_utils.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>
#include <QSettings>
#include <QSqlDatabase>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>

CDTTManager::CDTTManager(QSharedPointer<CDTTSession> session)
    : ManagerBase(session)
{
    m_jre = CypressSettings::readSetting("cdtt/jre").toString();
    m_runnableName = CypressSettings::readSetting("cdtt/runnableName").toString();
    m_runnablePath = CypressSettings::readSetting("cdtt/runnablePath").toString();
    m_outputPath = CypressSettings::readSetting("cdtt/outputPath").toString();
    m_settingsFilePath = CypressSettings::readSetting("cdtt/settingsFilePath").toString();

    QDir outputDir(m_outputPath);
    m_outputFile = outputDir.filePath(QString("Results-%0.xlsx").arg(m_session->getBarcode()));

    qDebug() << "CDTTManager";

    qDebug() << m_jre;
    qDebug() << m_runnableName;
    qDebug() << m_runnablePath;
    qDebug() << m_outputPath;

    qDebug() << session->getSessionId();
    qDebug() << session->getBarcode();
    qDebug() << session->getInterviewer();

    qDebug() << session->getInputData();

    m_test.reset(new CDTTTest);
    m_test->setMinimumMeasurementCount(1);
}

CDTTManager::~CDTTManager()
{
    QSqlDatabase::removeDatabase("xlsx_connection");
}

bool CDTTManager::isInstalled()
{
    qDebug() << "CDTTManager::isInstalled";

    const QString jre = CypressSettings::readSetting("cdtt/jre").toString();
    const QString runnableName = CypressSettings::readSetting("cdtt/runnableName").toString();
    const QString runnablePath = CypressSettings::readSetting("cdtt/runnablePath").toString();
    const QString outputPath = CypressSettings::readSetting("cdtt/outputPath").toString();
    const QString settingsFilePath = CypressSettings::readSetting("cdtt/settingsFilePath").toString();

    if (jre.isNull() || jre.isEmpty()) {
        qDebug() << "jre is undefined";
        return false;
    }

    if (runnableName.isNull() || runnableName.isEmpty()) {
        qDebug() << "runnableName is undefined";
        return false;
    }

    if (runnablePath.isNull() || runnablePath.isEmpty()) {
        qDebug() << "runnablePath is undefined";
        return false;
    }

    if (outputPath.isNull() || outputPath.isEmpty()) {
        qDebug() << "outputPath is undefined";
        return false;
    }

    if (settingsFilePath.isNull() || settingsFilePath.isEmpty()) {
        qDebug() << "settingsFilePath is undefined";
        return false;
    }

    const QFileInfo jreInfo(jre);
    if (!jreInfo.exists()) {
        qDebug() << "JRE does not exist at " << jre;
        return false;
    }

    if (!jreInfo.isExecutable()) {
        qDebug() << "JRE is not executable at " << jre;
        return false;
    }

    const QFileInfo runnableNameInfo(runnableName);
    if (!runnableNameInfo.isFile()) {
        qDebug() << "runnableName does not exist";
        return false;
    }

    const QDir runnableDir(runnablePath);
    if (!runnableDir.exists()) {
        qDebug() << "runnablePath does not exist";
        return false;
    }

    const QDir outputDir(outputPath);
    if (!outputDir.exists()) {
        qDebug() << "output dir does not exist";
        return false;
    }

    return true;
}

bool CDTTManager::start()
{
    qDebug() << "CDTT::start";

    if (!setUp())
        return false;

    measure();

    return true;
}

// Set up device
bool CDTTManager::setUp()
{
    qDebug() << "CDTT::setUp";

    if (!clearData())
        return false;

    if (!cleanUp())
        return false;

    configureProcess();

    return true;
}

void CDTTManager::measure()
{
    qDebug() << "CDTT::measure";

    clearData();

    if (m_process.state() != QProcess::NotRunning) {
        emit error("CDTT is already running");
        return;
    }

    m_process.start();

    if (!m_process.waitForStarted()) {
        emit error("Could not start CDTT application");
    }
}

bool CDTTManager::clearData()
{
    qDebug() << "CDTTManager::clearData";

    m_test->reset();

    return true;
}

void CDTTManager::configureProcess()
{
    qDebug() << "CDTTManager::configureProcess";

    QString command = m_jre; // C:/Program Files (x86)/Java/jre1.8.0_51/bin/javaw.exe
    QStringList arguments;

    arguments << "-jar" << m_runnableName << m_session->getBarcode();

    m_process.setProgram(command);
    m_process.setArguments(arguments);
    m_process.setWorkingDirectory(m_runnablePath);
    m_process.setProcessChannelMode(QProcess::ForwardedChannels);

    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    qDebug() << QDir::toNativeSeparators(m_settingsFilePath);

    db.setDatabaseName(
        "DRIVER={Microsoft Excel Driver (*.xls, *.xlsx, *.xlsm, *.xlsb)};DBQ=" + QDir::toNativeSeparators(m_settingsFilePath));

    if (!db.isValid()) {
        emit error("ERROR: invalid database");
        return;
    }

    if (!db.open()) {
        qDebug() << db.isOpenError();
        qDebug() << db.lastError();
        emit error("Cannot find the settings file");
        return;
    }

    QSqlQuery query; //
    query.prepare("UPDATE [DefaultParameters$] SET B2 = 'EN_CA'");
    if (!query.exec()) {
        qDebug() << query.lastQuery();
        qDebug() << query.lastError();
        emit error("Cannot set language settings");
    }

    qDebug() << "CDTTManager::configureProcess - config args: "
         << m_process.arguments().join(" ");

    qDebug() << "CDTTManager::configureProces - working dir: "
         << m_runnablePath;

    // connect signals and slots to QProcess one time only
    //
    connect(&m_process, &QProcess::started, this,
        [this] () {
            qDebug() << "process started: " << m_process.arguments().join(" ");
        }
    );

    connect(
        &m_process,
        QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        this,
        &CDTTManager::readOutput
    );

    connect(&m_process, &QProcess::errorOccurred, this,
        [](QProcess::ProcessError error) {
            QStringList s = QVariant::fromValue(error).toString().split(QRegExp("(?=[A-Z])"), Qt::SkipEmptyParts);
            qDebug() << "ERROR: process error occured: " << s.join(" ").toLower();
        });

    connect(&m_process, &QProcess::stateChanged, this,
        [](QProcess::ProcessState state) {
            QStringList s = QVariant::fromValue(state).toString().split(QRegExp("(?=[A-Z])"), Qt::SkipEmptyParts);
            qDebug() << "process state: " << s.join(" ").toLower();
    });
}

void CDTTManager::readOutput()
{
    qDebug() << "CDTTManager::readOutput";

    if(QProcess::NormalExit != m_process.exitStatus()) {
        emit error("CDTT failed to finish correctly, cannot read data.");
        return;
    }

    QDir dir(m_outputPath);
    QString fileName = dir.filePath(QString("Results-%0.xlsx").arg(m_session->getBarcode()));

    fileName = dir.toNativeSeparators(fileName);
    if (!QFileInfo::exists(fileName)) {
        emit error("Error: cannot find the output .xlsx file");
    }

    qDebug() << "found output xlsx file " << fileName;

    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName(
        "DRIVER={Microsoft Excel Driver (*.xls, *.xlsx, *.xlsm, *.xlsb)};DBQ=" + fileName);

    if (!db.isValid()) {
        emit error("ERROR: invalid database");
        return;
    }


    if (!db.open()) {
        qDebug() << "cannot find valid file";
        emit error("Error: cannot find valid file");
        return;
    }

    auto test = qSharedPointerCast<CDTTTest>(m_test);
    test->fromDatabase(db);
    db.close();
    finish();
}


void CDTTManager::finish() {
    qDebug() << "CDTTManager::finish";

    const int answer_id = m_session->getAnswerId();
    const QString pineOrigin = m_session->getOrigin();
    const QString answerUrl = pineOrigin + "/answer/" + QString::number(answer_id);

    qDebug() << pineOrigin;

    //const QString host = CypressSettings::getPineHost();
    //const QString endpoint = CypressSettings::getPineEndpoint();

    QDir dir(m_outputPath);
    if (!dir.exists()) {
        qDebug() << "directory does not exist: " << m_outputPath;
        emit error("Output directory does not exist. Could not save measurements.");
    }

    QString outputFilePath = dir.absoluteFilePath(QString("Results-%0.xlsx").arg(m_session->getBarcode()));
    QFileInfo excelFile(outputFilePath);
    if (!excelFile.exists()) {
        qDebug() << "file does not exist: " << excelFile.absoluteFilePath();
        emit error("Output excel file does not exist. Could not save measurements.");
        return;
    }

    QJsonObject filesJson {};
    filesJson.insert("cdtt_xlsx", FileUtils::getHumanReadableFileSize(excelFile.absoluteFilePath()));

    QJsonObject testJson = m_test->toJsonObject();
    QJsonObject sessionJson = m_session->getJsonObject();
    testJson.insert("session", sessionJson);
    testJson.insert("files", filesJson);

    QJsonObject responseJson {};
    responseJson.insert("value", testJson);

    QJsonDocument jsonDoc(responseJson);
    QByteArray serializedData = jsonDoc.toJson();


    bool ok = NetworkUtils::sendHTTPSRequest(
        Poco::Net::HTTPRequest::HTTP_PATCH,
        (answerUrl + "?filename=cdtt.xlsx").toStdString(),
        "application/octet-stream",
        FileUtils::readFile(excelFile.absoluteFilePath())
    );

    if (!ok) {
        emit error("Could not transfer ECG file to Pine.");
        return;
    }

    //QString answerUrl = CypressSettings::getAnswerUrl(answer_id);

    ok = NetworkUtils::sendHTTPSRequest(
        Poco::Net::HTTPRequest::HTTP_PATCH,
        answerUrl.toStdString(),
        "application/json",
        serializedData
    );

    cleanUp();

    if (ok)
        emit success("Save successful. You may close this window.");
    else
        emit error("Something went wrong");
}



// Clean up the device for next time
bool CDTTManager::cleanUp()
{
    qDebug() << "CDTTManager::cleanUp";

    m_test->reset();

    if(QProcess::NotRunning != m_process.state())
        m_process.close();

    if(!m_outputFile.isEmpty() && QFileInfo::exists(m_outputFile))
    {
        QFile ofile(m_outputFile);
        ofile.remove();
        m_outputFile.clear();
    }
    return true;
}

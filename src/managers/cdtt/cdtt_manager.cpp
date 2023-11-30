#include "cypress_application.h"

#include "cdtt_manager.h"

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

CDTTManager::CDTTManager(QSharedPointer<CDTTSession> session)
    : ManagerBase(session)
{
    m_test.reset(new CDTTTest);
    m_test->setMinimumMeasurementCount(1);
}

CDTTManager::~CDTTManager()
{
    QSqlDatabase::removeDatabase("xlsx_connection");
}

bool CDTTManager::isInstalled()
{
    return false;
}

void CDTTManager::start()
{
    if (m_debug)
    {
        qDebug() << "CDTT::start";
    }

    setUp();

    emit canMeasure();
    emit dataChanged(m_test.get());
}

// Set up device
bool CDTTManager::setUp()
{
    if (m_debug)
    {
        qDebug() << "CDTT::setUp";
    }

    m_test->reset();
    configureProcess();
    return true;
}

void CDTTManager::measure()
{
    if (m_debug)
    {
        qDebug() << "CDTT::measure";
    }

    m_test->reset();
    if (CypressSettings::isSimMode())
    {
        m_test->simulate({});

        emit measured(m_test.get());
        emit canFinish();

        return;
    }

    m_process.start();

    emit dataChanged(m_test.get());
}

bool CDTTManager::clearData()
{
    if (m_debug)
    {
        qDebug() << "CDTTManager::clearData";
    }

    m_test->reset();
    return true;
}

void CDTTManager::finish()
{
    if (m_debug)
    {
        qDebug() << "CDTTManager::finish";
    }

    QJsonObject responseJson {};

    int answer_id = m_session->getAnswerId();

    QJsonObject testJson = m_test->toJsonObject();
    testJson.insert("session", m_session->getJsonObject());

    responseJson.insert("value", testJson);

    QJsonDocument jsonDoc(responseJson);
    QByteArray serializedData = jsonDoc.toJson();

    QString answerUrl = CypressSettings::getAnswerUrl(answer_id);
    sendHTTPSRequest("PATCH", answerUrl, "application/json", serializedData);

    emit success("Measurements saved to Pine");

    cleanUp();
}

void CDTTManager::configureProcess()
{
    if (m_debug)
    {
        qDebug() << "CDTTManager::configureProcess";
    }

    QDir workingDir("");
    QDir outputDir("");

    QString runnablePath = "";

    QString command = "java";
    QStringList arguments;
    arguments << "-jar" << runnablePath << m_session->getBarcode();

    m_process.setProgram(command);
    m_process.setArguments(arguments);
    m_process.setWorkingDirectory(workingDir.absolutePath());
    m_process.setProcessChannelMode(QProcess::ForwardedChannels);

    qDebug() << "process config args: " << m_process.arguments().join(" ");
    qDebug() << "process working dir: " << workingDir.absolutePath();

    // connect signals and slots to QProcess one time only
    //
    connect(&m_process, &QProcess::started,
        this, [this]() {
            qDebug() << "process started: " << m_process.arguments().join(" ");
        });

    connect(&m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        this, &CDTTManager::readOutput);

    connect(&m_process, &QProcess::errorOccurred,
        this, [](QProcess::ProcessError error)
        {
            QStringList s = QVariant::fromValue(error).toString().split(QRegExp("(?=[A-Z])"), Qt::SkipEmptyParts);
            qDebug() << "ERROR: process error occured: " << s.join(" ").toLower();
        });

    connect(&m_process, &QProcess::stateChanged,
        this, [](QProcess::ProcessState state) {
            QStringList s = QVariant::fromValue(state).toString().split(QRegExp("(?=[A-Z])"), Qt::SkipEmptyParts);
            qDebug() << "process state: " << s.join(" ").toLower();
        });
}

void CDTTManager::readOutput()
{
    if (m_debug)
    {
        qDebug() << "CDTTManager::readOutput";
    }

    if(QProcess::NormalExit != m_process.exitStatus())
    {
        QMessageBox::critical(nullptr, "Error", "CDTT failed to finish correctly, cannot read data. Please contact support");
        return;
    }
    else
    {
        qDebug() << "process finished successfully";
    }

    CDTTTest* test = static_cast<CDTTTest*>(m_test.get());

    QDir dir(m_outputPath);
    QString fileName = dir.filePath(QString("Results-%0.xlsx").arg(m_session->getBarcode()));
    if(QFileInfo::exists(fileName))
    {
        qDebug() << "found output xlsx file " << fileName;

        QSqlDatabase db;
        if(!QSqlDatabase::contains("xlsx_connection"))
        {
            db = QSqlDatabase::addDatabase("QODBC", "xlsx_connection");
            db.setDatabaseName(
                "DRIVER={Microsoft Excel Driver (*.xls, *.xlsx, *.xlsm, *.xlsb)};DBQ=" + fileName);
            if(db.isValid())
                db.open();
            else
                qDebug() << "ERROR: invalid database using" << fileName;
        }
        else
            db = QSqlDatabase::database("xlsx_connection");

        if(db.isValid() && !db.isOpen())
            db.open();

        if(db.isOpen())
        {
            test->fromDatabase(db);
            if(test->isValid())
            {
                emit measured(m_test.get());
                emit canFinish();
            }
            else
            {
                qDebug() << "";
            }

            db.close();
        }
    }
    else
    {
        qDebug() << "ERROR: no output xlsx file found" << fileName;
    }
}



// Clean up the device for next time
bool CDTTManager::cleanUp()
{
    if (m_debug)
    {
        qDebug() << "CDTTManager::cleanUp";
    }

    m_test.reset();

    if(QProcess::NotRunning != m_process.state())
    {
        m_process.kill();
    }

    if(!m_outputFile.isEmpty() && QFileInfo::exists(m_outputFile))
    {
        QFile ofile(m_outputFile);
        ofile.remove();
        m_outputFile.clear();
    }
    return true;
}

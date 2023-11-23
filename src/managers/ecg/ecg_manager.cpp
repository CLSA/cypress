#include "ecg_manager.h"
#include "cypress_application.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QFileDialog>
#include <QJsonObject>
#include <QSettings>
#include <QStandardItemModel>
#include <QJsonDocument>
#include <QMessageBox>

ECGManager::ECGManager(QSharedPointer<ECGSession> session)
    : ManagerBase(session)
{
    m_runnableName = CypressSettings::readSetting("ecg/runnableName").toString();
    m_workingPath = CypressSettings::readSetting("ecg/workingPath").toString();
    m_exportPath = CypressSettings::readSetting("ecg/exportPath").toString();
    m_outputFile = m_exportPath + "/" + session->getBarcode() + ".xml";

    m_test.reset(new ECGTest);
    m_test->setExpectedMeasurementCount(1);
}

bool ECGManager::isInstalled()
{
    return false;
}

bool ECGManager::isDefined(const QString &fileName, const FileType &type) const
{
    if (m_debug)
    {
        qDebug() << "ECGManager::isDefined";
    }

    if(m_sim)
    {
        return true;
    }

    bool ok = false;
    if(type == FileType::ECGApplication)
    {
        QFileInfo info(fileName);
        ok = info.exists() && info.isExecutable();
    }
    else
    {
        QDir info(fileName);
        ok = info.exists() && info.isReadable() && info.exists("Export");
    }
    return ok;
}

void ECGManager::start()
{
    if (m_debug)
    {
        qDebug() << "ECGManager::start";
    }

    if (m_sim)
    {
        emit started(m_test.get());
        emit canMeasure();

        return;
    }

    // connect signals and slots to QProcess one time only
    //
    connect(&m_process, &QProcess::started,
        this, [this]() {
            qDebug() << "process started: " << m_process.arguments().join(" ");
        });

    connect(&m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        this, &ECGManager::readOutput);

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

    m_process.setProcessChannelMode(QProcess::ForwardedChannels);

    configureProcess();
    //emit dataChanged();
}

void ECGManager::configureProcess()
{
    if (m_debug)
    {
        qDebug() << "ECGManager::configureProcess";
    }

    QDir workingDir(m_workingPath);
    QDir exportDir(m_exportPath);

    if(isDefined(m_runnableName) && workingDir.exists() && exportDir.exists())
    {
        m_process.setProgram(m_runnableName);

        QString path = QDir::cleanPath(QString("%1%2%3").arg(m_workingPath, QDir::separator(), INIT_PATH));

        QDir backupDir(path);

        if(backupDir.exists())
        {
            if(!backupDir.removeRecursively())
            {
                qDebug() << "failed to configure and remove backup directory" << path;
            }
        }

        if(deleteDeviceData())
        {
            emit canMeasure();
        }
        else
        {
            qDebug() << "couldn't delete device data";
        }
    }
    else
    {
        qDebug() << "failed to configure process";
    }
}

void ECGManager::measure()
{
    if (m_debug)
    {
        qDebug() << "ECGManager::measure";
    }

    clearData();

    if (CypressSettings::isSimMode())
    {
        m_test->simulate();

        emit measured(m_test.get());
        emit canFinish();

        return;
    }

    qDebug() << "starting process";
    m_process.start();
}

void ECGManager::readOutput()
{
    if (m_debug)
    {
        qDebug() << "ECGManager::readOutput";
    }

    ECGTest* test = static_cast<ECGTest*>(m_test.get());
    if(QProcess::NormalExit != m_process.exitStatus())
    {
        QMessageBox::critical(nullptr, "Error", "CardioSoft error, cannot read output");

        return;
    }

    if(QFileInfo::exists(m_outputFile))
    {
        qDebug() << "found xml output file " << m_outputFile;
        test->fromFile(m_outputFile);
        if(m_test->isValid())
        {
            emit measured(test);
            emit canFinish();
        }
        else
        {
            QMessageBox::critical(nullptr, "Error", "The measurements from CardioSoft were not valid");
        }
    }
    else
    {
        QMessageBox::critical(nullptr, "Error", "Cannot find the measurements from CardioSoft");
    }
}



bool ECGManager::clearData()
{
    if (m_debug)
    {
        qDebug() << "ECGManager::clearData";
    }

    m_test.reset();
    return true;
}

void ECGManager::finish()
{
    if (m_debug)
    {
        qDebug() << "ECGManager::finish";
    }

    int answer_id = m_session->getAnswerId();

    QJsonObject testJson = m_test->toJsonObject();
    QJsonObject sessionObj = m_session->getJsonObject();
    QJsonObject value = testJson.value("value").toObject();

    value.insert("session", sessionObj);
    testJson.insert("value", value);

    QJsonDocument jsonDoc(testJson);
    QByteArray serializedData = jsonDoc.toJson();

    QString host = CypressSettings::getPineHost();
    QString endpoint = CypressSettings::getPineEndpoint();

    QString answerUrl = CypressSettings::getAnswerUrl(answer_id);
    sendHTTPSRequest("PATCH", answerUrl, "application/json", serializedData);

    m_test->reset();

    emit success("Measurements saved to Pine");

    if(QProcess::NotRunning != m_process.state())
    {
        m_process.kill();
    }

    deleteDeviceData();
}

bool ECGManager::deleteDeviceData()
{
    if (m_debug)
    {
        qDebug() << "ECGManager::deleteDeviceData";
    }

    QString path = QDir::cleanPath(QString("%1%2%3").arg(m_workingPath,QDir::separator(),INIT_PATH));
    QDir backupDir(path);

    if(!backupDir.exists())
    {
        if(!backupDir.mkdir(path))
        {
            qCritical() << "unable to create backup directory" << path;
            return false;
        }
    }

    // list of backed up database files
    //
    backupDir.setNameFilters(QStringList() << "*.BTR");
    backupDir.setFilter(QDir::Files);

    QFileInfoList list = backupDir.entryInfoList();
    if(!list.isEmpty())
    {
        path = QDir::cleanPath(QString("%1%2%3").arg(m_workingPath, QDir::separator(), DATABASE_PATH));
        foreach(const auto info, list)
        {
            QString toFile = QDir(path).filePath(info.fileName());
            QFile::copy(info.absoluteFilePath(), toFile);
        }
    }

    // xml output from Cardiosoft export
    //
    QFile file(m_outputFile);
    if(file.exists() && !file.remove())
    {
        qDebug() << "could not delete Cardiosoft xml output file" << m_outputFile;
        return false;
    }

    return true;
}

// Set up device
bool ECGManager::setUp()
{
    if (m_debug)
    {
        qDebug() << "ECGManager::setUp";
    }

    return true;
}

// Clean up the device for next time
bool ECGManager::cleanUp()
{
    if (m_debug)
    {
        qDebug() << "ECGManager::cleanUp";
    }
    return true;
}

#include "cypress_application.h"
#include "ecg_manager.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QFileDialog>
#include <QJsonObject>
#include <QSettings>
#include <QStandardItemModel>
#include <QJsonDocument>

ECGManager::ECGManager(const CypressSession& session)
    : ManagerBase(session)
{
    m_test.reset(new ECGTest);
    m_test->setExpectedMeasurementCount(1);
}

bool ECGManager::isAvailable()
{
    return false;
}

bool ECGManager::isInstalled()
{
    return false;
}

void ECGManager::start()
{
    emit started(m_test.get());
    emit canMeasure();

    // connect signals and slots to QProcess one time only
    //
    //connect(&m_process, &QProcess::started,
    //    this, [this]() {
    //        qDebug() << "process started: " << m_process.arguments().join(" ");
    //    });

    //connect(&m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
    //    this, &ECGManager::readOutput);

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

    //m_process.setProcessChannelMode(QProcess::ForwardedChannels);

    //configureProcess();
    //emit dataChanged();
}

void ECGManager::measure()
{
    m_test->reset();
    m_test->simulate();

    emit measured(m_test.get());

    if (m_test->isValid())
    {
        emit canFinish();
    }
}

void ECGManager::readOutput()
{
}

void ECGManager::configureProcess()
{
}

bool ECGManager::clearData()
{
    m_test.reset();
    return true;
}

void ECGManager::finish()
{
    int answer_id = m_session.getAnswerId();

    QJsonObject testJson = m_test->toJsonObject();
    QJsonObject sessionObj = m_session.getJsonObject();
    QJsonObject value = testJson.value("value").toObject();

    value.insert("session", sessionObj);

    testJson.insert("value", value);

    QJsonDocument jsonDoc(testJson);
    QByteArray serializedData = jsonDoc.toJson();

    QString host = CypressSettings::getInstance().getPineHost();
    QString endpoint = CypressSettings::getInstance().getPineEndpoint();

    sendHTTPSRequest("PATCH",
                     host + endpoint + QString::number(answer_id),
                     "application/json",
                     serializedData);

    emit success("");
}

bool ECGManager::deleteDeviceData()
{
   return false;
}

// Set up device
bool ECGManager::setUp()
{
    return true;
}

// Clean up the device for next time
bool ECGManager::cleanUp()
{
    return true;
}

void ECGManager::setInputData(const QVariantMap& inputData)
{
    Q_UNUSED(inputData)
}


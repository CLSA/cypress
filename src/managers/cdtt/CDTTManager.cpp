#include "CDTTManager.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QJsonObject>
#include <QProcess>
#include <QSettings>
#include <QSqlDatabase>
#include <QStandardItemModel>

CDTTManager::CDTTManager(QWidget* parent) : ManagerBase(parent)
{
    m_test.setMinimumMeasurementCount(1);
}

CDTTManager::~CDTTManager()
{
  QSqlDatabase::removeDatabase("xlsx_connection");
}

void CDTTManager::start()
{
    // connect signals and slots to QProcess one time only
    //
    connect(&m_process, &QProcess::started,
        this, [this]() {
            qDebug() << "process started: " << m_process.arguments().join(" ");
        });

    //connect(&m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
    //    this, &CDTTManager::readOutput);

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
    emit dataChanged();
}

bool CDTTManager::isDefined(const QString& runnableName) const
{
    return false;
}

void CDTTManager::selectRunnable(const QString &runnableName)
{

}

void CDTTManager::measure()
{
    clearData();
    // launch the process
    qDebug() << "starting process from measure";
    m_process.start();
}

void CDTTManager::clearData()
{
    m_test.reset();
}

void CDTTManager::finish()
{
    QJsonObject results = m_test.toJsonObject();
}

void CDTTManager::configureProcess()
{

}

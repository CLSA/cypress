#include "ora_manager.h"

#include <QSqlQuery>
#include <QSqlError>

DeviceConfig ORAManager::config {{
    {"runnableName",       {"ora/runnableName",       Exe  }},
    {"runnablePath",       {"ora/runnablePath",       Dir  }},
    {"databasePath",       {"ora/databasePath",       File }},
    {"backupDatabasePath", {"ora/backupDatabasePath", File }},
}};

ORAManager::ORAManager(QSharedPointer<ORASession> session): ManagerBase { session }
{
    m_runnableName = config.getSetting("runnableName");
    m_runnablePath = config.getSetting("runnablePath");
    m_databasePath = config.getSetting("databasePath");
    m_backupDatabasePath = config.getSetting("backupDatabasePath");
}

bool ORAManager::start()
{
    qInfo() << "TonometerManager::configureProcess";

    m_process.setProgram(m_runnableName);
    m_process.setWorkingDirectory(m_runnablePath);
    m_process.setProcessChannelMode(QProcess::ForwardedChannels);

    // connect signals and slots to QProcess one time only
    //
    connect(&m_process, &QProcess::started,
        this, [this]() {
            qDebug() << "process started: " << m_process.arguments().join(" ");
        });

    connect(&m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        this, &ORAManager::readOutput);

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

    return true;
}

void ORAManager::readOutput()
{
    //QSqlQuery query;

    //query.prepare("SELECT * FROM ");
    //if (!query.exec()) {
    //    qCritical() << query.lastError();
    //    return;
    //}
    //if (!query.size()) {
    //    return;
    //}

    //query.prepare("SELECT * FROM ");
    //if (!query.exec()) {
    //    qCritical() << query.lastError();
    //    return;
    //}
    //if (!query.size()) {
    //    return;
    //}

    //query.prepare("SELECT * FROM ");
    //if (!query.exec()) {
    //    qCritical() << query.lastError();
    //    return;
    //}

    //if (!query.size()) {
    //    return;
    //}
}

void ORAManager::finish()
{
    // send data
}

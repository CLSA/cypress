#include "ora_manager.h"
#include "data/tonometer/tests/ora_test.h"
#include "auxiliary/file_utils.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

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

    //m_database = QSqlDatabase::addDatabase("QODBC");
    //m_database.setDatabaseName("Driver={Microsoft Access Driver (*.mdb)};DBQ=" + QDir::toNativeSeparators(m_databasePath));

    m_test.reset(new ORATest);
}

bool ORAManager::start()
{
    qInfo() << "TonometerManager::configureProcess";

    // restore database
    if (!FileUtils::deleteFile(m_databasePath)) {
        qCritical() << "ORAManager::start - could not delete " << m_databasePath;
        return false;
    }

    if (!FileUtils::copyFile(m_backupDatabasePath, m_databasePath)) {
        qCritical() << "ORAManager::start - could not copy " << m_backupDatabasePath;
        return false;
    }

    // connect to database
    m_database = QSqlDatabase::addDatabase("QODBC");
    m_database.setDatabaseName("Driver={Microsoft Access Driver (*.mdb)};DBQ=" + QDir::toNativeSeparators(m_databasePath));


    if (!m_database.open()) {
        qCritical() << "ORAManager::start - could not open database";
        return false;
    }

    QSqlQuery query(m_database);

    query.prepare("INSERT INTO Patients ( Name, BirthDate, Sex, GroupID, ID, RaceID ) VALUES ( :name, :birthDate, :sex, :groupId, :id, :raceId )");
    query.bindValue(":name", m_session->getBarcode() + ",CLSA");
    query.bindValue(":birthDate", m_session->getInputData()["dob"].toString());
    query.bindValue(":sex", m_session->getInputData()["sex"].toString().startsWith("m", Qt::CaseSensitivity::CaseInsensitive));
    query.bindValue(":groupId", 2);
    query.bindValue(":id", m_session->getBarcode().toInt());
    query.bindValue(":raceId", 1);

    if (!query.exec()) {
        qCritical() << "Database error:" << m_database.lastError().text();
        return false;
    }

    m_database.close();

    // configure process
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

    // start process
    m_process.start();

    return true;
}

void ORAManager::readOutput()
{
    qInfo() << "TonometerManager::readOutput";

    if (!m_database.open()) {
        emit error("Could not open database");
        return;
    }

    const QVariantMap leftResults = extractMeasures("L");
    const QVariantMap rightResults = extractMeasures("R");

    const QList<QVariantMap> results { leftResults, rightResults };

    auto test = qSharedPointerCast<ORATest>(m_test);
    test->fromVariantMapList(results);

    m_database.close();

    qDebug() << "ORAManager::readOutput" << results;
    finish();
}

QVariantMap ORAManager::extractMeasures(const QString& eye)
{
    qInfo() << "TonometerManager::extractMeasures";

    if (!m_database.isOpen()) {
        if (!m_database.open()) {
            throw QException();
        }
    }

    QSqlQuery query(m_database);

    query.prepare("SELECT PatientID from Patients where ID = :id");
    query.bindValue(":id", m_session->getBarcode().toInt());

    if (!query.exec()) {
        qWarning() << "Database error:" << m_database.lastError().text();
    }

    query.prepare("SELECT * from Measures where Eye = :eye ORDER BY MeasureDate desc");
    query.bindValue(":eye", QString(eye));

    QVariantMap resultMap;
    if (!query.exec()) {
        qWarning() << "Database error:" << m_database.lastError().text();
        return resultMap;
    }

    while (query.next()) {
        for (int i = 0; i < query.record().count(); ++i) {
            resultMap.insert(query.record().fieldName(i), query.value(i));
        }
    }

    return resultMap;
}

void ORAManager::finish()
{
    ManagerBase::finish();
}

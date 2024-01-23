#include "cypress_settings.h"

#include "tonometer_manager.h"
#include "server/sessions/tonometer_session.h"

#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardItemModel>

TonometerManager::TonometerManager(QSharedPointer<TonometerSession> session)
    : ManagerBase(session)
{
    m_test.reset(new TonometerTest);
    m_test->setExpectedMeasurementCount(2);

    m_db = QSqlDatabase::addDatabase("QODBC");
    QString mdbFilePath = "C:\\Users\\antho\\Documents\\Database11.mdb";
    m_db.setDatabaseName("Driver={Microsoft Access Driver (*.mdb)};DBQ=" + mdbFilePath);

    if (!m_db.open()) {
        qWarning() << "Error: Unable to connect to database.";
        qWarning() << "Database error:" << m_db.lastError().text();
    }
}

TonometerManager::~TonometerManager()
{
    QSqlDatabase::removeDatabase("mdb_connection");
}

bool TonometerManager::start()
{
    if (m_debug)
        qDebug() << "TonometerManager::start";

    if (!setUp()) {
        emit error("Something went wrong. Please contact support");
    }

    measure();

    return true;
}


bool TonometerManager::isDefined(const QString& fileName, const TonometerManager::FileType& type) const
{
    if (m_debug)
        qDebug() << "TonometerManager::isDefined";

    bool ok = false;
    QFileInfo info(fileName);
    if(type == TonometerManager::FileType::ORAApplication)
      ok = info.isExecutable() && info.exists();
    else
      ok = info.isFile() && info.exists();
    return ok;
}

bool TonometerManager::isInstalled()
{
    return false;
}

void TonometerManager::measure()
{
    if (m_debug)
        qDebug() << "TonometerManager::measure";

    clearData();

    if (m_sim)
    {
        m_test->simulate({});
        return;
    }

    if (m_process.state() != QProcess::NotRunning) {
        emit error("Tonometer is already running");
        return;
    }

    m_process.start();

    if (!m_process.waitForStarted()) {
        emit error("Could not start Tonometer application");
        return;
    }
}

void TonometerManager::readOutput()
{
    if (m_debug)
        qDebug() << "TonometerManager::readOutput";

    QVariantMap leftResults = extractMeasures(m_session->getBarcode().toInt(), "Left");
    QVariantMap rightResults = extractMeasures(m_session->getBarcode().toInt(), "Right");

    QList<QVariantMap> results { leftResults, rightResults };

    QSharedPointer<TonometerTest> test = qSharedPointerCast<TonometerTest>(m_test);
    test->fromVariantMapList(results);

    finish();
}

void TonometerManager::configureProcess()
{
    if (m_debug)
        qDebug() << "TonometerManager::configureProcess";

    // connect signals and slots to QProcess one time only
    //
    connect(&m_process, &QProcess::started,
        this, [this]() {
            qDebug() << "process started: " << m_process.arguments().join(" ");
        });

    connect(&m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        this, &TonometerManager::readOutput);

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



bool TonometerManager::clearData()
{
    if (m_debug)
        qDebug() << "TonometerManager::clearData";

    m_test->reset();
    restoreDatabase();

    return false;
}

bool TonometerManager::setUp()
{
    if (m_debug)
        qDebug() << "TonometerManager::setUp";

    bool ok = restoreDatabase();
    if (!ok) {
        return false;
    }

    ok = insertPatient(
        m_session->getBarcode(),
        m_session->getInputData()["date_of_birth"].toString(),
        m_session->getInputData()["sex"].toString(),
        m_session->getBarcode().toInt()
    );
    if (!ok) {
        return false;
    }

    return true;
}

bool TonometerManager::cleanUp()
{
    if (m_debug)
        qDebug() << "TonometerManager::cleanUp";

    bool ok = restoreDatabase();
    return ok;
}

bool TonometerManager::insertPatient(
                                     const QString &name,
                                     const QString &birthDate,
                                     const QString &sex,
                                     const int id)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO Patients (PatientID, Name, BirthDate, Sex, GroupID, ID, RaceID) VALUES (?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(id);
    query.addBindValue(name);
    query.addBindValue(QDate::fromString(birthDate));
    query.addBindValue(sex.startsWith("m", Qt::CaseSensitivity::CaseInsensitive));
    query.addBindValue(2);
    query.addBindValue(id);
    query.addBindValue(1);

    if (!query.exec()) {
        qWarning() << "Database error:" << m_db.lastError().text();
        return false;
    }

    return true;

}

QVariantMap TonometerManager::extractMeasures(const int patientId, const QString &eye)
{
    QSqlQuery query(m_db);
    QVariantMap resultMap;

    query.prepare("SELECT * from Measures where PatientID = ? and Eye = ? ORDER BY MeasureDate desc");

    query.addBindValue(patientId);
    query.addBindValue(eye);

    if (!query.exec()) {
        qWarning() << "Database error:" << m_db.lastError().text();
    }

    while (query.next()) {
        for (int i = 0; i < query.record().count(); ++i) {
            resultMap.insert(query.record().fieldName(i), query.value(i));
        }
    }

    return resultMap;
}

bool TonometerManager::restoreDatabase()
{
    QSqlQuery query(m_db);

    query.prepare("DELETE FROM Patients WHERE PatientID = ?");
    query.addBindValue(m_session->getBarcode().toInt());

    if (!query.exec()) {
        qWarning() << "Error deleting data from table:" << query.lastError().text();
        return false;
    }

    query.prepare("DELETE FROM Measures WHERE PatientID = ?");
    query.addBindValue(m_session->getBarcode().toInt());

    if (!query.exec()) {
        qWarning() << "Error deleting data from table:" << query.lastError().text();
        return false;
    }

    return true;
}

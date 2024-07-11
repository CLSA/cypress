#include "oct_manager.h"

#include "data/oct_test.h"

#include "server/sessions/oct_session.h"

#include <QFileInfo>
#include <QMessageBox>

#include <QSqlQuery>
#include <QSqlError>


OCTManager::OCTManager(QSharedPointer<OCTSession> session): ManagerBase { session }
{
    m_test.reset(new OCTTest);

    m_runnableName = CypressSettings::readSetting("oct/dicom/runnableName").toString();
    m_runnablePath = CypressSettings::readSetting("oct/dicom/runnablePath").toString();
    m_webpage = CypressSettings::readSetting("oct/webpage").toString();

    m_databaseName = CypressSettings::readSetting("oct/database/name").toString();
    m_databasePort = CypressSettings::readSetting("oct/database/port").toString();
    m_databaseUser = CypressSettings::readSetting("oct/database/user").toString();
}

OCTManager::~OCTManager()
{
}

bool OCTManager::isInstalled()
{
    if (CypressSettings::isSimMode())
        return true;

    const QString runnableName = CypressSettings::readSetting("oct/runnableName").toString();
    const QString runnablePath = CypressSettings::readSetting("oct/runnablePath").toString();
    const QString webpage = CypressSettings::readSetting("oct/webpage").toString();

    const QString databaseName = CypressSettings::readSetting("oct/database/name").toString();
    const QString databasePort = CypressSettings::readSetting("oct/database/port").toString();
    const QString databaseUser = CypressSettings::readSetting("oct/database/user").toString();

    if (runnableName.isNull() || runnableName.isEmpty()) {
        qDebug() << "runnableName is not defined";
        return false;
    }

    if (runnablePath.isNull() || runnablePath.isEmpty()) {
        qDebug() << "runnablePath is not defined";
        return false;
    }

    if (webpage.isNull() || webpage.isEmpty()) {
        qDebug() << "webpage is not defined";
        return false;
    }

    if (databaseName.isNull() || databaseName.isEmpty()) {
        qDebug() << "databaseName is not defined";
        return false;
    }

    const QFileInfo exeInfo(runnableName);
    if (!exeInfo.exists()) {
        qDebug() << "runnableName does not exist at" << runnableName;
        return false;
    }
    if (!exeInfo.isExecutable()) {
        qDebug() << "runnableName is not executable at" << runnableName;
        return false;
    }

    const QFileInfo workingDir(runnablePath);
    if (!workingDir.exists()) {
        qDebug() << "working directory does not exist at" << workingDir;
        return false;
    }
    if (!workingDir.isDir()) {
        qDebug() << "working directory is not writable at" << workingDir;
        return false;
    }
    if (!workingDir.isWritable()) {
        qDebug() << "working directory is not writable at" << workingDir;
        return false;
    }

    return true;
}

bool OCTManager::start()
{
    qDebug() << "OCTManager::start";

    // Setup process
    qDebug() << "OCTManager::start - setup process";
    const QString command = m_runnableName;
    QStringList arguments;
    arguments << m_webpage;

    m_process.setProgram(command);
    m_process.setArguments(arguments);
    m_process.setWorkingDirectory(m_runnablePath);
    m_process.setProcessChannelMode(QProcess::ForwardedChannels);

    connect(&m_process, &QProcess::started, this, [=]() {
        qDebug() << "process started: " << m_process.arguments().join(" ");
    });

    connect(&m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [=]() {
        qDebug() << "process finished, reading output";
        readOutput();
    });

    qDebug() << "OCTManager::start - connect to db";

    // Connect to database
    m_db = QSqlDatabase::addDatabase("QODBC");
    m_db.setDatabaseName(m_databaseName);
    if (!m_db.open())
        return false;

    // Cleanup database
    qDebug() << "OCTManager::start - cleanup db";
    QSqlQuery query(m_db);

    query.prepare("DELETE * FROM Patients");
    if (!query.exec()) {
        qDebug() << query.lastError().text();
        return false;
    }
    query.prepare("DELETE * FROM People");
    if (!query.exec()) {
        qDebug() << query.lastError().text();
        return false;
    }
    query.prepare("DELETE * FROM OCTDSA");
    if (!query.exec()) {
        qDebug() << query.lastError().text();
        return false;
    }
    query.prepare("DELETE * FROM Media");
    if (!query.exec()) {
        qDebug() << query.lastError().text();
        return false;
    }
    query.prepare("DELETE * FROM Exams");
    if (!query.exec()) {
        qDebug() << query.lastError().text();
        return false;
    }

    // Prepare database
    query.prepare("INSERT INTO Persons (PersonUid, SurName, ForeName) VALUES (:personUid, :firstName, :lastName");
    query.bindValue(":personUid", defaultPersonUUID);
    query.bindValue(":firstName", "CLSA");
    query.bindValue(":lastName", "Participant");
    if (!query.exec()) {
        qDebug() << query.lastError().text();
        return false;
    }

    query.prepare("INSERT INTO Patients (PatientUid, PatientIdentifier, PersonUid) VALUES (:patientUid, :participantId, :personUUID");
    query.bindValue(":patientUUID", defaultPatientUUID);
    query.bindValue(":participantId", m_session->getBarcode());
    query.bindValue(":personUUID", defaultPatientUUID);
    if (!query.exec()) {
        qDebug() << query.lastError().text();
        return false;
    }

    qDebug() << "OCTManager::start - start ImageNet";
    // Open IMAGENet webpage
    m_process.start();
    if (!m_process.waitForStarted())
        return false;

    emit started(m_test);
    emit dataChanged(m_test);
    emit canMeasure();

    return true;
}

void OCTManager::readOutput()
{
    qDebug() << "OCTManager::readOutput";
    measure();
}

void OCTManager::measure()
{
    qDebug() << "OCTManager::measure";
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM OCTDSA WHERE PatientID = :patientId");

    if (!query.exec()) {
        qWarning() << "Select failed";
        return;
    }

    if (!query.size()) {
        qWarning() << "Select returned 0 records";
        return;
    }

    query.prepare("SELECT * FROM Media WHERE PatientID = :patientId");
    if (!query.exec()) {
        qWarning() << "Select returned 0 records";
        return;
    }
    if (!query.size()) {
        qWarning() << "Select returned 0 records";
        return;
    }

    if (m_test->isValid()) {
        emit canFinish();
    }
}

void OCTManager::finish()
{
    qDebug() << "OCTManager::finish";

}

bool OCTManager::clearData()
{
    qDebug() << "OCTManager::clearData";
    m_test->reset();


    QJsonObject json = m_test->toJsonObject();


    return true;
}

bool OCTManager::setUp()
{
    qDebug() << "OCTManager::setUp";

    return true;
}

bool OCTManager::cleanUp()
{
    qDebug() << "OCTManager::cleanUp";

    return true;
}

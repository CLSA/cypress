#include "gripstrengthmanager.h"
#include "../auxiliary/tracker5util.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQuery>

#include <QTemporaryDir>
#include <QFile>
#include <QtSql>

GripStrengthManager::GripStrengthManager(QObject *parent)
    : QObject(parent)
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");

    m_workingDirPath = settings.value("instruments/grip_strength/working_dir").toString();
    if (m_workingDirPath.isEmpty() || m_workingDirPath.isNull())
    {
        const QString error = "working directory path for tracker 5 is not defined";
        qCritical() << error;
        throw std::runtime_error(error.toStdString());
    }

    m_executablePath = settings.value("instruments/grip_strength/executable").toString();
    if (m_executablePath.isEmpty() || m_executablePath.isNull())
    {
        const QString error = "executable path for Tracker 5 is not defined";
        qCritical() << error;
        throw std::runtime_error(error.toStdString());
    }

    m_databaseName = settings.value("instruments/grip_strength/databaseName").toString();
    if (m_databaseName.isEmpty() || m_databaseName.isNull())
    {
        const QString error = "database name is not defined";
        qCritical() << error;
        throw std::runtime_error(error.toStdString());
    }
}

GripStrengthManager::~GripStrengthManager()
{
    if (m_database.isOpen())
    {
        m_database.close();
    }
}

void GripStrengthManager::start()
{
    initialize();
    //startApp();
}

void GripStrengthManager::initialize()
{
    createDatabaseBackupFolder();
    backupDatabase();
    openDatabase();
}

void GripStrengthManager::startApp()
{
    m_process.setProgram(m_workingDirPath + m_executablePath);
    m_process.start();

    connect(&m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [this]
    {
        finish();
    });

    m_process.waitForStarted();
}

void GripStrengthManager::measure()
{
    QMap<QString, QVariant> exam = extractExam();
    QVector<QMap<QString, QVariant>> trialData;

    QSqlQuery query(m_database);
    query.prepare("SELECT * FROM ZGripTestData");
    if (!query.exec())
    {
        const QString error = "Error executing query:" + query.lastError().text();
        qCritical() << error;
        throw std::runtime_error(error.toStdString());
    }

    while (query.next())
    {
        QVariant examMax = query.value("Maximum");
        QVariant avg = query.value("Average");
        QVariant cv = query.value("CV");

        for (int i = 1; i <= 8; i++)
        {
            QVariant side = query.value("Side");
            QVariant rungPosition = query.value("Position");
            QVariant rep = query.value(QString("Rep%1").arg(i));
            int excludeFieldIndex = query.record().indexOf("Rep" + QString::number(i) + "Exclude");
            QVariant exclude = query.record().value(excludeFieldIndex);

            if (!rep.isNull() && (exclude.isNull() || exclude.toInt() == 0))
            {
                QMap<QString, QVariant> map = exam; // copy

                map.insert("Side", side);
                map.insert("Position", rungPosition.toInt());
                map.insert("Rep", rep.toDouble());
                map.insert("Max", examMax.toDouble());
                map.insert("Avg", avg.toDouble());
                map.insert("CV", cv.toInt());

                trialData.append(map);
            }
        }
    }

    QJsonArray trialArray;
    for (const auto& trial : trialData)
    {
        QJsonObject trialObject;
        for (auto it = trial.begin(); it != trial.end(); ++it)
        {
            trialObject.insert(it.key(), QJsonValue::fromVariant(it.value()));
        }
        trialArray.append(trialObject);
    }

    QJsonObject examObject;
    for (auto it = exam.begin(); it != exam.end(); ++it)
    {
        examObject.insert(it.key(), QJsonValue::fromVariant(it.value()));
    }

    examObject.insert("trials", trialArray);

    m_exam = QJsonDocument(examObject);
    emit validExam(m_exam);

    qDebug().noquote() << m_exam.toJson(QJsonDocument::Indented);
}

void GripStrengthManager::finish()
{
    measure();
    sendToPine();
    restoreTrackerDatabase();
}

QMap<QString, QVariant> GripStrengthManager::extractExam() const
{
    QMap<QString, QVariant> examData;
    QSqlQuery query(m_database);

    query.prepare("SELECT * FROM ZGripTest");
    if (!query.exec())
    {
        const QString error = "Error executing query:" + query.lastError().text();
        qWarning() << error;
        throw std::runtime_error(error.toStdString());
    }

    if (!query.next())
    {
        qWarning() << "No records found in ZGripTest";
        throw std::runtime_error("No records found in ZGripTest");
    }

    QSqlRecord record = query.record();
    QStringList fields = { "Rung", "MaxReps", "Sequence", "RestTime", "Rate", "Threshold", "NormType", "Comparison" };
    for (const QString &field : fields)
    {
        QVariant value = record.value(field);
        if (field == "Threshold")
        {
            value = Tracker5Util::asKg(value.toInt());
        }
        examData.insert(field, value);
        qDebug() << field << ":" << value;
    }

    return examData;
}

void GripStrengthManager::openDatabase() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName(m_databaseName);

    if (!db.open())
    {
        const QString error = "Failed to open database:" + db.lastError().text();
        qWarning() << error;
        throw std::runtime_error(error.toStdString());
    }

    m_database = db;
}

void GripStrengthManager::restoreTrackerDatabase() {
    if (!m_trackerDir.removeRecursively())
    {
        throw std::runtime_error("Failed to remove trackerDir");
    }

    for (const QString &fileName : m_backupDir.entryList(QDir::Files))
    {
        if (!QFile::copy(m_backupDir.filePath(fileName), m_trackerDir.filePath(fileName)))
        {
            throw std::runtime_error("Failed to open database");
        }
    }

    if (!m_backupDir.removeRecursively())
    {
       throw std::runtime_error("Failed to remove backupDir");
    }
}

void GripStrengthManager::createDatabaseBackupFolder() {
    QString backupFolder = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/database_backup";
    m_backupDir = QDir(backupFolder);
    if (!m_backupDir.exists())
    {
        if (!m_backupDir.mkpath(backupFolder))
        {
            throw std::runtime_error("Error creating temp directory");
        }
    }
}

void GripStrengthManager::backupDatabase() {
    for (const QString &fileName : m_trackerDir.entryList(QDir::Files))
    {
        if (!QFile::copy(m_trackerDir.filePath(fileName), m_backupDir.filePath(fileName)))
        {
            throw std::runtime_error("Could not backup file");
        }
    }
}

void GripStrengthManager::sendToPine()
{
    if (m_exam.isEmpty() || m_exam.isNull())
    {
        return;
    }
}


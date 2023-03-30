#include "gripstrengthmanager.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQuery>

#include <QTemporaryDir>
#include <QFile>
#include <QtSql>

#include "../auxiliary/tracker5util.h"

GripStrengthManager::GripStrengthManager(QObject *parent)
    : QObject(parent)
{
}

GripStrengthManager::~GripStrengthManager()
{
    if (database.isOpen()) {
        database.close();
    }
}

void GripStrengthManager::setTrackerDatabaseName(const QString &name)
{
    trackerDatabaseName = name;
}

void GripStrengthManager::initialize()
{
    createDatabaseBackupFolder();
    backupTrackerDatabase();

    database = getTracker5DB();
    if (!database.isOpen()) {
        qWarning() << "Cannot open database:" << database.lastError().text();
    }
}

void GripStrengthManager::run()
{
    qDebug() << "Launching Tracker 5 software";
    extractTrials();
}

void GripStrengthManager::shutdown()
{
    restoreTrackerDatabase();
}

QByteArray GripStrengthManager::extractTrials()
{
    QMap<QString, QVariant> exam = extractExam();
    QVector<QMap<QString, QVariant>> trialData;

    QSqlQuery query(database);
    query.prepare("SELECT * FROM ZGripTestData");
    if (!query.exec()) {
        qWarning() << "Error executing query:" << query.lastError().text();
        //throw std::exception;
    }

    while (query.next()) {
        QVariant examMax = query.value("Maximum");
        QVariant avg = query.value("Average");
        QVariant cv = query.value("CV");

        for (int i = 1; i <= 8; i++) {
            QVariant side = query.value("Side");
            QVariant rungPosition = query.value("Position");
            QVariant rep = query.value(QString("Rep%1").arg(i));
            int excludeFieldIndex = query.record().indexOf("Rep" + QString::number(i) + "Exclude");
            QVariant exclude = query.record().value(excludeFieldIndex);

            if (!rep.isNull() && (exclude.isNull() || exclude.toInt() == 0)) {
                QMap<QString, QVariant> map = exam;
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

    // Convert the trials vector to a JSON array
    QJsonArray trialArray;
    for (const auto& trial : trialData) {
        QJsonObject trialObject;
        for (auto it = trial.begin(); it != trial.end(); ++it) {
            trialObject.insert(it.key(), QJsonValue::fromVariant(it.value()));
        }
        trialArray.append(trialObject);
    }

    // Add the exam data to a JSON object
    QJsonObject examObject;
    for (auto it = exam.begin(); it != exam.end(); ++it) {
        examObject.insert(it.key(), QJsonValue::fromVariant(it.value()));
    }

    // Add the trial array to the exam object
    examObject.insert("trials", trialArray);

    // Convert the exam object to a JSON document and pretty print it to the console
    QJsonDocument doc(examObject);
    qDebug().noquote() << doc.toJson(QJsonDocument::Indented);

    return doc.toBinaryData();
}

QMap<QString, QVariant> GripStrengthManager::extractExam()
{
    QMap<QString, QVariant> map;
    QSqlQuery query(database);
    query.prepare("SELECT * FROM ZGripTest");
    if (!query.exec()) {
        qWarning() << "Error executing query:" << query.lastError().text();
        return map;
    }

    if (!query.next()) {
        qWarning() << "No records found in ZGripTest";
        return map;
    }

    QSqlRecord record = query.record();
    QStringList fields = { "Rung", "MaxReps", "Sequence", "RestTime", "Rate", "Threshold", "NormType", "Comparison" };
    for (const QString &field : fields) {
        QVariant value = record.value(field);
        if (field == "Threshold") {
            value = Tracker5Util::asKg(value.toInt());
        }
        map.insert(field, value);
        qDebug() << field << ":" << value;
    }

    return map;
}

QSqlDatabase GripStrengthManager::getTracker5DB() {
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");

    QString workingDir = settings.value("instruments/grip_strength/working_dir").toString();
    QString databaseName = settings.value("instruments/grip_strength/database").toString();

    QDir databaseDir(workingDir + databaseName);
    qDebug() << "database dir: " << databaseDir.absolutePath();

    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Tracker 5 Data");
    if (!db.open()) {
        qWarning() << "Failed to open database:" << db.lastError().text();
    }

    return db;
}

void GripStrengthManager::restoreTrackerDatabase() {
    // Copy backed-up database files back into the tracker database folder
    trackerDir.removeRecursively();
    for (const QString &fileName : backupDir.entryList(QDir::Files)) {
        QFile::copy(backupDir.filePath(fileName), trackerDir.filePath(fileName));
    }
    backupDir.removeRecursively();
}

void GripStrengthManager::createDatabaseBackupFolder() {
    QString backupFolder = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/database_backup";
    backupDir = QDir(backupFolder);
    if (!backupDir.exists()) {
        if (!backupDir.mkpath(backupFolder)) {
            throw std::runtime_error("Error creating temp directory");
        }
    }
}

void GripStrengthManager::backupTrackerDatabase() {
    // Copy database files out of the database folder
    if (!backupDir.exists()) {
        throw std::runtime_error("Error: backup dir does not exist");
    }
    for (const QString &fileName : trackerDir.entryList(QDir::Files)) {
        QFile::copy(trackerDir.filePath(fileName), backupDir.filePath(fileName));
    }

    qDebug() << backupDir.absolutePath();
}


void GripStrengthManager::sendToPine(const QMap<QString, QVariant> &values) {

}


#include "dxa_hip_test.h"

#include "data/dxa/measurements/hip_measurement.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

#include <QDir>
#include <QJsonObject>

const QMap<QString, QString> DxaHipTest::ranges = {
    // hip
    {"NECK_BMD", "1..."},
    {"TROCH_BMD", ".2.."},
    {"INTER_BMD", "..3."},
    {"WARDS_BMD", "...4"},
    {"HTOT_BMD", "123."},
};

DxaHipTest::DxaHipTest()
{
    leftHipMeasurement = QSharedPointer<HipMeasurement>(new HipMeasurement(Side::LEFT));
    rightHipMeasurement = QSharedPointer<HipMeasurement>(new HipMeasurement(Side::RIGHT));
}

void DxaHipTest::fromDicomFiles(QList<DicomFile> files, const DxaHipSession &session)
{
    foreach (const DicomFile &file, files) {
        if (file.patientId != session.getBarcode()) {
            qDebug() << "patientId" << file.patientId << "does not match session barcode"
                     << session.getBarcode();
            continue;
        }

        if (file.bodyPartExamined == "HIP") {
            if (file.laterality == "L") {
                if (m_debug)
                    qDebug() << "DxaHipTest::fromDicomFiles - found left hip";
                QSharedPointer<HipMeasurement> measure(new HipMeasurement(Side::LEFT));
                if (measure->isValidDicomFile(file))
                measure->addDicomFile(file);
                addMeasurement(measure);
                leftHipMeasurement = measure;
            } else {
                if (m_debug)
                    qDebug() << "DxaHipTest::fromDicomFiles - found right hip";
                QSharedPointer<HipMeasurement> measure(new HipMeasurement(Side::RIGHT));
                if (measure->isValidDicomFile(file))
                measure->addDicomFile(file);
                addMeasurement(measure);

                rightHipMeasurement = measure;
            }
        }
    }
}



bool DxaHipTest::hasAllNeededFiles() const
{
    return leftHipMeasurement->hasAllNeededFiles();
}

void DxaHipTest::getPatientScan(const QSqlDatabase &db, const QString &participantId) {
    QSqlQuery query(db);

    query.prepare("SELECT PATIENT_KEY, BIRTHDATE, SEX, ETHNICITY, WEIGHT, HEIGHT FROM PATIENT WHERE IDENTIFIER1 = :participantId");
    query.bindValue(":participantId", participantId);

    if (!query.exec()) {
        qDebug() << query.lastError().text();
        throw QException();
    }

    if (!query.first()) {
        return;
    }

    addMetaData("PATIENT_KEY",  query.value("PATIENT_KEY").toString());
    addMetaData("BIRTHDATE",    query.value("BIRTHDATE").toString());
    addMetaData("SEX",          query.value("SEX").toString());
    addMetaData("ETHNICITY",    query.value("ETHNICITY").toString());
    addMetaData("WEIGHT", 	    query.value("WEIGHT").toDouble());
    addMetaData("HEIGHT",       query.value("HEIGHT").toDouble());
}

bool DxaHipTest::isValid() const
{
    return leftHipMeasurement->isValid() || rightHipMeasurement->isValid();
}

void DxaHipTest::reset()
{
    leftHipMeasurement->reset();
    rightHipMeasurement->reset();
}

void DxaHipTest::simulate()
{
    leftHipMeasurement->simulate();
    rightHipMeasurement->simulate();
}

QJsonObject DxaHipTest::toJsonObject() const
{
    QJsonObject json{};
    QJsonObject results{};
    QJsonObject hip{};

    results.insert("hip_l", leftHipMeasurement->toJsonObject());
    results.insert("hip_r", rightHipMeasurement->toJsonObject());

    json.insert("results", results);
    json.insert("manual_entry", getManualEntryMode());

    return json;
}


QString DxaHipTest::toString() const
{
    return "";
}

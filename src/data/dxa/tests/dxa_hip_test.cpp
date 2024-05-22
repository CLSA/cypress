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
}

int DxaHipTest::fromDicomFiles(QList<DicomFile> files, const DxaHipSession &session)
{
    int filesReceived = 0;

    foreach (const DicomFile &file, files) {
        if (file.patientId != session.getBarcode()) {
            qDebug() << "DxaHipTest::fromDicomFiles: patientId" << file.patientId
                     << "does not match session barcode" << session.getBarcode();
            continue;
        }

        if (file.bodyPartExamined != "HIP") {
            qWarning() << "DxaHipTest::fromDicomFiles: received a non-hip dicom file";
            continue;
        }

        if (HipMeasurement::isValidDicomFile(file)) {
            QSharedPointer<HipMeasurement> measure(new HipMeasurement);
            measure->addDicomFile(file);
            addMeasurement(measure);
            filesReceived++;
        }
    }

    return filesReceived;
}

void DxaHipTest::getScanAnalysisData(
    const QSqlDatabase& patscanDb,
    const QSqlDatabase& referenceDb,
    const QJsonObject& patientData) {

    foreach(auto measure, m_measurementList) {
        auto dxaMeasure = qSharedPointerDynamicCast<DXAMeasurement>(measure);
        dxaMeasure->getScanAnalysisData(patscanDb, referenceDb, patientData);
    }
}

bool DxaHipTest::hasAllNeededFiles() const
{
    return true;
}

void DxaHipTest::getPatientScan(const QSqlDatabase &db, const QString &participantId) {
    QSqlQuery query(db);

    query.prepare("SELECT PATIENT_KEY, BIRTHDATE, SEX, ETHNICITY, WEIGHT, HEIGHT FROM PATIENT WHERE IDENTIFIER1 = :participantId");
    query.bindValue(":participantId", participantId);

    if (!query.exec()) {
        qDebug() << query.lastError().text();
        throw QException();
    }

    while (query.next()) {
        addMetaData("PATIENT_KEY",  query.value("PATIENT_KEY").toString());
        addMetaData("BIRTHDATE",    query.value("BIRTHDATE").toString());
        addMetaData("SEX",          query.value("SEX").toString());
        addMetaData("ETHNICITY",    query.value("ETHNICITY").toString());
        addMetaData("WEIGHT", 	    query.value("WEIGHT").toDouble());
        addMetaData("HEIGHT",       query.value("HEIGHT").toDouble());
    }
}

bool DxaHipTest::isValid() const
{
    return true;
}

void DxaHipTest::reset()
{
    TestBase::reset();
}

QJsonObject DxaHipTest::toJsonObject() const
{
    QJsonObject json{};
    QJsonObject results{};
    QJsonObject hip{};

    //results.insert("hip_l", leftHipMeasurement->toJsonObject());
    //results.insert("hip_r", rightHipMeasurement->toJsonObject());

    json.insert("results", results);
    json.insert("manual_entry", getManualEntryMode());

    return json;
}


QString DxaHipTest::toString() const
{
    return "";
}

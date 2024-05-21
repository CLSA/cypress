#include "dxa_test.h"
#include "dicom/dcm_recv.h"

#include "data/dxa/measurements/ap_spine_measurement.h"
#include "data/dxa/measurements/forearm_measurement.h"
#include "data/dxa/measurements/iva_imaging_measurement.h"
#include "data/dxa/measurements/whole_body_measurement.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDir>
#include <QJsonObject>

const QMap<QString, QString> DXATest::ranges = {
    // forearm
    {"RU13TOT_BMD", "1.."},
    {"RUMIDTOT_BMD", ".2."},
    {"RUUDTOT_BMD", "..3"},
    {"RUTOT_BMD", "123"},
    {"R_13_BMD", "R.."},
    {"R_MID_BMD", ".R."},
    {"R_UD_BMD", "..R"},
    {"RTOT_BMD", "RRR"},
    {"U_13_BMD", "U.."},
    {"U_MID_BMD", ".U."},
    {"U_UD_BMD", "..U"},
    {"UTOT_BMD", "UUU"},

    // whole body
    {"WBTOT_BMD", "NULL"},

    // hip
    {"NECK_BMD", "1..."},
    {"TROCH_BMD", ".2.."},
    {"INTER_BMD", "..3."},
    {"WARDS_BMD", "...4"},
    {"HTOT_BMD", "123."},

    // ap lumbar spine
    {"L1_BMD", "1..."},
    {"L2_BMD", ".2.."},
    {"L3_BMD", "..3."},
    {"L4_BMD", "...4"},
    {"TOT_BMD", "1234"},
    {"TOT_L1_BMD", "1..."},
    {"TOT_L2_BMD", ".2.."},
    {"TOT_L3_BMD", "..3."},
    {"TOT_L4_BMD", "...4"},
    {"TOT_L1L2_BMD", "12.."},
    {"TOT_L1L3_BMD", "1.3."},
    {"TOT_L1L4_BMD", "1..4"},
    {"TOT_L2L3_BMD", ".23."},
    {"TOT_L2L4_BMD", ".2.4"},
    {"TOT_L3L4_BMD", "..34"},
    {"TOT_L1L2L3_BMD", "123."},
    {"TOT_L1L2L4_BMD", "12.4"},
    {"TOT_L1L3L4_BMD", "1.34"},
    {"TOT_L2L3L4_BMD", ".234"},
};

DXATest::DXATest()
{
    wholeBodyMeasurement.reset(new WholeBodyScanMeasurement);
    apSpineMeasurement.reset(new ApSpineMeasurement);
    leftForearmMeasurement.reset(new ForearmMeasurement(Side::LEFT));
    rightForearmMeasurement.reset(new ForearmMeasurement(Side::RIGHT));
    ivaImagingMeasurement.reset(new IVAImagingMeasurement);
}

bool DXATest::isValid() const
{
    return true;
}

void DXATest::reset()
{
    apSpineMeasurement->reset();
    wholeBodyMeasurement->reset();

    leftForearmMeasurement->reset();
    rightForearmMeasurement->reset();

    ivaImagingMeasurement->reset();
}

bool DXATest::hasAllNeededFiles() const
{
    return true;
}

int DXATest::fromDicomFiles(QList<DicomFile> files, const DXASession &session)
{
    int filesReceived = 0;
    foreach (const DicomFile &file, files) {
        if (file.patientId != session.getBarcode()) {
            qDebug() << "file patient id " << file.patientId << "does not equal session barcode"
                     << session.getBarcode();
            continue;
        }

        QString bodyPartExamined = file.bodyPartExamined;
        QString patientId = file.patientId;

        if (bodyPartExamined == "HIP") // hip done in different stage
            continue;

        qDebug() <<
            "checking file: " <<
            file.bodyPartExamined <<
            file.seriesNumber;

        if (ivaImagingMeasurement->isValidDicomFile(file)) {
            ivaImagingMeasurement->addDicomFile(file);
            filesReceived++;
        }
        else if (wholeBodyMeasurement->isValidDicomFile(file)) {
            wholeBodyMeasurement->addDicomFile(file);
            filesReceived++;
        }
        else if (file.bodyPartExamined == "ARM" && file.laterality == "L" && leftForearmMeasurement->isValidDicomFile(file)) {
            leftForearmMeasurement->addDicomFile(file);
            filesReceived++;
        }
        else if (file.bodyPartExamined == "ARM" && file.laterality == "R" && rightForearmMeasurement->isValidDicomFile(file)) {
            rightForearmMeasurement->addDicomFile(file);
            filesReceived++;
        }
        else if (apSpineMeasurement->isValidDicomFile(file)) {
            apSpineMeasurement->addDicomFile(file);
            filesReceived++;
        }
        else {
            qDebug() << "Unknown file";
        }
    }

    return filesReceived;
}

void DXATest::getPatientScan(const QSqlDatabase &db, const QString &participantId) {
    QSqlQuery query(db);

    query.prepare("SELECT PATIENT_KEY, BIRTHDATE, SEX, ETHNICITY, WEIGHT, HEIGHT FROM PATIENT WHERE IDENTIFIER1 = :participantId");
    query.bindValue(":participantId", participantId);

    if (!query.exec()) {
        qDebug() << query.lastError().text();
        throw QException();
    }

    while (query.next()) {
        addMetaData("PATIENT_KEY", query.value("PATIENT_KEY").toString());
        addMetaData("BIRTHDATE",   query.value("BIRTHDATE").toString());
        addMetaData("SEX",         query.value("SEX").toString());
        addMetaData("ETHNICITY",   query.value("ETHNICITY").toString());
        addMetaData("WEIGHT", 	   query.value("WEIGHT").toDouble());
        addMetaData("HEIGHT",      query.value("HEIGHT").toDouble());
    }
}

void DXATest::simulate()
{
    wholeBodyMeasurement->simulate();
    apSpineMeasurement->simulate();
    leftForearmMeasurement->simulate();
    rightForearmMeasurement->simulate();
    ivaImagingMeasurement->simulate();
}

QJsonObject DXATest::toJsonObject() const
{
    auto wholeBody = QSharedPointer<QJsonObject>::create();
    auto apSpine = QSharedPointer<QJsonObject>::create();
    auto leftForearm = QSharedPointer<QJsonObject>::create();
    auto rightForearm = QSharedPointer<QJsonObject>::create();
    auto ivaSpine = QSharedPointer<QJsonObject>::create();

    QJsonObject json{};
    QJsonObject results{};

    if (wholeBodyMeasurement)
        *wholeBody = wholeBodyMeasurement->toJsonObject();

    if (apSpineMeasurement)
        *apSpine = apSpineMeasurement->toJsonObject();

    if (leftForearmMeasurement)
        *leftForearm = leftForearmMeasurement->toJsonObject();

    if (rightForearmMeasurement)
        *rightForearm = rightForearmMeasurement->toJsonObject();

    if (ivaImagingMeasurement)
        *ivaSpine = ivaImagingMeasurement->toJsonObject();

    results.insert("ap_spine", *apSpine);
    results.insert("whole_body", *wholeBody);
    results.insert("forearm_left", *leftForearm);
    results.insert("forearm_right", *rightForearm);
    results.insert("iva", *ivaSpine);

    json.insert("results", results);
    json.insert("manual_entry", getManualEntryMode());

    return json;
}


QString DXATest::toString() const
{
    return "";
}






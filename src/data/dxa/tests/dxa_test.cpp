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
}

bool DXATest::isValid() const
{
    return true;
}

void DXATest::reset()
{
    TestBase::reset();
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
            qWarning()
                << "DXATest::fromDicomFiles: patientId" << file.patientId
                << "does not equal session barcode" << session.getBarcode();
            continue;
        }

        QString bodyPartExamined = file.bodyPartExamined;

        if (bodyPartExamined == "HIP") { // hip done in different stage
            qWarning() << "DXATest::fromDicomFiles: received a hip dicom file";
            continue;
        }

        if (IVAImagingMeasurement::isValidDicomFile(file)) {
            auto measure = QSharedPointer<IVAImagingMeasurement>(new IVAImagingMeasurement);
            measure->addDicomFile(file);
            addMeasurement(qSharedPointerCast<Measurement>(measure));

            filesReceived++;
        }
        else if (WholeBodyScanMeasurement::isValidDicomFile(file)) {
            auto measure = QSharedPointer<WholeBodyScanMeasurement>(new WholeBodyScanMeasurement);
            measure->addDicomFile(file);
            addMeasurement(qSharedPointerCast<Measurement>(measure));

            filesReceived++;
        }
        else if (ForearmMeasurement::isValidDicomFile(file)) {
            auto measure = QSharedPointer<ForearmMeasurement>(new ForearmMeasurement);;
            measure->addDicomFile(file);
            addMeasurement(qSharedPointerCast<Measurement>(measure));

            filesReceived++;
        }
        else if (ApSpineMeasurement::isValidDicomFile(file)) {
            auto measure = QSharedPointer<ApSpineMeasurement>(new ApSpineMeasurement);
            measure->addDicomFile(file);
            addMeasurement(qSharedPointerCast<Measurement>(measure));

            filesReceived++;
        }
        else {
            qWarning() << "Unknown file";
        }
    }

    return filesReceived;
}

void DXATest::getPatientScan(const QSqlDatabase &db, const QString &participantId) {
    QSqlQuery query(db);

    query.prepare("SELECT PATIENT_KEY, BIRTHDATE, SEX, ETHNICITY, WEIGHT, HEIGHT "
                  "FROM PATIENT "
                  "WHERE IDENTIFIER1 = :participantId");
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

void DXATest::getScanAnalysisData(
    const QSqlDatabase& patscanDb,
    const QSqlDatabase& referenceDb,
    const QJsonObject& patientData) {

    foreach (auto measure, m_measurementList) {
        auto dxaMeasure = qSharedPointerDynamicCast<DXAMeasurement>(measure);

        QString fileName = dxaMeasure->getAttribute("NAME").toString();

        QStringList doScanAnalysis { "WB_DICOM_1", "SP_DICOM_1", "FA_DICOM" };

        if (doScanAnalysis.contains(fileName))
            dxaMeasure->getScanAnalysisData(patscanDb, referenceDb, patientData);
    }
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

    foreach (auto measure, m_measurementList) {
        if (qSharedPointerDynamicCast<WholeBodyScanMeasurement>(measure)) {
            if (measure->getAttribute("NAME").toString() == "WB_DICOM_1")
                *wholeBody = measure->toJsonObject();
        }
        else if (qSharedPointerDynamicCast<ApSpineMeasurement>(measure)) {
            if (measure->getAttribute("NAME").toString() == "SP_DICOM_1")
            *apSpine = measure->toJsonObject();
        }
        else if (qSharedPointerDynamicCast<ForearmMeasurement>(measure)) {
            auto forearmMeasure = dynamic_cast<ForearmMeasurement*>(measure.get());
            if (forearmMeasure->getSide() == Side::LEFT) {
                *leftForearm = forearmMeasure->toJsonObject();
            }
            else if (forearmMeasure->getSide() == Side::RIGHT) {
                *rightForearm = forearmMeasure->toJsonObject();
            }
        }
        else if (qSharedPointerDynamicCast<IVAImagingMeasurement>(measure)) {
            *ivaSpine = measure->toJsonObject();
        }
    }

    results.insert("ap_spine", *apSpine);
    results.insert("whole_body", *wholeBody);
    results.insert("forearm_left", *leftForearm);
    results.insert("forearm_right", *rightForearm);
    results.insert("iva", *ivaSpine);

    json.insert("results", results);
    json.insert("manual_entry", getManualEntryMode());

    qDebug() << json;

    return json;
}


QString DXATest::toString() const
{
    return "";
}






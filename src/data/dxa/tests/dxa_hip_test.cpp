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
    leftHipMeasurement = QSharedPointer<HipMeasurement>(new HipMeasurement);
    rightHipMeasurement = QSharedPointer<HipMeasurement>(new HipMeasurement);
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
            QSharedPointer<HipMeasurement> measure(new HipMeasurement);
            if (file.laterality == "L" && measure->isValidDicomFile(file)) {
                if (m_debug)
                    qDebug() << "DxaHipTest::fromDicomFiles - found left hip";
                measure->addDicomFile(file);
                addMeasurement(measure);

                leftHipMeasurement = measure;
            } else {
                if (m_debug)
                    qDebug() << "DxaHipTest::fromDicomFiles - found right hip";

                measure->addDicomFile(file);
                addMeasurement(measure);

                rightHipMeasurement = measure;
            }
        }
    }
}

void DxaHipTest::retrieveResults(const QSqlDatabase &db, const QString &barcode)
{
    if (!db.isOpen()) {
        qDebug() << "DxaHipTest::db is not open";
        return;
    }

    QJsonObject leftScanResults = extractScanAnalysis(db, barcode, "21");
    QJsonObject rightScanResults = extractScanAnalysis(db, barcode, "31");

    if (!leftScanResults.isEmpty() && leftHipMeasurement) {
        QString scanId = leftScanResults["SCANID"].toString();
        QString scanMode = leftScanResults["SCAN_MODE"].toString();
        QString scanDate = leftScanResults["SCAN_DATE"].toString();

        leftHipMeasurement->setAttribute("SCANID", scanId);
        leftHipMeasurement->setAttribute("SCAN_MODE", scanMode);
        leftHipMeasurement->setAttribute("SCAN_DATE", scanDate);

        QJsonObject leftHipMeasurementData = extractScanData(db, barcode, scanId);
        for (auto key : leftHipMeasurementData.keys()) {
            QJsonValue value = leftHipMeasurementData.value(key);
            leftHipMeasurement->setAttribute(key, value);
        }
    }

    if (!rightScanResults.isEmpty() && rightHipMeasurement) {
        QString scanId = rightScanResults["SCANID"].toString();
        QString scanMode = rightScanResults["SCAN_MODE"].toString();
        QString scanDate = rightScanResults["SCAN_DATE"].toString();

        rightHipMeasurement->setAttribute("SCANID", scanId);
        rightHipMeasurement->setAttribute("SCAN_MODE", scanMode);
        rightHipMeasurement->setAttribute("SCAN_DATE", scanDate);

        QJsonObject rightHipMeasurementData = extractScanData(db, barcode, scanId);
        for (auto key : rightHipMeasurementData.keys()) {
            QJsonValue value = rightHipMeasurementData.value(key);
            rightHipMeasurement->setAttribute(key, value);
        }
    }
}

bool DxaHipTest::hasAllNeededFiles() const
{
    return leftHipMeasurement->hasAllNeededFiles();
}

QJsonObject DxaHipTest::extractScanAnalysis(const QSqlDatabase &db,
                                            const QString &barcode,
                                            const QString &scanType)
{
    QJsonObject scanJson{};

    QSqlQuery query;

    query.prepare("SELECT SCANID, SCAN_MODE, SCAN_DATE FROM ScanAnalysis WHERE PATIENT_KEY = "
                  ":patient_key AND SCAN_TYPE = :scan_type");
    query.bindValue(":patient_key", barcode);
    query.bindValue(":scan_type", scanType);

    if (!query.exec()) {
        qDebug() << query.lastError().text();
        return scanJson;
    }

    QString scanId{};
    QString scanMode{};
    QString scanDate{};

    // only get last result
    while (query.next()) {
        scanId = query.value(0).toString();
        scanMode = query.value(1).toString();
        scanDate = query.value(2).toString();
    }

    scanJson["SCANID"] = scanId;
    scanJson["SCAN_MODE"] = scanMode;
    scanJson["SCAN_DATE"] = scanDate;

    return scanJson;
}

QJsonObject DxaHipTest::extractScanData(const QSqlDatabase &db,
                                        const QString &barcode,
                                        const QString &scanId)
{
    QJsonObject recordJson;

    QSqlQuery query;
    query.prepare("SELECT * from Hip where PATIENT_KEY = :patient_key AND SCANID = :scan_id");
    query.bindValue(":patient_key", barcode);
    query.bindValue(":scan_id", scanId);

    if (!query.exec()) {
        qDebug() << query.lastError().text();
        return recordJson;
    }

    while (query.next()) {
        QSqlRecord record = query.record();
        for (int i = 0; i < record.count(); ++i) {
            QVariant value = record.value(i);
            recordJson[record.fieldName(i)] = value.toJsonValue();
        }
    }

    query.prepare("SELECT * from HipHSA where PATIENT_KEY = :patient_key AND SCANID = :scan_id");
    query.bindValue(":patient_key", barcode);
    query.bindValue(":scan_id", scanId);

    if (!query.exec()) {
        qDebug() << query.lastError().text();
    }

    while (query.next()) {
        QSqlRecord record = query.record();
        for (int i = 0; i < record.count(); ++i) {
            QVariant value = record.value(i);
            recordJson[record.fieldName(i)] = value.toJsonValue();
        }
    }

    return recordJson;
}

void DxaHipTest::computeTandZScores() {

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

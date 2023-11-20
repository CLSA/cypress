#include "dxa_test.h"
#include "../../../dicom/dcm_recv.h"

#include "dcmtk/dcmdata/dcfilefo.h"

#include "../measurements/ap_spine_measurement.h"
#include "../measurements/forearm_measurement.h"
#include "../measurements/whole_body_measurement.h"
#include "../measurements/iva_imaging_measurement.h"

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
    forearmMeasurement.reset(new ForearmMeasurement);
    ivaImagingMeasurement.reset(new IVAImagingMeasurement);
}

bool DXATest::isValid() const
{
    return 	wholeBodyMeasurement->isValid() 	||
            apSpineMeasurement->isValid() 		||
            forearmMeasurement->isValid() 		||
           ivaImagingMeasurement->isValid();
}

void DXATest::reset()
{
    wholeBodyMeasurement->reset();
    apSpineMeasurement->reset();
    forearmMeasurement->reset();
    ivaImagingMeasurement->reset();
}

bool DXATest::areDicomFilesValid() const
{
    return wholeBodyMeasurement->hasAllNeededFiles() && apSpineMeasurement->hasAllNeededFiles()
           && forearmMeasurement->hasAllNeededFiles();
}

void DXATest::fromDicomFiles(QList<DicomFile> files)
{
    foreach (const DicomFile &file, files) {
        QString bodyPartExamined = file.bodyPartExamined;

        if (bodyPartExamined == "HIP") // hip done in different stage
            continue;

        qDebug() << "checking file: " << file.bodyPartExamined << file.seriesNumber;

        if (ivaImagingMeasurement->isValidDicomFile(file)) {
            qDebug("Found IVA");
            ivaImagingMeasurement->addDicomFile(file);
        } else if (wholeBodyMeasurement->isValidDicomFile(file)) {
            qDebug("Found WB");
            wholeBodyMeasurement->addDicomFile(file);
        } else if (forearmMeasurement->isValidDicomFile(file)) {
            qDebug("Found Forearm");
            forearmMeasurement->addDicomFile(file);
        } else if (apSpineMeasurement->isValidDicomFile(file)) {
            qDebug("Found Spine");
            apSpineMeasurement->addDicomFile(file);
        } else {
            qDebug() << "Unknown file";
        }
    }
}

void DXATest::simulate()
{
    wholeBodyMeasurement->simulate();
    apSpineMeasurement->simulate();
    forearmMeasurement->simulate();
    ivaImagingMeasurement->simulate();
}

QJsonObject DXATest::toJsonObject() const
{
    QJsonObject wholeBody {};
    if (wholeBodyMeasurement)
    {
        wholeBody = wholeBodyMeasurement->toJsonObject();
    }

    QJsonObject apSpine {};
    if (apSpineMeasurement)
    {
        apSpine = apSpineMeasurement->toJsonObject();
    }

    QJsonObject forearm {};
    if (forearmMeasurement)
    {
        forearm = apSpineMeasurement->toJsonObject();
    }

    QJsonObject iva {};
    if (ivaImagingMeasurement)
    {
        iva = ivaImagingMeasurement->toJsonObject();
    }

    QJsonObject value {};
    QJsonObject results {};

    results.insert("ap_spine", apSpine);
    results.insert("whole_body", wholeBody);
    results.insert("forearm", forearm);
    results.insert("iva", iva);

    value.insert("results", results);
    value.insert("manual_entry", getManualEntryMode());

    QJsonObject json;
    return value;
}


QString DXATest::toString() const
{
    return "";
}






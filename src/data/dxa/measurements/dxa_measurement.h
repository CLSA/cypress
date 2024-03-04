#ifndef DXA_MEASUREMENT_H
#define DXA_MEASUREMENT_H

#include "data/measurement.h"
#include "dicom/dcm_recv.h"

#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/ofstd/ofstdinc.h"

#include <QSqlDatabase>

struct ValidDCMTag {
    ValidDCMTag(DcmTagKey _key, OFString _value) {
        key =_key;
        value = _value;
    }
    DcmTagKey key;
    OFString value;
};

enum Side {
    LEFT,
    RIGHT,
    BOTH
};

class DXAMeasurement : public Measurement
{
public:
    DXAMeasurement();

    QStringList m_mdb_keys {};
    QJsonObject readJsonFile(const QString &filePath);

    QList<ValidDCMTag> m_metaInfoTagExistsWithValue {};
    QList<ValidDCMTag> m_metaInfoTagExists {};
    QList<ValidDCMTag> m_datasetTagExistsWithValue {};
    QList<ValidDCMTag> m_datasetTagExists {};

    virtual bool isValidDicomFile(DicomFile file) const;

    virtual bool hasAllNeededFiles() const = 0;

    virtual Side getSide() = 0;
    virtual quint8 getScanType() = 0;
    virtual QString getName() = 0;
    virtual QString getBodyPartName() = 0;
    virtual QString getRefType() = 0;
    virtual QString getRefSource() = 0;

    virtual void simulate() {};

    virtual bool isValid() const override;
    virtual QString toString() const override;
    virtual QStringList toStringList(const bool& no_keys = false) const override;

    void getPatientScan(const QSqlDatabase& db, const QString& participantId);
    void getScanAnalysisData(const QSqlDatabase& patscanDb, const QSqlDatabase& referenceDb, const QJsonObject& patientData);
    void computeTZScore(const QSqlDatabase& referenceDb, const QJsonObject& patientData, const QString& scanDate);

    virtual void getScanData(const QSqlDatabase& db, const QString& patientKey, const QString& scanId) = 0;
    double computeYearsDifference(const QString& first, const QString& second);
};

#endif // DXA_MEASUREMENT_H

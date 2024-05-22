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

    void getPatientScan(const QSqlDatabase& db, const QString& participantId);
    void getScanAnalysisData(const QSqlDatabase& patscanDb, const QSqlDatabase& referenceDb, const QJsonObject& patientData);
    virtual void getScanData(const QSqlDatabase& db, const QString& patientKey, const QString& scanId) = 0;
    void computeTZScore(const QSqlDatabase& referenceDb, const QJsonObject& patientData, const QString& scanDate);


    double computeYearsDifference(const QString& first, const QString& second);

    virtual bool hasAllNeededFiles() const = 0;

public: // Measurement
    virtual bool isValid() const override;
    virtual QString toString() const override;

    virtual void simulate() {};

public: // getters
    virtual Side getSide() = 0;
    virtual quint8 getScanType() = 0;
    virtual QString getName() = 0;
    virtual QString getBodyPartName() = 0;
    virtual QString getRefType() = 0;
    virtual QString getRefSource() = 0;

};

#endif // DXA_MEASUREMENT_H

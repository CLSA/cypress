#ifndef DXA_MEASUREMENT_H
#define DXA_MEASUREMENT_H

#include "data/measurement.h"

#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmdata/dcuid.h"
#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmdata/dcmetinf.h"

#include "dcmtk/ofstd/ofstdinc.h"

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

    QList<ValidDCMTag> m_metaInfoTagExistsWithValue {};
    QList<ValidDCMTag> m_metaInfoTagExists {};
    QList<ValidDCMTag> m_datasetTagExistsWithValue {};
    QList<ValidDCMTag> m_datasetTagExists {};

    virtual bool isValidDicomFile(DcmFileFormat& dicomFileFormat) const;

    virtual Side getSide() = 0;
    virtual quint8 getScanType() = 0;
    virtual QString getName() = 0;
    virtual QString getBodyPartName() = 0;
    virtual QString getRefType() = 0;
    virtual QString getRefSource() = 0;

    virtual bool isValid() const override;
    virtual QJsonObject toJsonObject() const override;
    virtual QString toString() const override;
    virtual QStringList toStringList(const bool& no_keys = false) const override;
};

#endif // DXA_MEASUREMENT_H

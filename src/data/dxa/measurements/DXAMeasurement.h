#ifndef DXAMEASUREMENT_H
#define DXAMEASUREMENT_H

#include "../../Measurement.h"
#include "dcmtk/dcmdata/dcfilefo.h"
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

class DXAMeasurement : public Measurement
{
public:
    DXAMeasurement();

    QList<ValidDCMTag> m_metaInfoTagExistsWithValue {};
    QList<ValidDCMTag> m_metaInfoTagExists {};
    QList<ValidDCMTag> m_datasetTagExistsWithValue {};
    QList<ValidDCMTag> m_datasetTagExists {};

    bool isValidDicomFile(DcmFileFormat& dicomFileFormat) const;

    virtual bool isValid() const;
    virtual QJsonObject toJsonObject() const;
    virtual QString toString() const;
    virtual QStringList toStringList(const bool& no_keys = false) const;
};

#endif // DXAMEASUREMENT_H

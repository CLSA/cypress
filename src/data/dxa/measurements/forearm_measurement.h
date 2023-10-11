#ifndef FOREARM_MEASUREMENT_H
#define FOREARM_MEASUREMENT_H

#include "dxa_measurement.h"

#include "dcmtk/dcmdata/dcuid.h"
#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmdata/dcmetinf.h"

class ForearmMeasurement : public DXAMeasurement
{
public:
    ForearmMeasurement();

    // String representation for debug and GUI display purposes
    //
    virtual QString toString() const override;

    virtual QStringList toStringList(const bool& no_keys = false) const override;

    virtual bool isValid() const override;

    // String keys are converted to snake_case
    //
    virtual QJsonObject toJsonObject() const override;

    // DXAMeasurement interface
public:
    bool isValidDicomFile(DcmFileFormat &dicomFileFormat) const override;
    Side getSide() override;
    quint8 getScanType() override;
    QString getName() override;
    QString getBodyPartName() override;
    QString getRefType() override;
    QString getRefSource() override;
};

#endif // FOREARM_MEASUREMENT_H

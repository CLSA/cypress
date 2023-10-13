#ifndef FOREARM_MEASUREMENT_H
#define FOREARM_MEASUREMENT_H

#include "dxa_measurement.h"



class ForearmMeasurement : public DXAMeasurement
{
public:
    ForearmMeasurement();

    QString toString() const override;
    QStringList toStringList(const bool& no_keys = false) const override;

    bool isValid() const override;

    // DXAMeasurement interface
public:
    bool isValidDicomFile(DcmFileFormat& loadedFileFormat) const override;

    Side getSide() override;
    quint8 getScanType() override;
    QString getName() override;
    QString getBodyPartName() override;
    QString getRefType() override;
    QString getRefSource() override;
    void simulate() override;
};

#endif // FOREARM_MEASUREMENT_H

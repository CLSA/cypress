#ifndef FOREARM_MEASUREMENT_H
#define FOREARM_MEASUREMENT_H

#include "dxa_measurement.h"

#include "dicom/dcm_recv.h"

class ForearmMeasurement : public DXAMeasurement
{
public:
    ForearmMeasurement();

    QString toString() const override;

    bool isValid() const override;

    void addDicomFile(DicomFile);

    static bool isValidDicomFile(DicomFile file);

    void getScanData(const QSqlDatabase &db, const QString &patientKey, const QString &scanId) override;

    Side getSide() override;
    quint8 getScanType() override;
    QString getName() override;
    QString getBodyPartName() override;
    QString getRefType() override;
    QString getRefSource() override;

    bool m_hasDicomFile { false };

    // DXAMeasurement interface
public:
    bool hasAllNeededFiles() const override;

private:
    Side m_side;
};

#endif // FOREARM_MEASUREMENT_H

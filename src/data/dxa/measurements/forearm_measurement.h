#ifndef FOREARM_MEASUREMENT_H
#define FOREARM_MEASUREMENT_H

#include "dxa_measurement.h"

#include "dicom/dcm_recv.h"

class ForearmMeasurement : public DXAMeasurement
{
public:
    ForearmMeasurement(Side side);

    QString toString() const override;
    QStringList toStringList(const bool& no_keys = false) const override;

    bool isValid() const override;

    void addDicomFile(DicomFile);
    void simulate() override;

    bool isValidDicomFile(DicomFile file) const override;
    DicomFile m_forearmDicomFile{};
    void getScanData(const QSqlDatabase &db, const QString &patientKey, const QString &scanId) override;

    Side getSide() override;
    quint8 getScanType() override;
    QString getName() override;
    QString getBodyPartName() override;
    QString getRefType() override;
    QString getRefSource() override;

private:
    bool hasForearmFile { false };
    Side m_side;

    // DXAMeasurement interface
public:
    bool hasAllNeededFiles() const override;
};

#endif // FOREARM_MEASUREMENT_H

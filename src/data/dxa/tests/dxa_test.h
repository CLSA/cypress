#ifndef DXA_TEST_H
#define DXA_TEST_H

#include "data/dxa/measurements/ap_spine_measurement.h"
#include "data/dxa/measurements/forearm_measurement.h"
#include "data/dxa/measurements/iva_imaging_measurement.h"
#include "data/dxa/measurements/whole_body_measurement.h"
#include "data/test_base.h"

#include "dicom/dcm_recv.h"
#include "server/sessions/dxa/dxa_session.h"

#include "dcmtk/ofstd/ofstdinc.h"

class DXATest : public TestBase
{

public:
    DXATest();

    static const QMap<QString, QString> ranges;

    bool isValid() const override;
    void reset() override;

    bool hasAllNeededFiles() const;

    void fromDicomFiles(QList<DicomFile> files, const DXASession &session);

    void simulate() override;

    virtual QJsonObject toJsonObject() const override;
    virtual QString toString() const override;

    QScopedPointer<WholeBodyScanMeasurement> wholeBodyMeasurement;
    QScopedPointer<ForearmMeasurement> leftForearmMeasurement;
    QScopedPointer<ForearmMeasurement> rightForearmMeasurement;
    QScopedPointer<ApSpineMeasurement> apSpineMeasurement;
    QScopedPointer<IVAImagingMeasurement> ivaImagingMeasurement;

    void getPatientScan(const QSqlDatabase &db, const QString &participantId);
};

#endif // DXA_TEST_H

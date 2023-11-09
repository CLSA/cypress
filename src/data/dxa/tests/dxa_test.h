#ifndef DXA_TEST_H
#define DXA_TEST_H

#include "dicom/dcm_recv.h"
#include "../../test_base.h"

#include "../measurements/ap_spine_measurement.h"
#include "../measurements/forearm_measurement.h"
#include "../measurements/iva_imaging_measurement.h"
#include "../measurements/whole_body_measurement.h"

#include "dcmtk/ofstd/ofstdinc.h"

class DXATest : public TestBase
{

public:
    DXATest();

    static const QMap<QString, QString> ranges;

    bool isValid() const override;
    void reset() override;

    bool areDicomFilesValid() const;

    void fromDicomFiles(QList<DicomFile> files);

    void simulate() override;

    virtual QJsonObject toJsonObject() const override;
    virtual QString toString() const override;

    QScopedPointer<WholeBodyScanMeasurement> wholeBodyMeasurement;
    QScopedPointer<ForearmMeasurement> forearmMeasurement;
    QScopedPointer<ApSpineMeasurement> apSpineMeasurement;
    QScopedPointer<IVAImagingMeasurement> ivaImagingMeasurement;
};

#endif // DXA_TEST_H

#ifndef DXA_TEST_H
#define DXA_TEST_H

#include "data/test_base.h"
#include "dcmtk/ofstd/ofstdinc.h"

#include "../measurements/ap_spine_measurement.h"
#include "../measurements/forearm_measurement.h"
#include "../measurements/hip_measurement.h"
#include "../measurements/iva_imaging_measurement.h"
#include "../measurements/whole_body_measurement.h"

class DXATest : public TestBase
{
public slots:
    void onDicomDirectoryChange(const QString& path);

public:
    QScopedPointer<WholeBodyScanMeasurement> wholeBodyMeasurement;
    QScopedPointer<ForearmMeasurement> forarmMeasurement;
    QScopedPointer<HipMeasurement> hipMeasurement;
    QScopedPointer<ApSpineMeasurement> apSpineMeasurement;
    QScopedPointer<IVAImagingMeasurement> ivaImagingMeasurement;

    static const QMap<QString, QString> ranges;

    bool isValid() const override;
    void reset() override;

    virtual QJsonObject toJsonObject() const override;
    virtual QString toString() const override;
};

#endif // DXA_TEST_H

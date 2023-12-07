#ifndef DXAHIPTEST_H
#define DXAHIPTEST_H

#include "../../test_base.h"

#include "../measurements/hip_measurement.h"

#include "dcmtk/ofstd/ofstdinc.h"

class DxaHipTest: public TestBase
{
public:
    DxaHipTest();

    static const QMap<QString, QString> ranges;

    bool isValid() const override;
    void reset() override;

    void simulate() override;

    virtual QJsonObject toJsonObject() const override;
    virtual QString toString() const override;

    void fromDicomFiles(QList<DicomFile> files);

    QScopedPointer<HipMeasurement> hipMeasurement;
};

#endif // DXAHIPTEST_H

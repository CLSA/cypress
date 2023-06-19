#ifndef IVA_IMAGING_TEST_H
#define IVA_IMAGING_TEST_H

#include <QJsonObject>

#include "dxa_test.h"
#include "../measurements/iva_imaging_measurement.h"

class IVAImagingTest: public DXATest<IVAImagingMeasurement>
{
public:
    explicit IVAImagingTest();

    bool isValid() const override;
    bool isValidDicom(DcmFileFormat &loadedFileFormat) const override;
    void reset() override;

    Side getSide() override;
    quint8 getScanType() override;
    QString getName() override;
    QString getBodyPartName() override;
    QString getRefType() override;
    QString getRefSource() override;

    QString toString() const override;
    QJsonObject toJsonObject() const override;

private:
    const static QList<QString> m_outputKeyList;
    const static QList<QString> m_inputKeyList;
};

#endif // IVA_IMAGING_TEST_H

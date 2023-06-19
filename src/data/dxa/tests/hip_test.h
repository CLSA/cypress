#ifndef HIP_TEST_H
#define HIP_TEST_H

#include "./dxa_test.h"
#include "../measurements/hip_measurement.h"

class HipTest : public DXATest<HipMeasurement>
{
public:
    explicit HipTest();

    void reset() override;
    bool isValid() const override;

    bool isValidDicom(DcmFileFormat &loadedFileFormat) const override;

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

#endif // HIP_TEST_H

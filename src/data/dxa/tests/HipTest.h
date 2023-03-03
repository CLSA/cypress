#ifndef HIPTEST_H
#define HIPTEST_H

#include "./dxatest.h"
#include "../measurements/HipMeasurement.h"

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

#endif // HIPTEST_H

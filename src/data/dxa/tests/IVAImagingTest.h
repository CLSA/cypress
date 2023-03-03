#ifndef IVAIMAGINGTEST_H
#define IVAIMAGINGTEST_H

#include <QJsonObject>

#include "dxatest.h"
#include "../measurements/IVAImagingMeasurement.h"

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

#endif // IVAIMAGINGTEST_H

#ifndef FOREARMTEST_H
#define FOREARMTEST_H

#include <QJsonObject>

#include "./dxatest.h"
#include "../measurements/ForearmMeasurement.h"

class ForearmTest : public DXATest<ForearmMeasurement>
{
public:
    explicit ForearmTest();

    QString toString() const override;
    QJsonObject toJsonObject() const override;


    bool isValid() const override;
    bool isValidDicom(DcmFileFormat &loadedFileFormat) const override;
    void reset() override;

    Side getSide() override;
    quint8 getScanType() override;
    QString getName() override;
    QString getBodyPartName() override;
    QString getRefType() override;
    QString getRefSource() override;

private:
    const static QList<QString> m_outputKeyList;
    const static QList<QString> m_inputKeyList;
};

#endif // FOREARMTEST_H

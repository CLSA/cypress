#ifndef FOREARM_TEST_H
#define FOREARM_TEST_H

#include <QJsonObject>

#include "./dxa_test.h"
#include "../measurements/forearm_measurement.h"

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

#endif // FOREARM_TEST_H

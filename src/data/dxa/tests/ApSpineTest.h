#ifndef APSPINETEST_H
#define APSPINETEST_H

#include <QJsonObject>
#include "dxatest.h"
#include "../measurements/ApSpineMeasurement.h"

class ApSpineTest : public DXATest<ApSpineMeasurement>
{
public:
    explicit ApSpineTest();

    bool isValid() const override;
    bool isValidDicom(DcmFileFormat &loadedFileFormat) const override;
    void reset() override;

    Side getSide() override;
    quint8 getScanType() override;
    QString getName() override;
    QString getBodyPartName() override;
    QString getRefType() override;
    QString getRefSource() override;

    QJsonObject toJsonObject() const override;
    QString toString() const override;

private:
    const static QList<QString> m_outputKeyList;
    const static QList<QString> m_inputKeyList;

};

#endif // APSPINETEST_H

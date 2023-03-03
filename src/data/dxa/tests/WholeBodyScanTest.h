#ifndef WHOLEBODYSCANTEST_H
#define WHOLEBODYSCANTEST_H

#include <QString>
#include <QList>

#include "./dxatest.h"
#include "../measurements/WholeBodyScanMeasurement.h"


class WholeBodyScanTest: public DXATest<WholeBodyScanMeasurement>
{
public:
    explicit WholeBodyScanTest();

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
    static const QList<QString> m_inputKeyList;
    static const QList<QString> m_outputKeyList;
};

#endif // WHOLEBODYSCANTEST_H

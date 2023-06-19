#ifndef WHOLE_BODY_TEST_H
#define WHOLE_BODY_TEST_H

#include <QString>
#include <QList>

#include "./dxa_test.h"
#include "../measurements/whole_body_measurement.h"


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

#endif // WHOLE_BODY_TEST_H

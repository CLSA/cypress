#ifndef DXATEST_H
#define DXATEST_H

#include "dcmtk/dcmdata/dcfilefo.h"
#include "data/TestBase.h"

enum Side {
    LEFT,
    RIGHT,
    BOTH
};

template <class DxaMeasurement>
class DXATest: public TestBase<DxaMeasurement>
{
public:
    static const QMap<QString, QString> ranges;

    virtual bool isValid() const = 0;
    virtual bool isValidDicom(DcmFileFormat &loadedFileFormat) const = 0;
    virtual void reset() = 0;

    virtual Side getSide() = 0;
    virtual quint8 getScanType() = 0;
    virtual QString getName() = 0;
    virtual QString getBodyPartName() = 0;
    virtual QString getRefType() = 0;
    virtual QString getRefSource() = 0;

    virtual QJsonObject toJsonObject() const = 0;
    virtual QString toString() const = 0;
};

#endif // DXATEST_H

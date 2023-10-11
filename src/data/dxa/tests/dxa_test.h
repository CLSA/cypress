#ifndef DXA_TEST_H
#define DXA_TEST_H

#include "data/test_base.h"
#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/ofstd/ofstdinc.h"

enum Side {
    LEFT,
    RIGHT,
    BOTH
};

class DXATest : public TestBase
{
public slots:
    void onDicomDirectoryChange(const QString& path);

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

#endif // DXA_TEST_H

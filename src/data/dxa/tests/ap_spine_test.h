#ifndef AP_SPINE_TEST_H
#define AP_SPINE_TEST_H

#include <QJsonObject>

#include "data/test_base.h"

#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmdata/dcmetinf.h"
#include "dcmtk/dcmdata/dcuid.h"

class ApSpineTest : public TestBase
{
public:
    explicit ApSpineTest();

    bool isValid() const override;

    bool isValidDicom(DcmFileFormat &loadedFileFormat) const;

    void reset() override;

    QJsonObject toJsonObject() const override;
    QString toString() const override;

private:
    const static QList<QString> m_outputKeyList;
    const static QList<QString> m_inputKeyList;

};

#endif // AP_SPINE_TEST_H

#ifndef IVA_IMAGING_TEST_H
#define IVA_IMAGING_TEST_H

#include <QJsonObject>

#include "data/test_base.h"

class IVAImagingTest : public TestBase
{
public:
    explicit IVAImagingTest();

    bool isValid() const override;
    bool isValidDicom(DcmFileFormat &loadedFileFormat) const override;
    void reset() override;

private:
    const static QList<QString> m_outputKeyList;
    const static QList<QString> m_inputKeyList;
};

#endif // IVA_IMAGING_TEST_H

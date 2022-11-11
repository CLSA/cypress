#ifndef IVAIMAGINGTEST_H
#define IVAIMAGINGTEST_H

#include <QJsonObject>

#include "managers/dxa/DXAManager.h"
#include "data/dxa/measurements/IVAImagingMeasurement.h"
#include "data/TestBase.h"

class IVAImagingTest : public TestBase<IVAImagingMeasurement>
{
public:
    explicit IVAImagingTest();

    Side side = LEFT;

    // String representation for debug and GUI display purposes
    //
    QString toString() const override;

    // derived classes should at minimum check
    // - number of measurements = expected number of measurements
    // - meta data received matches on expected keys
    // - attributes received matched on expected keys
    //
    bool isValid() const override;

    // String keys are converted to snake_case
    //
    QJsonObject toJsonObject() const override;

    // default is to reset both meta and measurement data
    //
    void reset() override;

private:
    const static QList<QString> m_outputKeyList;
    const static QList<QString> m_inputKeyList;
};
#endif // IVAIMAGINGTEST_H

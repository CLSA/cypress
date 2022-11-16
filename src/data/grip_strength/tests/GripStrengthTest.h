#ifndef GRIPSTRENGTHTEST_H
#define GRIPSTRENGTHTEST_H

#include "data/TestBase.h"
#include "data/grip_strength/measurements/GripStrengthMeasurement.h"

class GripStrengthTest : public TestBase<GripStrengthMeasurement>
{
public:
    GripStrengthTest();
    ~GripStrengthTest() = default;

    static const q_stringMap testMetaMap;

    // String representation for debug and GUI display purposes
    //
    QString toString() const override;

    bool isValid() const override;

    // String keys are converted to snake_case
    //
    void readGripTestOptions(const QString& gripTestPath);
    void readGripTestResults(const QString& gripTestDataPath);

    QJsonObject toJsonObject() const override;

private:
    QStringList m_outputKeyList;
};

Q_DECLARE_METATYPE(GripStrengthTest);

#endif // GRIPSTRENGTHTEST_H


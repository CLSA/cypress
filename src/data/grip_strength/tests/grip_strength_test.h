#ifndef GRIP_STRENGTH_TEST_H
#define GRIP_STRENGTH_TEST_H

#include <QDir>

#include "../../TestBase.h"
#include "../measurements/grip_strength_measurement.h"

class GripStrengthTest : public TestBase<GripStrengthMeasurement>
{
public:
    GripStrengthTest();
    ~GripStrengthTest() = default;

    static const q_stringMap testMetaMap;

    QString toString() const override;

    bool isValid() const override;

    void openDatabase();

    bool readMeasurements();


    QString getTrackerDatabaseFolder();
    QJsonObject toJsonObject() const override;

private:
    QStringList m_outputKeyList;


};

Q_DECLARE_METATYPE(GripStrengthTest);

#endif // GRIP_STRENGTH_TEST_H


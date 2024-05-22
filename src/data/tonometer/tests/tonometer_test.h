#ifndef TONOMETER_TEST_H
#define TONOMETER_TEST_H

#include "data/test_base.h"
#include "data/tonometer/measurements/tonometer_measurement.h"

QT_FORWARD_DECLARE_CLASS(QJsonArray)

class TonometerTest : public TestBase
{
public:
    TonometerTest();
    ~TonometerTest() = default;

    void fromVariantMapList(const QList<QVariantMap> &);

    // String representation for debug purposes
    //
    QString toString() const override;

    // Get the measurements by side for UI display
    //
    QStringList getMeasurementStrings(const QString&) const;

    bool isValid() const override;

    // String keys are converted to snake_case
    //
    QJsonObject toJsonObject() const override;

    static const q_stringMap metaLUT;
    static q_stringMap initMetaLUT();

private:
    QStringList m_outputKeyList;
};

Q_DECLARE_METATYPE(TonometerTest);

#endif // TONOMETER_TEST_H

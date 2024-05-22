#ifndef BODY_COMPOSITION_TEST_H
#define BODY_COMPOSITION_TEST_H

#include "data/test_base.h"

class BodyCompositionTest : public TestBase
{
public:
    BodyCompositionTest();
    ~BodyCompositionTest() = default;

    void fromArray(const QByteArray &);

    //void simulate(const double&, const QString&, double);

    // String representation for debug and GUI display purposes
    //
    QString toString() const override;

    bool isValid() const override;

    // String keys are converted to snake_case
    //
    QJsonObject toJsonObject() const override;

private:
    QList<QString> m_outputKeyList;
    QByteArray m_array;
    QString readArray(const quint8 &, const quint8 &) const;

    QString  readBodyType() const;
    QString  readGender() const;
    QVariant readHeight() const;
    QVariant readWeight() const;
    QVariant readImpedance() const;
    QVariant readFatPercent() const;
    QVariant readFatMass() const;
    QVariant readFatFreeMass() const;
    QVariant readTotalBodyWater() const;
    QVariant readAge() const;
    QVariant readBMI() const;
    QVariant readBMR() const;
};

Q_DECLARE_METATYPE(BodyCompositionTest);

#endif // BODY_COMPOSITION_TEST_H

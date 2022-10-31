#ifndef WHOLEBODYSCANTEST_H
#define WHOLEBODYSCANTEST_H

#include "managers/dxa/DXAManager.h"
#include "data/TestBase.h"
#include "data/dxa/measurements/WholeBodyScanMeasurement.h"

#include <QString>
#include <QList>




class WholeBodyScanTest : public TestBase<WholeBodyScanMeasurement>
{
public:
    explicit WholeBodyScanTest();
      // TODO: implement validation for tests that capture participant ID meta data
    // against an interview barcode
    //

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
    static const QList<QString> m_inputKeyList;
    static const QList<QString> m_outputKeyList;
};

#endif // WHOLEBODYSCANTEST_H

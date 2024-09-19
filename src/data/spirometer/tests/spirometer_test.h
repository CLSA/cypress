#ifndef SPIROMETER_TEST_H
#define SPIROMETER_TEST_H

#include "data/test_base.h"
#include "data/spirometer/measurements/spirometer_measurement.h"

#include <QJsonObject>

QT_FORWARD_DECLARE_CLASS(QDomNode)

class SpirometerTest : public TestBase
{

public:
    SpirometerTest();
    ~SpirometerTest() = default;

    void fromFile(const QString&);

    // String representation for debug and GUI display purposes
    //
    QString toString() const override;

    bool isValid() const override;

    // String keys are converted to snake_case
    //
    QJsonObject toJsonObject() const override;

    static const q_stringMap testMetaMap;
    static const q_stringMap patientMetaMap;

    std::unique_ptr<QJsonObject> toJsonObjectHeap();

protected:
    QStringList m_outputKeyList;

    virtual void readPDFReportPath(const QDomNode&);
    virtual void readPatients(const QDomNode&);
    virtual void readTrials(const QDomNode&);
    virtual void readParameters(const QDomNode&, SpirometerMeasurement*);
    virtual void readChannel(const QDomNode&, SpirometerMeasurement*);
    virtual void readBestValues(const QDomNode&);
};

Q_DECLARE_METATYPE(SpirometerTest);

#endif // SPIROMETER_TEST_H


#ifndef SPIROMETER_TEST_H
#define SPIROMETER_TEST_H

#include "../../test_base.h"
#include "../measurements/spirometer_measurement.h"

#include <QJsonObject>

QT_FORWARD_DECLARE_CLASS(QDomNode)

class SpirometerTest : public TestBase
{

public:
    SpirometerTest();
    ~SpirometerTest() = default;

    void fromFile(const QString&);

    void simulate(const QVariantMap &inputData) override;

    // String representation for debug and GUI display purposes
    //
    QString toString() const override;

    QList<QStringList> toStringListList();

    bool isValid() const override;

    // String keys are converted to snake_case
    //
    QJsonObject toJsonObject() const override;

    static const q_stringMap testMetaMap;
    static const q_stringMap patientMetaMap;

private:
    QStringList m_outputKeyList;

    void readPDFReportPath(const QDomNode&);
    void readPatients(const QDomNode&);
    void readTrials(const QDomNode&);
    void readParameters(const QDomNode&, SpirometerMeasurement*);
    void readChannel(const QDomNode&, SpirometerMeasurement*);
    void readBestValues(const QDomNode&);
};

Q_DECLARE_METATYPE(SpirometerTest);

#endif // SPIROMETER_TEST_H


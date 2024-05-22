#ifndef TESTBASE_H
#define TESTBASE_H

#include "auxiliary/Constants.h"
#include "measurement.h"

#include <QException>

/*!
 * \class TestBase
 * \brief An abstract Test class
 *
 * Tests are comprised of instrument characteristics, or meta data,
 * and measurements.  Template specialization is done on the
 * measurement class parameter type, such as a HearingMeasurement.
 *
 * Inherited classes must implement the pure virtual methods:
 * toString, isValid, toJsonObject
 *
 * \sa Measurement, AudiometerTest, QList
 *
 */

QT_FORWARD_DECLARE_CLASS(QJsonObject)

class TestBase
{
public:
    TestBase() = default;
    virtual ~TestBase() = default;

    // When a fixed maximum number of measurements is
    // established, the newest elements are kept and
    // the older ones are removed from the list
    //

    // TODO: implement validation for tests that capture participant ID meta data
    // against an interview barcode
    //
    virtual QString toString() const = 0;

    // derived classes should at minimum check
    // - number of measurements = expected number of measurements
    // - meta data received matches on expected keys
    // - attributes received matched on expected keys
    //
    virtual bool isValid() const = 0;

    virtual QJsonObject toJsonObject() const = 0;

public:
    virtual void reset();
    virtual void reinterpret();

public:
    void addMetaData(const QString &key, const QVariant &value, const QString &units);
    void addMetaData(const QString &key, const QVariant &value, const int &precision);
    void addMetaData(const QString &key, const QVariant &value);
    void addMetaData(const QString &key, const MetaData::Value &value);

    bool metaDataIsEmpty() const;

    const MetaData getMetaData() const;

    QVariant getMetaData(const QString &key) const;

    QString getMetaDataAsString(const QString &key) const;

    bool hasMetaData(const QString &key) const;

public:
    bool hasMeasurement(const int &) const;

    void addMeasurement(QSharedPointer<Measurement> measurement);

    Measurement &getMeasurement(const int &) const;
    Measurement &get(int index);
    const QVector<QSharedPointer<Measurement>> &getMeasurements() const;

    Measurement &lastMeasurement() const;

    void removeMeasurement(int index);

    Constants::UnitsSystem getUnitsSystem() const;
    void setUnitsSystem(const Constants::UnitsSystem &system);

    int getMeasurementCount() const;
    int getMinimumMeasurementCount() const;
    void setMinimumMeasurementCount(const int &min);
    int getExpectedMeasurementCount() const;
    void setExpectedMeasurementCount(const int &num);

    bool getManualEntryMode() const;
    void setManualEntryMode(bool isManualEntry);

protected:
    QVector<QSharedPointer<Measurement>> m_measurementList;

    QList<QVariant> m_files;

    MetaData m_metaData;

    bool m_debug;

    bool isManualEntryMode{false};

    int m_expectedMeasurementCount { 1 };
    int m_minimumMeasurementCount { 1 };

    Constants::UnitsSystem m_unitsSystem { Constants::UnitsSystem::systemMetric };
};

#endif // TESTBASE_H

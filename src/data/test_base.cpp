#include "test_base.h"
#include "cypress_settings.h"

void TestBase::reset()
{
    m_metaData.reset();
    m_measurementList.clear();
    m_debug = CypressSettings::isDebugMode();
    m_sim = CypressSettings::isSimMode();

    setManualEntryMode(false);
}

void TestBase::simulate() {}

void TestBase::simulate(const QVariantMap &inputData) {
    Q_UNUSED(inputData)
}

void TestBase::setUnitsSystem(const Constants::UnitsSystem &system)
{
    m_unitsSystem = system;
}

Constants::UnitsSystem TestBase::getUnitsSystem() const
{
    return m_unitsSystem;
}

void TestBase::addMetaData(const QString &key, const QVariant &value, const QString &units)
{
    m_metaData.setAttribute(key, value, units);
}

void TestBase::addMetaData(const QString &key, const QVariant &value, const int &precision)
{
    m_metaData.setAttribute(key, value, QString(), precision);
}

void TestBase::addMetaData(const QString &key, const QVariant &value)
{
    m_metaData.setAttribute(key, value);
}

void TestBase::addMetaData(const QString &key, const Measurement::Value &value)
{
    m_metaData.setAttribute(key, value);
}

const MetaData TestBase::getMetaData() const
{
    return m_metaData;
}

QVariant TestBase::getMetaData(const QString &key) const
{
    return m_metaData.getAttributeValue(key);
}

QString TestBase::getMetaDataAsString(const QString &key) const
{
    return m_metaData.getAttribute(key).toString();
}

bool TestBase::hasMetaData(const QString &key) const
{
    return m_metaData.hasAttribute(key);
}

// FIFO addition of measurements
//
void TestBase::addMeasurement(QSharedPointer<Measurement> item)
{
    m_measurementList.append(item);
    if (m_expectedMeasurementCount < m_measurementList.size()) {
        m_measurementList.pop_front();
    }
}

void TestBase::removeMeasurement(int index)
{
    if (index < 0 || index >= m_measurementList.size()) {
        qCritical() << "index out of bounds at TestBase";
        throw QException();
    }

    m_measurementList.removeAt(index);
}

Measurement &TestBase::getMeasurement(const int &index) const
{
    if (index < 0 || index >= m_measurementList.size()) {
        throw QException();
    }

    if (!m_measurementList[index]) {
        throw QException();
    }

    return *m_measurementList.at(index).get();
}

const QVector<QSharedPointer<Measurement> > &TestBase::getMeasurements() const
{
    return m_measurementList;
}

int TestBase::getMeasurementCount() const
{
    return m_measurementList.size();
}

Measurement &TestBase::get(int index)
{
    if (index >= m_measurementList.size()) {
        qCritical() << "index out of bounds at TestBase[]";
        throw QException();
    }

    return *m_measurementList[index];
}

void TestBase::setExpectedMeasurementCount(const int &num)
{
    m_expectedMeasurementCount = 0 < num ? num : 1;
}

int TestBase::getExpectedMeasurementCount() const
{
    return m_expectedMeasurementCount;
}

void TestBase::setMinimumMeasurementCount(const int &min)
{
    m_minimumMeasurementCount = 0 < min ? min : 1;
}

int TestBase::getMinimumMeasurementCount() const
{
    return m_minimumMeasurementCount;
}

bool TestBase::hasMeasurement(const int &index) const
{
    const Measurement &m = getMeasurement(index);
    return !m.isEmpty();
}

Measurement &TestBase::lastMeasurement() const
{
    if (m_measurementList.isEmpty()) {
        throw QException();
    }

    if (!m_measurementList.last()) {
        throw QException();
    }

    return *m_measurementList.last();
}

bool TestBase::metaDataIsEmpty() const
{
    return m_metaData.isEmpty();
}

bool TestBase::getManualEntryMode() const
{
    return isManualEntryMode;
}

void TestBase::setManualEntryMode(bool isManualEntry)
{
    isManualEntryMode = isManualEntry;
}

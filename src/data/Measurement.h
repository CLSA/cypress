#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#include <QMap>
#include <QVariant>
#include <QDebug>

/*!
 * \class Measurement
 * \brief A Base Measurement class
 *
 * Measurements are composed of atomic elements or "attributes".
 * This class facilitates parsing measurement data from different
 * instruments into component characteristics, such as value, units etc.
 * and stores them labeled by QString keys as class Value objects in a QMap.
 * The measurement Value objects can be retrieved as a QString for UI purposes.
 *
 * \note  This class stores one or more attributes which taken as whole
 * can represent a single measurement.  A Test class inherited from TestBase
 * should be used to store multiple measurements.
 *
 * Child classes should override toString and isValid methods as appropriate.
 *
 * This class can also be used to store instrument attributes such as
 * serial number, model number etc. or any unitless data.
 *
 * \sa QVariant, QString, QMap
 *
 */

class Measurement
{

public:
    Measurement();

    virtual ~Measurement() = default;

    Measurement(const Measurement &other);

    Measurement(Measurement &other);

    Measurement(const Measurement &&other);

    Measurement(Measurement &&other);

    Measurement &operator=(const Measurement &other);

    bool operator==(const Measurement &rhs) const;

    bool operator!=(const Measurement &rhs);

    class Value
    {
      public:
        Value() = default;
        Value(const QVariant& value, const QString& units = QString(), const quint16& precision = 0);
        Value(const Value&);
        Value(const QVariant& value, const quint16& precision);
        Value &operator=(const Value&);
        ~Value() = default;

        bool operator==(const Measurement::Value &rhs) const;

        bool operator!=(const Measurement::Value &rhs) const;

        bool hasUnits() const;
        bool isNull() const;
        QString toString() const;

        QVariant value() const;
        QString units() const;
        quint16 precision() const;

    private:
        QVariant m_value;
        QString m_units;

        // display digits of precision for double and float values
        //
        quint16 m_precision { 0 };
    };

    // String representation for debug and GUI display purposes
    //
    virtual QString toString() const;

    virtual QStringList toStringList(const bool& no_keys = false) const;

    virtual bool isValid() const;

    // String keys are converted to snake_case
    //
    virtual QJsonObject toJsonObject() const;

    void reset();

    void remove(const QStringList &list);

    void removeAttribute(const QString &key);

    void setAttribute(const QString &key,
                      const QVariant &value,
                      const QString &units,
                      const quint16 &precision = 0);

    void setAttribute(const QString &key, const QVariant &value);

    void setAttribute(const QString &key, const QVariant &value, const quint16 &precision);

    void setAttribute(const QString &key, const Value &value);

    Value getAttribute(const QString &key) const;

    QVariant getAttributeValue(const QString &key) const;

    const QMap<QString, Value> getAttributes() const;

    const QList<QString> getAttributeKeys() const;

    bool hasAttribute(const QString &key) const;

    bool hasUnits(const QString &key) const;

    int size() const;

    bool isEmpty() const;

    // precision should be set before reading any values from the output.
    // derived classes can set precision as needed per data variable
    // ie., setting precision does not apply to all attribute values
    //
    void setPrecision(const int &precision);

    int getPrecision() const;

protected:
    QMap<QString,Value> m_attributes;

    // display precision, actual storage precision is data intrinsic
    //
    int m_precision = { 4 };
};

QDebug operator<<(QDebug dbg, const Measurement &);

Q_DECLARE_METATYPE(Measurement);
Q_DECLARE_METATYPE(Measurement::Value)

typedef Measurement MetaData;

#endif // MEASUREMENT_H

#include "measurement.h"
#include "../auxiliary/Constants.h"

#include <QDebug>
#include <QJsonObject>

bool Measurement::isValid() const
{
    if(isEmpty())
    {
      return false;
    }
    bool ok = true;
    foreach(const auto x, m_attributes.toStdMap())
    {
      if(x.second.isNull())
      {
        ok = false;
        break;
      }
    }
    return ok;
}

void Measurement::reset()
{
    m_attributes.clear();
}

void Measurement::remove(const QStringList &list)
{
    foreach (const auto key, list)
      removeAttribute(key);
}

void Measurement::removeAttribute(const QString &key)
{
    m_attributes.remove(key);
}

void Measurement::setAttribute(const QString &key,
                               const QVariant &value,
                               const QString &units,
                               const quint16 &precision)
{
    setAttribute(key, Measurement::Value(value, units, precision));
}

void Measurement::setAttribute(const QString &key, const QVariant &value)
{
    setAttribute(key, Measurement::Value(value));
}

void Measurement::setAttribute(const QString &key, const QVariant &value, const quint16 &precision)
{
    setAttribute(key, Measurement::Value(value, precision));
}

void Measurement::setAttribute(const QString &key, const Value &value)
{
    m_attributes[key] = value;
}

Measurement::Value Measurement::getAttribute(const QString &key) const
{
    return m_attributes.contains(key) ? m_attributes[key] : Value();
}

QVariant Measurement::getAttributeValue(const QString &key) const
{
    return m_attributes.contains(key) ? m_attributes[key].value() : QVariant();
}

const QMap<QString, Measurement::Value> Measurement::getAttributes() const
{
    return m_attributes;
}

const QList<QString> Measurement::getAttributeKeys() const
{
    return m_attributes.keys();
}

bool Measurement::hasAttribute(const QString &key) const
{
    return m_attributes.contains(key) && !m_attributes[key].isNull();
}

bool Measurement::hasUnits(const QString &key) const
{
    return (hasAttribute(key) && m_attributes[key].hasUnits());
}

int Measurement::size() const
{
    return m_attributes.size();
}

bool Measurement::isEmpty() const
{
    return m_attributes.isEmpty();
}

void Measurement::setPrecision(const int &precision)
{
    if (0 < precision)
      m_precision = precision;
}

int Measurement::getPrecision() const
{
    return m_precision;
}

QStringList Measurement::toStringList(const bool& no_keys) const
{
    QStringList list;
    foreach(const auto x, m_attributes.toStdMap())
    {
      QString key = x.first; // the key
      Measurement::Value value = x.second; // the value
      list << (no_keys ? value.toString() : QString("%1: %2").arg(key,value.toString()));
    }
    return list;
}

Measurement::Measurement() {}

Measurement::Measurement(const Measurement &other)
{
    m_attributes = other.m_attributes;
}

Measurement::Measurement(Measurement &other)
{
    m_attributes = other.m_attributes;
}

Measurement::Measurement(const Measurement &&other)
{
    m_attributes = other.m_attributes;
}

Measurement::Measurement(Measurement &&other)
{
    m_attributes = other.m_attributes;
}

Measurement &Measurement::operator=(const Measurement &other)
{
    if (this != &other) {
      m_attributes = other.m_attributes;
    }
    return *this;
}

bool Measurement::operator==(const Measurement &rhs) const
{
    return (this->getAttributes() == rhs.getAttributes());
}

bool Measurement::operator!=(const Measurement &rhs)
{
    return !(*this == rhs);
}

QString Measurement::toString() const
{
    return toStringList().join(" ");
}

QJsonObject Measurement::toJsonObject() const
{
    QJsonObject json;
    foreach(const auto x, m_attributes.toStdMap())
    {
      // convert to space delimited phrases to snake_case
      //
      QString key = QString(x.first).toLower().replace(QRegExp("[\\s]+"),"_");
      Value v = x.second;
      QJsonValue jval = QJsonValue::fromVariant(v.value());
      if(v.hasUnits())
      {
          QJsonObject obj;
          obj.insert("value",jval);
          obj.insert("units",v.units());
          json.insert(key,obj);
      }
      else
        json.insert(key,jval);
    }
    return json;
}

QDebug operator<<(QDebug dbg, const Measurement &item)
{
    const QString s = item.toString();
    if(s.isEmpty())
      dbg.nospace() << "Measurement()";
    else
      dbg.nospace() << "Measurement(" << s << " ...)";
    return dbg.maybeSpace();
}

Measurement::Value::Value(const QVariant& value, const QString& units, const quint16& precision)
{
    m_value = value;
    if(!units.isEmpty() && 0 < units.length())
      m_units = units;
    m_precision = precision;
}

Measurement::Value::Value(const QVariant& value, const quint16& precision)
{
    m_value = value;
    m_precision = precision;
}

Measurement::Value::Value(const Measurement::Value& other)
{
    m_value = other.m_value;
    m_units = other.m_units;
    m_precision = other.m_precision;
}

Measurement::Value& Measurement::Value::operator=(const Value& other)
{
    if(*this != other)
    {
      m_value = other.m_value;
      m_units = other.m_units;
      m_precision = other.m_precision;
    }
    return *this;
}

bool Measurement::Value::operator==(const Value &rhs) const
{
    return (this->value() == rhs.value() && this->units() == rhs.units()
            && this->precision() == rhs.precision());
}

bool Measurement::Value::operator!=(const Value &rhs) const
{
    return !(*this == rhs);
}

bool Measurement::Value::hasUnits() const
{
    return (!m_units.isEmpty() && 0 < m_units.length());
}

bool Measurement::Value::isNull() const
{
    return m_value.isNull();
}

QString Measurement::Value::toString() const
{
    QString str;
    if(m_value.type() == QVariant::Type::DateTime)
    {
      str = m_value.toDateTime().toString("yyyy-MM-dd HH:mm:ss");
    }
    else
    {
      if(QVariant::Double == m_value.type() && 0 < m_precision)
      {
        str = QString::number(m_value.toDouble(),'f',m_precision);
      }
      else
      {
        if(m_value.canConvert<QString>())
          str = m_value.toString();
        else if(m_value.canConvert<QStringList>())
        {
          str = m_value.toStringList().join(",");
        }
        else
        {
          qCritical() << "value conversion to string may fail with type" << m_value.typeName();
        }
        if(str.contains(","))
        {
          QStringList values = str.split(",");
          if(Constants::DefaultSplitLength <= values.size())
          {
            QStringList list;
            for(int i=0;i<Constants::DefaultSplitLength;i++)
                list.push_back(values.at(i));

            str = list.join(",") + "...";
          }
          else
          {
            str = values.join(",");
          }
        }
      }
    }
    if(this->hasUnits())
    {
      return QString("%1 (%2)").arg(str,m_units);
    }
    else
    {
      return str;
    }
}

QVariant Measurement::Value::value() const
{
    return m_value;
}

QString Measurement::Value::units() const
{
    return m_units;
}

quint16 Measurement::Value::precision() const
{
    return m_precision;
}

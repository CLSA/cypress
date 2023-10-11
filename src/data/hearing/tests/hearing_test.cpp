#include "hearing_test.h"

#include <QDateTime>
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QStringBuilder>
#include <QRandomGenerator>

HearingTest::HearingTest()
{
  m_outputKeyList << "patient_id";
  m_outputKeyList << "test_datetime";
  m_outputKeyList << "last_calibration_date";
  m_outputKeyList << "test_id";
}

bool HearingTest::isValid() const
{
  bool okMeta = true;
  foreach (const auto key, m_outputKeyList) {
      if (!hasMetaData(key)) {
          okMeta = false;
          break;
      }
    }
    bool okTest = getMeasurementCount() == getExpectedMeasurementCount();
    if (okTest) {
        foreach (const auto m, m_measurementList) {
            if (!m->isValid()) {
                okTest = false;
                break;
            }
        }
    }
    return okMeta && okTest;
}

bool HearingTest::isPartial() const
{
    bool okTest = getMeasurementCount() >= getMinimumMeasurementCount();
    if(okTest)
    {
      foreach(const auto m, m_measurementList)
      {
        if (!m->isValid()) {
          okTest = false;
          break;
        }
      }
    }
    return okTest;
}

void HearingTest::simulate(const QVariantMap& inputData)
{
    addMetaData("patient_id", inputData["barcode"].toString());
    addMetaData("test_datetime", QDateTime::currentDateTimeUtc());
    addMetaData("last_calibration_date", QDateTime::currentDateTimeUtc());
    addMetaData("test_id", 1);

    QStringList sides = {"left", "right"};
    QMap<int, QString> frequencyTests = HearingMeasurement::initFrequencyLookup();
    QMap<int, QString>::const_iterator i;

    foreach (auto side, sides) {
      for (i = frequencyTests.constBegin(); i != frequencyTests.constEnd(); ++i)
      {
        QSharedPointer<HearingMeasurement> measurement(new HearingMeasurement);
        measurement->setAttribute("side", side);
        measurement->setAttribute("test", i.value());
        measurement->setAttribute("level", QRandomGenerator::global()->bounded(1, 101));
        qDebug() << "addMeasurement";
        addMeasurement(measurement);
      }
    }
}

QString HearingTest::toString() const
{
    QString str;
    if(isValid())
    {
        QStringList list;
        list << QString("patient id: ") % getMetaData("patient_id").toString();
        list << QString("test id: ") % getMetaData("test_id").toString();
        list << QString("test datetime: ") % getMetaData("test_datetime").toDateTime().toString("yyyy-MM-dd hh:mm:ss");
        list << QString("last calibration date: ") % getMetaData("last_calibration_date").toDate().toString("yyyy-MM-dd");
        foreach (const auto m, m_measurementList) {
        list << m->toString();
        }
        str = list.join("\n");
    }
    return str;
}

QString HearingTest::readArray(const quint8 &begin, const quint8 &end) const
{
    int len = end - begin + 1;
    return (0 < len && end < m_array.size() ?
            QString::fromLatin1(m_array.mid(begin, len)) :
            QString());
}

// The AudiometerManager class provides the data after validating
// it via hasEndCode(arr) before passing to this class
//
void HearingTest::fromArray(const QByteArray &arr)
{
    if(!arr.isEmpty())
    {
        reset();
        m_array = arr;

        addMetaData("patient_id",readPatientID());
        addMetaData("test_id",readTestID());
        addMetaData("test_datetime",readTestDateTime());
        addMetaData("last_calibration_date",readCalibrationDate());

        QStringList sides = {"left", "right"};

        foreach (auto side, sides) {
        foreach (auto m, readHearingThresholdLevels(side)) {
          QSharedPointer<HearingMeasurement> measure(new HearingMeasurement(m));
          addMeasurement(measure);
        }
        }
    }
}

QString HearingTest::readPatientID() const
{
    return readArray(4,17).trimmed();
}

QString HearingTest::readTestID() const
{
    return readArray(18,34).trimmed();
}

QDateTime HearingTest::readTestDateTime() const
{
    QString d_str = readArray(35,50).trimmed();
    if(d_str.contains("A"))
        d_str.replace("A","1");
    return QDateTime::fromString(d_str,"MM/dd/yyHH:mm:ss").addYears(100);
}

QDate HearingTest::readCalibrationDate() const
{
    QString d_str = readArray(51,58).trimmed();
    return QDate::fromString(d_str,"MM/dd/yy").addYears(100);
}

QString HearingTest::readExaminerID() const
{
    return readArray(59,74).trimmed();
}

QList<HearingMeasurement> HearingTest::readHearingThresholdLevels(const QString& side) const
{
  QList<HearingMeasurement> htl;
  QString str = ("left" == side.toLower()) ?
    readArray(75,106).trimmed() :
    readArray(107,138).trimmed();

  QStringList list = str.split(QRegExp("\\s+")).replaceInStrings(QRegExp("^\\s+|\\s+$"),"");
  int index = 0;
  foreach(const auto code, list)
  {
    HearingMeasurement m(side, index++, code);
    htl.append(m);
  }

  return htl;
}

const HearingMeasurement &HearingTest::getMeasurement(const QString &side, const int &index) const
{
  for (int i = 0; i < m_measurementList.size(); i++) {
    const HearingMeasurement *measure = static_cast<HearingMeasurement *>(
        m_measurementList[i].get());

    if (side == measure->getAttributeValue("side").toString()
        && HearingMeasurement::frequencyLookup[index]
               == measure->getAttributeValue("test").toString()) {
        return *measure;
    }
  }

  throw QException();
}

QJsonObject HearingTest::toJsonObject() const
{
  QJsonObject value{};
  QJsonArray jsonArr{};

  foreach (const auto m, m_measurementList) {
    jsonArr.append(m->toJsonObject());
  }

  QJsonObject json{};
  value.insert("metadata", m_metaData.toJsonObject());
  value.insert("results", jsonArr);
  value.insert("manual_entry", getManualEntryMode());

  json.insert("value", value);

  return json;
}

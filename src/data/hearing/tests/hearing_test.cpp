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

// Meta data

// PatientID bytes 4-17
// TestID bytes 18-34
// TestDatetime 35-50
// CalibrationDate
const static int PATIENT_ID_INDEX_START = 4;
const static int PATIENT_ID_INDEX_END = 17;

const static int TEST_ID_INDEX_START = 18;
const static int TEST_ID_INDEX_END = 34;

const static int TEST_DATETIME_INDEX_START = 35;
const static int TEST_DATETIME_INDEX_END = 50;

const static int CALIBRATION_DATE_INDEX_START = 51;
const static int CALIBRATION_DATE_INDEX_END = 58;

const static int EXAMINER_ID_START = 59;
const static int EXAMINER_ID_END = 74;

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

        double level = QRandomGenerator::global()->bounded(1, 101);

        measurement->setAttribute("level", level);
        measurement->setAttribute("pass", level < 40);

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
    qDebug() << "From Array: " << arr;
    // assumes all data before end code (5 bytes) is valid
    if (arr.isEmpty()) {
        qDebug() << "HearingTest::fromArray is empty.";
        return;
    }

    reset();

    m_array = arr;

    addMetaData("patient_id", readPatientID());
    addMetaData("test_id", readTestID());
    addMetaData("test_datetime", readTestDateTime());
    addMetaData("last_calibration_date", readCalibrationDate());

    QStringList sides = {"left", "right"};

    foreach (auto side, sides) {
        foreach (auto m, readHearingThresholdLevels(side)) {
        QSharedPointer<HearingMeasurement> measure(new HearingMeasurement(m));
        addMeasurement(measure);
        }
    }
}

QString HearingTest::readPatientID() const
{
    return readArray(PATIENT_ID_INDEX_START, PATIENT_ID_INDEX_END).trimmed();
}

QString HearingTest::readTestID() const
{
    return readArray(TEST_ID_INDEX_START, TEST_ID_INDEX_END).trimmed();
}

QDateTime HearingTest::readTestDateTime() const
{
    QString d_str = readArray(TEST_DATETIME_INDEX_START, TEST_DATETIME_INDEX_END).trimmed();

    if (d_str.contains("A"))
        d_str.replace("A", "1");

    return QDateTime::fromString(d_str, "MM/dd/yyHH:mm:ss").addYears(100);
}

QDate HearingTest::readCalibrationDate() const
{
    QString d_str = readArray(CALIBRATION_DATE_INDEX_START, CALIBRATION_DATE_INDEX_END).trimmed();
    return QDate::fromString(d_str,"MM/dd/yy").addYears(100);
}

QString HearingTest::readExaminerID() const
{
    return readArray(EXAMINER_ID_START, EXAMINER_ID_END).trimmed();
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
  QJsonArray leftResults {};
  QJsonArray rightResults {};

  foreach (const auto m, m_measurementList) {
    QJsonObject measureJson = m->toJsonObject();
    QString side = measureJson["side"].toString();
    measureJson.remove("side");

    if (side == "left")
    {
        leftResults.append(measureJson);
    }
    else if (side == "right") {
        rightResults.append(measureJson);
    }
  }

  QJsonObject json{};
  json.insert("metadata", m_metaData.toJsonObject());
  json.insert("results", QJsonObject { { "left", leftResults }, { "right", rightResults }});
  json.insert("manual_entry", getManualEntryMode());

  return json;
}

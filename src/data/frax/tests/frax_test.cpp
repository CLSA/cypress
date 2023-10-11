#include "frax_test.h"

#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include <QRandomGenerator>
#include "../../../auxiliary/Utilities.h"
#include "../measurements/frax_measurement.h"

/**
 * sample contents of output.txt from blackbox.exe
 *
 * t,19,50.2,0,21.60494,1,0,1,0,0,0, 0, -1.1, 6.37,1.17,4.47,0.34
 *
 * 0 1  2  3 4        5 6 7 8 9 10 11 12 13   14   15   16
 * 0 : t or z (score input for field at position 12)
 * 1 : country code (19 = Canada)
 * 2 : Age in years (positive real number, can have decimals) Age should be 40-90.
 * 3 : Sex (0 : men, 1 : women)
 * 4 : BMI kg/m2 (positive real number, can have decimals)
 * 5 : Previous fracture (0 : no, 1 : yes)
 * 6 : Parental history of hip fracture (0 : no, 1 : yes)
 * 7 : Current smoker (0 : no, 1 : yes)
 * 8 : Gluccocorticoid (0 : no, 1 : yes)
 * 9 : Rheumatoid Arthritis (0 : no, 1 : yes)
 * 10: Secondary osteoporosis (0 : no, 1 : yes)
 * 11: Alcohol more than two drinks a day (0 : no, 1 : yes)
 * 12: Femoral neck BMD (real number, can have decimals). There are two inputs possible, T-score or Z-score
 * 13: 10 year probability (x 100) of osteoporotic fracture, calculated without knowing BMD (positive real number with decimals)
 * 14: 10 year probability (x 100) of hip fracture, calculated without knowing BMD (positive real number with decimals)
 * 15: 10 year probability (x 100) of osteoporotic fracture, calculated knowing BMD (real number with decimals)
 * 16: 10 year probability (x 100) of hip fracture, calculated knowing BMD (real number with decimals)
 *
 */

FraxTest::FraxTest()
{
    setExpectedMeasurementCount(4);

    m_outputKeyList << "TYPE";
    m_outputKeyList << "COUNTRY_CODE";
    m_outputKeyList << "AGE";
    m_outputKeyList << "SEX";
    m_outputKeyList << "BODY_MASS_INDEX";
    m_outputKeyList << "PREVIOUS_FRACTURE";
    m_outputKeyList << "PARENT_HIP_FRACTURE";
    m_outputKeyList << "CURRENT_SMOKER";
    m_outputKeyList << "GLUCCOCORTICOID";
    m_outputKeyList << "RHEUMATOID_ARTHRITIS";
    m_outputKeyList << "SECONDARY_OSTEOPOROSIS";
    m_outputKeyList << "ALCOHOL";
    m_outputKeyList << "FEMORAL_NECK_TSCORE";
}

void FraxTest::fromFile(const QString& fileName)
{
    QFile ifile(fileName);
    if(ifile.open(QIODevice::ReadOnly))
    {
        qDebug() << "OK, reading input file " << fileName;

        QTextStream instream(&ifile);
        QString line = instream.readLine();
        if(false == instream.atEnd())
        {
          qDebug() << "Frax: More lines of content than expected";
        }
        ifile.close();
        reset();

        QStringList list = line.split(",");
        if (17 == list.size()) {
          QSharedPointer<FraxMeasurement> measure1(new FraxMeasurement);
          measure1->setAttribute("TYPE", "osteoporotic_fracture");
          measure1->setAttribute("PROBABILITY", list.at(13).toDouble(), "%");
          addMeasurement(measure1);

          QSharedPointer<FraxMeasurement> measure2(new FraxMeasurement);
          measure2->setAttribute("TYPE", "hip_fracture");
          measure2->setAttribute("PROBABILITY", list.at(14).toDouble(), "%");
          addMeasurement(measure2);

          QSharedPointer<FraxMeasurement> measure3(new FraxMeasurement);
          measure3->setAttribute("TYPE", "osteoporotic_fracture_bmd");
          measure3->setAttribute("PROBABILITY", list.at(15).toDouble(), "%");
          addMeasurement(measure3);

          QSharedPointer<FraxMeasurement> measure4(new FraxMeasurement);
          measure4->setAttribute("TYPE", "hip_fracture_bmd");
          measure4->setAttribute("PROBABILITY", list.at(16).toDouble(), "%");
          addMeasurement(measure4);

          addMetaData("TYPE", list.at(0).toLower());
          addMetaData("COUNTRY_CODE", list.at(1).toUInt());
          addMetaData("AGE", list.at(2).toDouble(), "yr");
          addMetaData("SEX", list.at(3).toUInt());
          addMetaData("BODY_MASS_INDEX", list.at(4).toDouble(), "kg/m2");
          addMetaData("PREVIOUS_FRACTURE", list.at(5).toUInt());
          addMetaData("PARENT_HIP_FRACTURE", list.at(6).toUInt());
          addMetaData("CURRENT_SMOKER", list.at(7).toUInt());
          addMetaData("GLUCCOCORTICOID", list.at(8).toUInt());
          addMetaData("RHEUMATOID_ARTHRITIS", list.at(9).toUInt());
          addMetaData("SECONDARY_OSTEOPOROSIS", list.at(10).toUInt());
          addMetaData("ALCOHOL", list.at(11).toUInt());
          addMetaData("FEMORAL_NECK_TSCORE", list.at(12).toDouble());
        }
    }
}

void FraxTest::simulate(const QVariantMap& input)
{
    foreach(auto key, input.keys())
    {
      if("barcode" == key || "language" == key) continue;

      QVariant value = input[key];
      if("SEX" == key)
      {
        value = "male" == value.toString() ? 0 : 1;
      }
      else if("FEMORAL_NECK_BMD" == key)
      {
        value = Utilities::tscore(value.toDouble());
        key = "FEMORAL_NECK_TSCORE";
      }
      else
      {
        if(QVariant::Bool == value.type())
        {
          value = value.toUInt();
        }
      }
      if("AGE" == key)
        addMetaData(key,value,"yr");
      else if("BODY_MASS_INDEX" == key)
        addMetaData(key,value,"kg/m2");
      else
        addMetaData(key,value);
    }

    QSharedPointer<FraxMeasurement> measure1(new FraxMeasurement);
    double mu = QRandomGenerator::global()->generateDouble();

    measure1->setAttribute("TYPE", "osteoporotic_fracture");
    double p = Utilities::interp(1.0f,30.0f,mu);
    measure1->setAttribute("PROBABILITY", p, "%");
    addMeasurement(measure1);

    QSharedPointer<FraxMeasurement> measure2(new FraxMeasurement);
    measure2->setAttribute("TYPE", "hip_fracture");
    p = Utilities::interp(0.0f,13.0f,mu);
    measure2->setAttribute("PROBABILITY", p, "%");
    addMeasurement(measure2);

    QSharedPointer<FraxMeasurement> measure3(new FraxMeasurement);
    measure3->setAttribute("TYPE", "osteoporotic_fracture_bmd");
    p = Utilities::interp(2.0f,22.0f,mu);
    measure3->setAttribute("PROBABILITY", p, "%");
    addMeasurement(measure3);

    QSharedPointer<FraxMeasurement> measure4(new FraxMeasurement);
    measure4->setAttribute("TYPE", "hip_fracture_bmd");
    p = Utilities::interp(0.0f,8.0f,mu);
    measure4->setAttribute("PROBABILITY", p, "%");
    addMeasurement(measure4);
}

// String representation for debug and GUI display purposes
//
QString FraxTest::toString() const
{
    QString str;
    if(isValid())
    {
      QStringList list;
      foreach(const auto measurement, m_measurementList)
      {
        list << measurement->toString();
      }
      str = list.join("\n");
    }
    return str;
}

bool FraxTest::isValid() const
{
    //bool okMeta = true;
    //foreach(const auto key, m_outputKeyList)
    //{
    //  if(!hasMetaData(key))
    //  {
    //     qDebug() << "ERROR: test missing meta data" << key;
    //     okMeta = false;
    //     break;
    //   }
    //}

    bool okTest = getMeasurementCount() == getExpectedMeasurementCount();
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
    //return okMeta && okTest;
}

QJsonObject FraxTest::toJsonObject() const
{
    QJsonObject testJson {

    };

    QJsonArray measurementArray;
    auto measurements { getMeasurements() };

    foreach (auto measurement, measurements) {
      measurementArray << measurement->toJsonObject();
    }

    QJsonObject valuesObject {};

    valuesObject.insert("metadata", getMetaData().toJsonObject());
    valuesObject.insert("results", measurementArray);

    testJson.insert("values", valuesObject);

    return testJson;
}

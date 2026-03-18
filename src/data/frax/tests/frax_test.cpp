#include "frax_test.h"
#include "data/frax/measurements/frax_measurement.h"


#include "auxiliary/Utilities.h"

#include "config/device_config.h"

#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include <QRandomGenerator>
#include <QDir>
#include <QFileInfo>


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
 * 8 : Glucocorticoid (0 : no, 1 : yes)
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
    m_outputKeyList << "type";
    m_outputKeyList << "country_code";
    m_outputKeyList << "age";
    m_outputKeyList << "sex";
    m_outputKeyList << "body_mass_index";
    m_outputKeyList << "previous_fracture";
    m_outputKeyList << "parent_hip_fracture";
    m_outputKeyList << "current_smoker";
    m_outputKeyList << "glucocorticoid";
    m_outputKeyList << "rheumatoid_arthritis";
    m_outputKeyList << "secondary_osteoporosis";
    m_outputKeyList << "alcohol";
    m_outputKeyList << "femoral_neck_tscore";

    setExpectedMeasurementCount(4);
    qDebug() << "GetExpected" << getExpectedMeasurementCount();
}

bool FraxTest::writeInputFile(const QString& inputFilePath, const QJsonObject& inputData)
{
    QString inputContents = getInputContents(inputData);
    addMetaData("frax_input", inputContents);

    qDebug() << "FraxTest::toFile - content = " << inputContents;

    QFile inputFile(inputFilePath);
    if (inputFile.exists()) {
        if (!inputFile.remove()) {
            qDebug() << "FraxTest::toFile - could not remove file";
            return false;
        }
    }

    if(inputFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(&inputFile);
        stream << inputContents << Qt::endl;
        inputFile.close();

        qDebug() << "populated input.txt file " << inputFilePath;
    }
    else {
        qDebug() << "FraxTest::toFile - Could not open input file";
        return false;
    }

    return true;
}

bool FraxTest::readOutputFile(const QString& fileName)
{
    QFile ifile(fileName);

    if (!ifile.open(QIODevice::ReadOnly)) {
        qDebug() << "Could not read frax output";
        return false;
    }

    qDebug() << "OK, reading input file " << fileName;

    QTextStream instream(&ifile);
    QString line = instream.readLine();

    if(!instream.atEnd()) {
        qDebug() << "Frax: More lines of content than expected";
        return false;
    }

    ifile.close();
    reset();

    QStringList list = line.split(",");

    qDebug() << list << list.size();

    if (list.size() != 17) {
        qCritical() << "Results are invalid (!= 17)";
        return false;
    }

    addMetaData("type",                   list.at(0).toLower());
    addMetaData("country_code",           list.at(1).toUInt());
    addMetaData("age",                    list.at(2).toDouble(), "yr");
    addMetaData("sex",                    list.at(3).toUInt());
    addMetaData("body_mass_index",        list.at(4).toDouble(), "kg/m2");
    addMetaData("previous_fracture",      list.at(5).toUInt());
    addMetaData("parent_hip_fracture",    list.at(6).toUInt());
    addMetaData("current_smoker",         list.at(7).toUInt());
    addMetaData("glucocorticoid",         list.at(8).toUInt());
    addMetaData("rheumatoid_arthritis",   list.at(9).toUInt());
    addMetaData("secondary_osteoporosis", list.at(10).toUInt());
    addMetaData("alcohol",                list.at(11).toUInt());
    addMetaData("femoral_neck_tscore",    list.at(12).toDouble());

    QSharedPointer<FraxMeasurement> measure1(new FraxMeasurement);
    measure1->setAttribute("type", "osteoporotic_fracture");
    measure1->setAttribute("probability", list.at(13).toDouble(), "%");
    addMeasurement(measure1);

    QSharedPointer<FraxMeasurement> measure2(new FraxMeasurement);
    measure2->setAttribute("type", "hip_fracture");
    measure2->setAttribute("probability", list.at(14).toDouble(), "%");
    addMeasurement(measure2);

    QSharedPointer<FraxMeasurement> measure3(new FraxMeasurement);
    measure3->setAttribute("type", "osteoporotic_fracture_bmd");
    measure3->setAttribute("probability", list.at(15).toDouble(), "%");
    addMeasurement(measure3);

    const double p = list.at(15).toDouble(); // interpretation of osteoporotic_fracture_bmd result for report
    QString interp = interpretResults(p);
    addMetaData("osteoporotic_fracture_bmd_interp", interp);

    QSharedPointer<FraxMeasurement> measure4(new FraxMeasurement);
    measure4->setAttribute("type", "hip_fracture_bmd");
    measure4->setAttribute("probability", list.at(16).toDouble(), "%");
    addMeasurement(measure4);



    return true;
}



QString FraxTest::getInputContents(const QJsonObject& inputData)
{
    QString type = inputData.value("type").toVariant().toString();
    QString countryCode = inputData.value("country_code").toVariant().toString();
    QString age = inputData.value("age").toVariant().toString();
    QString sex = inputData.value("sex").toVariant().toString().toLower()[0] == 'm' ? "0" : "1";
    QString bmi = inputData.value("body_mass_index").toVariant().toString();
    QString previousFracture = QString::number(inputData.value("previous_fracture").toVariant().toInt());
    QString parentHipFracture = QString::number(inputData.value("parent_hip_fracture").toVariant().toInt());
    QString currentSmoker = QString::number(inputData.value("current_smoker").toVariant().toInt());
    QString glucocorticoid = QString::number(inputData.value("glucocorticoid").toVariant().toInt());
    QString rheumatoidArthritis = QString::number(inputData.value("rheumatoid_arthritis").toVariant().toInt());
    QString secondaryOsteoporosis = QString::number(inputData.value("secondary_osteoporosis").toVariant().toInt());
    QString alcohol = QString::number(inputData.value("alcohol").toVariant().toInt());
    QString femoralNeckTScore = QString::number(inputData.value("femoral_neck_tscore").toVariant().toDouble());

    QStringList list;
    list << type;
    list << countryCode;
    list << age;
    list << sex;
    list << bmi;
    list << previousFracture;
    list << parentHipFracture;
    list << currentSmoker;
    list << glucocorticoid;
    list << rheumatoidArthritis;
    list << secondaryOsteoporosis;
    list << alcohol;
    list << femoralNeckTScore;

    return list.join(",");
}

QString FraxTest::interpretResults(double p)
{
    QString interp = "N/A";
    if (p > 20.0)
        interp = "High";
    else if ((p >= 10.0 && p <= 20.0) || m_metaData.getAttribute("femoral_neck_tscore").value().toDouble() <= -2.5)
        interp = "Moderate";
    else if (p < 10)
        interp = "Low";
    return interp;
}

// String representation for debug and GUI display purposes
//
QString FraxTest::toString() const
{
    QString str;
    QStringList list;

    foreach (const auto measurement, m_measurementList) {
      list << measurement->toString();
    }

    str = list.join("\n");

    return str;
}

bool FraxTest::isValid() const
{
    bool okMeta = true;
    foreach (const auto key, m_outputKeyList) {
        if (!hasMetaData(key)) {
            qDebug() << "ERROR: test missing meta data" << key;
            okMeta = false;
            break;
        }
    }

    bool okTest = getMeasurementCount() == getExpectedMeasurementCount();

    qDebug() << "measurement count: " << getMeasurementCount();
    qDebug() << "expected: " << getExpectedMeasurementCount();

    if(okTest) {
        foreach(const auto m, m_measurementList)
        {
            if (!m->isValid()) {
                okTest = false;
                qDebug() << "measurement is not valid:" << m->toJsonObject();
                break;
            }
        }
    }

    return okMeta && okTest;
}

QJsonObject FraxTest::toJsonObject() const
{
    QJsonArray measurementArray;
    auto measurements { getMeasurements() };

    foreach (auto measurement, measurements) {
        measurementArray << measurement->toJsonObject();
    }

    QJsonObject valuesObject {};

    valuesObject.insert("metadata", getMetaData().toJsonObject());
    valuesObject.insert("results", measurementArray);
    valuesObject.insert("manual_entry", getManualEntryMode());

    return valuesObject;
}

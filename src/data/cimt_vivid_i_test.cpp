#include "cimt_vivid_i_test.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QFile>


CimtVividiTest::CimtVividiTest()
{
    //setExpectedMeasurementCount(10);
}

void CimtVividiTest::simulate()
{
}

void CimtVividiTest::reset()
{
    TestBase::reset();

}

bool CimtVividiTest::isValid() const
{
    //if (getMeasurementCount() != getExpectedMeasurementCount()) {
    //    qDebug() << "Measurement count is incorrect";
    //    return false;
    //}

    auto measurements = getMeasurements();
    foreach (auto measurement, measurements)
    {
        if (!measurement->isValid()) {
            qDebug() << "Measurement is invalid" << measurement->toJsonObject();
            return false;
        }
    }

    return true;
}


QJsonObject CimtVividiTest::toJsonObject() const
{
    QJsonArray results {};

    QJsonObject leftResults {{"side", "left"}};
    QJsonArray leftFiles {};

    QJsonObject rightResults {{"side", "right"}};
    QJsonArray rightFiles {};

    QJsonObject unknownResults {{"side", "unknown"}};
    QJsonArray unknownFiles {};

    auto measurements { getMeasurements() };
    foreach(auto measurement, measurements)
    {
        QJsonObject measure = measurement->toJsonObject();
        QString side = measure["side"].toString();

        measure.remove("side");
        measure.remove("path");

        if (side == "Left")
            leftFiles.append(measure);
        else if (side == "Right")
            rightFiles.append(measure);
        else {
            unknownFiles.append(measure);
        }
    }

    leftResults["files"] = leftFiles;
    rightResults["files"] = rightFiles;
    unknownResults["files"] = unknownFiles;

    results.append(leftResults);
    results.append(rightResults);

    if (!unknownFiles.empty())
        results.append(unknownResults);

    QJsonObject valuesObject {};

    valuesObject.insert("metadata", getMetaData().toJsonObject());
    valuesObject.insert("results", results);
    valuesObject.insert("manual_entry", getManualEntryMode());

    return valuesObject;
}


QString CimtVividiTest::toString() const
{
    QString str;

    auto measurements { getMeasurements() };
    foreach(auto measurement, measurements)
    {
        str + "\n" + measurement->toString();
    }

    return str;
}

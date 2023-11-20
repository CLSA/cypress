#include "cimt_vivid_i_test.h"
#include "cimt_vivid_i_measurement.h"

#include "../auxiliary/Utilities.h"
#include "../auxiliary/file_utils.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QFile>


CimtVividiTest::CimtVividiTest()
{
    setExpectedMeasurementCount(10);
}

void CimtVividiTest::simulate()
{
    // Left
    QSharedPointer<CimtVividIMeasurement> left_cineloop(new CimtVividIMeasurement);
    left_cineloop->setAttribute("side", "left");
    left_cineloop->setAttribute("name", "CINELOOP_1_LEFT.dcm");
    left_cineloop->setAttribute("size", FileUtils::getHumanReadableFileSize("C:/Users/Anthony/Downloads/CIMT_SIM/Left/CINELOOP_1"));
    left_cineloop->setAttribute("path", "C:/Users/Anthony/Downloads/CIMT_SIM/Left/CINELOOP_1");
    addMeasurement(left_cineloop);

    QSharedPointer<CimtVividIMeasurement> left_still_1(new CimtVividIMeasurement);
    left_still_1->setAttribute("side", "left");
    left_still_1->setAttribute("name", "STILL_IMAGE_1_LEFT.dcm");
    left_still_1->setAttribute("size", FileUtils::getHumanReadableFileSize("C:/Users/Anthony/Downloads/CIMT_SIM/Left/STILL_IMAGE_1"));
    left_still_1->setAttribute("path", "C:/Users/Anthony/Downloads/CIMT_SIM/Left/STILL_IMAGE_1");
    addMeasurement(left_still_1);

    QSharedPointer<CimtVividIMeasurement> left_still_2(new CimtVividIMeasurement);
    left_still_2->setAttribute("side", "left");
    left_still_2->setAttribute("name", "STILL_IMAGE_2_LEFT.dcm");
    left_still_2->setAttribute("size", FileUtils::getHumanReadableFileSize("C:/Users/Anthony/Downloads/CIMT_SIM/Left/STILL_IMAGE_2"));
    left_still_2->setAttribute("path", "C:/Users/Anthony/Downloads/CIMT_SIM/Left/STILL_IMAGE_2");
    addMeasurement(left_still_2);

    QSharedPointer<CimtVividIMeasurement> left_still_3(new CimtVividIMeasurement);
    left_still_3->setAttribute("side", "left");
    left_still_3->setAttribute("name", "STILL_IMAGE_3_LEFT.dcm");
    left_still_3->setAttribute("size", FileUtils::getHumanReadableFileSize("C:/Users/Anthony/Downloads/CIMT_SIM/Left/STILL_IMAGE_3"));
    left_still_3->setAttribute("path", "C:/Users/Anthony/Downloads/CIMT_SIM/Left/STILL_IMAGE_3");
    addMeasurement(left_still_3);

    QSharedPointer<CimtVividIMeasurement> left_sr_1(new CimtVividIMeasurement);
    left_sr_1->setAttribute("side", "left");
    left_sr_1->setAttribute("name", "SR_1_LEFT.dcm");
    left_sr_1->setAttribute("size", FileUtils::getHumanReadableFileSize("C:/Users/Anthony/Downloads/CIMT_SIM/Left/SR_1"));
    left_sr_1->setAttribute("path", "C:/Users/Anthony/Downloads/CIMT_SIM/Left/SR_1");
    addMeasurement(left_sr_1);

    // Right
    QSharedPointer<CimtVividIMeasurement> right_cineloop(new CimtVividIMeasurement);
    right_cineloop->setAttribute("side", "right");
    right_cineloop->setAttribute("name", "CINELOOP_1_RIGHT.dcm");
    right_cineloop->setAttribute("size", FileUtils::getHumanReadableFileSize("C:/Users/Anthony/Downloads/CIMT_SIM/Right/CINELOOP_1"));
    right_cineloop->setAttribute("path", "C:/Users/Anthony/Downloads/CIMT_SIM/Right/CINELOOP_1");
    addMeasurement(right_cineloop);

    QSharedPointer<CimtVividIMeasurement> right_still_1(new CimtVividIMeasurement);
    right_still_1->setAttribute("side", "right");
    right_still_1->setAttribute("name", "STILL_IMAGE_1_RIGHT.dcm");
    right_still_1->setAttribute("size", FileUtils::getHumanReadableFileSize("C:/Users/Anthony/Downloads/CIMT_SIM/Right/STILL_IMAGE_1"));
    right_still_1->setAttribute("path", "C:/Users/Anthony/Downloads/CIMT_SIM/Right/STILL_IMAGE_1");
    addMeasurement(right_still_1);

    QSharedPointer<CimtVividIMeasurement> right_still_2(new CimtVividIMeasurement);
    right_still_2->setAttribute("side", "right");
    right_still_2->setAttribute("name", "STILL_IMAGE_2_RIGHT.dcm");
    right_still_2->setAttribute("size", FileUtils::getHumanReadableFileSize("C:/Users/Anthony/Downloads/CIMT_SIM/Right/STILL_IMAGE_2"));
    right_still_2->setAttribute("path", "C:/Users/Anthony/Downloads/CIMT_SIM/Right/STILL_IMAGE_2");
    addMeasurement(right_still_2);

    QSharedPointer<CimtVividIMeasurement> right_still_3(new CimtVividIMeasurement);
    right_still_3->setAttribute("side", "right");
    right_still_3->setAttribute("name", "STILL_IMAGE_3_RIGHT.dcm");
    right_still_3->setAttribute("size", FileUtils::getHumanReadableFileSize("C:/Users/Anthony/Downloads/CIMT_SIM/Right/STILL_IMAGE_3"));
    right_still_3->setAttribute("path", "C:/Users/Anthony/Downloads/CIMT_SIM/Right/STILL_IMAGE_3");
    addMeasurement(right_still_3);

    QSharedPointer<CimtVividIMeasurement> right_sr_1(new CimtVividIMeasurement);
    right_sr_1->setAttribute("side", "right");
    right_sr_1->setAttribute("name", "SR_1_RIGHT.dcm");
    right_sr_1->setAttribute("size", FileUtils::getHumanReadableFileSize("C:/Users/Anthony/Downloads/CIMT_SIM/Right/SR_1"));
    right_sr_1->setAttribute("path", "C:/Users/Anthony/Downloads/CIMT_SIM/Right/SR_1");
    addMeasurement(right_sr_1);
}

bool CimtVividiTest::isValid() const
{
    if (getMeasurementCount() != getExpectedMeasurementCount())
    {
        return false;
    }

    auto measurements = getMeasurements();
    foreach (auto measurement, measurements)
    {
        if (!measurement->isValid()) {
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

    auto measurements { getMeasurements() };
    foreach(auto measurement, measurements)
    {
        QJsonObject measure = measurement->toJsonObject();
        QString side = measure["side"].toString();

        measure.remove("side");
        measure.remove("path");

        if (side == "left")
            leftFiles.append(measure);
        else if (side == "right")
            rightFiles.append(measure);
        else
            qDebug() << "invalid measure, " << measure;
    }

    leftResults["files"] = leftFiles;
    rightResults["files"] = rightFiles;

    results.append(leftResults);
    results.append(rightResults);

    QJsonObject valuesObject {};

    valuesObject.insert("metadata", getMetaData().toJsonObject());
    valuesObject.insert("results", results);

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

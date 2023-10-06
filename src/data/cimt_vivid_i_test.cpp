#include "cimt_vivid_i_test.h"
#include "cimt_vivid_i_measurement.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

CimtVividiTest::CimtVividiTest()
{
    m_outputKeyList << "SIDE";
    m_outputKeyList << "CINELOOP_1";
    m_outputKeyList << "STILL_IMAGE_1";
    m_outputKeyList << "STILL_IMAGE_2";
    m_outputKeyList << "STILL_IMAGE_3";
    m_outputKeyList << "SR_1";

    setExpectedMeasurementCount(2);
}

QByteArray readFileIntoByteArray(QString filePath)
{
    QByteArray fileContent;

    // Open the file with the provided file path
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        // Error opening file
        qWarning("Cannot open file for reading: %s", qPrintable(file.errorString()));
        return fileContent;
    }

    // Read the content of the file
    fileContent = file.readAll();

    // Close the file
    file.close();

    return fileContent;
}

void CimtVividiTest::simulate()
{
    CimtVividIMeasurement measurementLeft;
    CimtVividIMeasurement measurementRight;

    measurementLeft.setAttribute("SIDE", "LEFT");
    measurementLeft.setAttribute("CINELOOP_1", "C:/Users/Anthony/Downloads/CIMT_SIM/Left/CINELOOP_1");
    measurementLeft.setAttribute("STILL_IMAGE_1", "C:/Users/Anthony/Downloads/CIMT_SIM/Left/STILL_IMAGE_1");
    measurementLeft.setAttribute("STILL_IMAGE_2", "C:/Users/Anthony/Downloads/CIMT_SIM/Left/STILL_IMAGE_2");
    measurementLeft.setAttribute("STILL_IMAGE_3", "C:/Users/Anthony/Downloads/CIMT_SIM/Left/STILL_IMAGE_3");
    measurementLeft.setAttribute("SR_1", "C:/Users/Anthony/Downloads/CIMT_SIM/Left/SR_1");

    measurementRight.setAttribute("SIDE", "RIGHT");
    measurementRight.setAttribute("CINELOOP_1", "C:/Users/Anthony/Downloads/CIMT_SIM/Right/CINELOOP_1");
    measurementRight.setAttribute("STILL_IMAGE_1", "C:/Users/Anthony/Downloads/CIMT_SIM/Right/STILL_IMAGE_1");
    measurementRight.setAttribute("STILL_IMAGE_2", "C:/Users/Anthony/Downloads/CIMT_SIM/Right/STILL_IMAGE_2");
    measurementRight.setAttribute("STILL_IMAGE_3", "C:/Users/Anthony/Downloads/CIMT_SIM/Right/STILL_IMAGE_3");
    measurementRight.setAttribute("SR_1", "C:/Users/Anthony/Downloads/CIMT_SIM/Right/SR_1");

    this->addMeasurement(measurementLeft);
    this->addMeasurement(measurementRight);
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
        if (!measurement.isValid())
        {
            return false;
        }
    }

    return true;
}


QJsonObject CimtVividiTest::toJsonObject() const
{
    QJsonObject testJson {
    };

    QJsonArray measurementArray;
    auto measurements { getMeasurements() };

    foreach(auto measurement, measurements)
    {
        measurementArray << measurement.toJsonObject();
    }

    QJsonObject valuesObject {};

    //valuesObject.insert("metadata", getMetaData().toJsonObject());
    valuesObject.insert("results", measurementArray);

    testJson.insert("value", valuesObject);

    return testJson;
}


QString CimtVividiTest::toString() const
{
    return "";
}

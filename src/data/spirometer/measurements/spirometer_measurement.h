#ifndef SPIROMETER_MEASUREMENT_H
#define SPIROMETER_MEASUREMENT_H

#include "data/measurement.h"

/*!
* \class SpirometerMeasurement
* \brief A SpirometerMeasurement class
*
* \sa Measurement
*/

class SpirometerMeasurement : public Measurement
{
public:
    enum ResultType {
        typeUnknown,
        typeBestValues,
        typeTrial
    };

    void setResultType(const ResultType& type){m_type = type;};
    ResultType getResultType(){return m_type;}

    bool isValid() const override;

    QString toString() const override;

    void simulate();

    static const QStringList parameterList;
    static const QMap<QString, QString> channelMap;
    static const QMap<QString, QString> resultMap;
    static const QMap<QString, QString> trialMap;

private:

    ResultType m_type { typeUnknown };
};

Q_DECLARE_METATYPE(SpirometerMeasurement);

QDebug operator<<(QDebug dbg, const SpirometerMeasurement&);

#endif // SPIROMETER_MEASUREMENT_H



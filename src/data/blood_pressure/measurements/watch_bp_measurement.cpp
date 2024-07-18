#include "watch_bp_measurement.h"

WatchBPMeasurement::WatchBPMeasurement() {
    m_outputKeyList << "ID";         // int
    m_outputKeyList << "Patient";    // int
    m_outputKeyList << "SYS";        // int
    m_outputKeyList << "DIA";        // int
    m_outputKeyList << "MAP";        // int
    m_outputKeyList << "PP";         // int
    m_outputKeyList << "cSYS";       // int
    m_outputKeyList << "cDIA";       // int
    m_outputKeyList << "cPP";        // int
    m_outputKeyList << "HR";         // int
    m_outputKeyList << "AFIB";       // int
    m_outputKeyList << "CODE";       // string
    m_outputKeyList << "NOTE";       // string
    m_outputKeyList << "Condition";  // int
    m_outputKeyList << "UpdateTime"; // int
    m_outputKeyList << "Device";     // int
    m_outputKeyList << "Date";       // int
    m_outputKeyList << "BPM";        // string
    m_outputKeyList << "AwakeTime";  // int
    m_outputKeyList << "AsleepTime"; // int
    m_outputKeyList << "Interval1";  // int
    m_outputKeyList << "Interval2";  // int
    m_outputKeyList << "Interval1C"; // int
    m_outputKeyList << "Interval2C"; // int
    m_outputKeyList << "Option";
}

QString WatchBPMeasurement::toString() const
{
    return "";
}

bool WatchBPMeasurement::isValid() const
{
    return false;
}

QJsonObject WatchBPMeasurement::toJsonObject() const
{
    QJsonObject json { };

    return json;
}

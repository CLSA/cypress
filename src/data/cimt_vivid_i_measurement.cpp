#include "cimt_vivid_i_measurement.h"

//#include "auxiliary/file_utils.h"

CimtVividIMeasurement::CimtVividIMeasurement()
{
    this->setAttribute("side", QVariant());
    this->setAttribute("name", QVariant());
    this->setAttribute("size", QVariant());
}

QString CimtVividIMeasurement::toString() const
{
    return QString("Side: %1, Name: %2, Size: %4").arg(
        getAttribute("side").toString(),
        getAttribute("name").toString(),
        getAttribute("patient_id").toString());
}

QStringList CimtVividIMeasurement::toStringList(const bool &no_keys) const
{
    Q_UNUSED(no_keys)
    return QStringList {};
}

bool CimtVividIMeasurement::isValid() const
{
    if (!hasAttribute("side"))
        return false;

    if (!hasAttribute("name"))
        return false;

    if (!hasAttribute("patient_id"))
        return false;

    const QString side = getAttribute("side").toString();
    if (side != "Left" && side != "Right") {
        qDebug() << "measure" << getAttribute("name").toString() << "does not have side defined";
        return false;
    }

    return true;
}

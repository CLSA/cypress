#include "cimt_vivid_i_measurement.h"

#include "auxiliary/file_utils.h"

CimtVividIMeasurement::CimtVividIMeasurement()
{
    this->setAttribute("CINELOOP_1", "");
    this->setAttribute("STILL_IMAGE_1", "");
    this->setAttribute("STILL_IMAGE_2", "");
    this->setAttribute("STILL_IMAGE_3", "");
    this->setAttribute("SR_1", "");
    this->setAttribute("SIDE", "");
}

QString CimtVividIMeasurement::toString() const
{
    return "";
}

QStringList CimtVividIMeasurement::toStringList(const bool &no_keys) const
{
    return QStringList {};
}

bool CimtVividIMeasurement::isValid() const
{
    return true;
    //if (m_attributes.contains("CINELOOP_1"))
    //{
    //    if (!FileUtils::doesFileExist(m_attributes["CINELOOP_1"].toString(), false))
    //    {
    //        return false;
    //    }
    //}


    //if (m_attributes.contains("STILL_IMAGE_1"))
    //{
    //    if (!FileUtils::doesFileExist(m_attributes["STILL_IMAGE"].toString(), false))
    //    {
    //        return false;
    //    }
    //}

    //if (m_attributes.contains("STILL_IMAGE_2"))
    //{
    //    if (!FileUtils::doesFileExist(m_attributes["STILL_IMAGE"].toString(), false))
    //    {
    //        return false;
    //    }
    //}

    //if (m_attributes.contains("STILL_IMAGE_3"))
    //{
    //    if (!FileUtils::doesFileExist(m_attributes["STILL_IMAGE"].toString(), false))
    //    {
    //        return false;
    //    }
    //}

    //if (m_attributes.contains("SR_1"))
    //{
    //    if (!FileUtils::doesFileExist(m_attributes["SR"].toString(), false))
    //    {
    //        return false;
    //    }
    //}

    //if (!m_attributes.contains("SIDE") || m_attributes.isEmpty())
    //{
    //    return false;
    //}

    return true;
}

QJsonObject CimtVividIMeasurement::toJsonObject() const
{
    return QJsonObject {

    };
}

#include "cimt_vivid_i_measurement.h"

#include "auxiliary/file_utils.h"

CimtVividIMeasurement::CimtVividIMeasurement()
{
    this->setAttribute("CINELOOP_1", "");
    this->setAttribute("CINELOOP_2", "");
    this->setAttribute("CINELOOP_3", "");
    this->setAttribute("CINELOOP_4", "");
    this->setAttribute("STILL_IMAGE", "");
    this->setAttribute("LATERALITY", "");
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
    if (m_attributes.contains("CINELOOP_1"))
    {
        if (!FileUtils::doesFileExist(m_attributes["CINELOOP_1"].toString(), false))
        {
            return false;
        }
    }

    if (m_attributes.contains("CINELOOP_2"))
    {
        if (!FileUtils::doesFileExist(m_attributes["CINELOOP_2"].toString(), false))
        {
            return false;
        }
    }

    if (m_attributes.contains("CINELOOP_3"))
    {
        if (!FileUtils::doesFileExist(m_attributes["CINELOOP_3"].toString(), false))
        {
            return false;
        }
    }

    if (m_attributes.contains("STILL_IMAGE"))
    {
        if (!FileUtils::doesFileExist(m_attributes["STILL_IMAGE"].toString(), false))
        {
            return false;
        }
    }

    if (m_attributes.contains("SR"))
    {
        if (!FileUtils::doesFileExist(m_attributes["SR"].toString(), false))
        {
            return false;
        }
    }

    if (m_attributes.contains("LATERALITY"))
    {

    }

    return true;
}

QJsonObject CimtVividIMeasurement::toJsonObject() const
{
    return QJsonObject {};
}

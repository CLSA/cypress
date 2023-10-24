#include "cypress_settings.h"

#include <QSettings>
#include <QException>
#include <QString>
#include <QDebug>

static const QMap<QString, QVariant> defaultSettings = {{
  {"pine/host",                    "127.0.0.1" },
  {"pine/port",                    5000 },
  {"pine/username",                "" },
  {"pine/password",                "" },

  {"client/server/host",           "127.0.0.1" },
  {"client/server/port",           9000 },
  {"client/server/args",           "" },

  {"grip_strength/working_dir",    "C:/Program Files/Tracker 5" },
  {"grip_strength/executable",     "WTS.exe" },
  {"grip_strength/database",       "Tracker 5 Data" },

  {"retina/patient_id", 		   "11111111-2222-3333-4444-555555555555" },
  {"retina/working_dir",           "C:/Program Files (x86)/TOPCON/IMAGEnet R4/" },
  {"retina/executable",            "imagenet.exe" },
  {"retina/database/driver", 	   "QODBC" },
  {"retina/database/host",         "localhost" },
  {"retina/database/database",     "IMAGEnet_R4" },
  {"retina/database/port",         1433 },
  {"retina/database/username",     "" },
  {"retina/database/password",     "" },

  {"vividi/dicom/aeTitle",         "DCMRECV" },
  {"vividi/dicom/hostname",        "0.0.0.0" },
  {"vividi/dicom/port",            1100 },
  {"vividi/dicom/asc_config",      "" },
  {"vividi/dicom/log_config",      "" },
  {"vividi/dicom/executable",      "dcmrecv.exe" },

  {"dxa/dicom/aeTitle",            "DCMRECV" },
  {"dxa/dicom/host", 		       "0.0.0.0" },
  {"dxa/dicom/port", 		       1100 },
  {"dxa/dicom/asc_config",         "" },
  {"dxa/dicom/log_config",         "" },
  {"dxa/dicom/executable",         "dcmrecv.exe" },

  {"dxa/patscandb/driver",         "QODBC" },
  {"dxa/patscandb/host",           "" },
  {"dxa/patscandb/port",           "" },
  {"dxa/patscandb/user",           "" },
  {"dxa/patscandb/password", 	   "" },
  {"dxa/patscandb/databaseName",   "" },

  {"dxa/refscandb/driver",         "QODBC" },
  {"dxa/refscandb/host",           "" },
  {"dxa/refscandb/port", 	       "" },
  {"dxa/refscandb/user", 	       "" },
  {"dxa/refscandb/password", 	   "" },
  {"dxa/refscandb/database", 	   "" },

  {"participant_report_en_path",      "C:\\Users\\antho\\Downloads\\CLSA_GEN_Proxy_V1.1_KEEP_2018_ForProg1 1.pdf" },
  {"participant_report_fr_path",      "C:\\Users\\antho\\Downloads\\CLSA_GEN_Proxy_V1.1_KEEP_2018_ForProg1 1.pdf" }
}};


CypressSettings::CypressSettings(): settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress")
{
    
}

QString CypressSettings::getPineHost() const
{
    QSettings &settings = getSettings();
    QString host = settings.value("pine/host", "127.0.0.1:9000").toString();

    return host;
}

void CypressSettings::setPineHost(const QString &host)
{
    if (host.isEmpty() || host.isNull())
        return;

    QSettings &settings = getSettings();
    settings.setValue("pine/host", host);
}

QString CypressSettings::getPineEndpoint() const
{
    QSettings &settings = getSettings();
    QString endpoint{settings.value("pine/endpoint", "/qa/pine/api/answer/").toString()};
    return endpoint;
}

void CypressSettings::setPineEndpoint(const QString &newEndpoint)
{
    if (newEndpoint.isEmpty() || newEndpoint.isNull())
        return;

    QSettings &settings = getSettings();
    settings.setValue("pine/endpoint", newEndpoint);
}

QString CypressSettings::getAnswerUrl(int answer_id) const
{
    QString host = getPineHost();
    QString endpoint = getPineEndpoint();

    QString answerUrl{QString("%1%2%3").arg(host, endpoint, QString::number(answer_id))};

    return answerUrl;
}

// .ini file can be found at C:\Users\<User>\AppData\Roaming\CLSA\Cypress.ini
void CypressSettings::setDefaultSettings()
{
    settings.clear();
    QMap<QString, QVariant>::const_iterator iter;
    for (iter = defaultSettings.constBegin(); iter != defaultSettings.constEnd(); ++iter) {
        settings.setValue(iter.key(), iter.value().toString());
    }
}


QVariant CypressSettings::readSetting(const QString& key) const
{
    QSettings& settings = getSettings();
    return settings.value(key);
}

void CypressSettings::writeSetting(const QString& key, const QVariant& value)
{
    QSettings& settings = getSettings();
    settings.setValue(key, value);
}

void CypressSettings::clearSettings()
{
    settings.clear();
}

QSettings& CypressSettings::getSettings() const
{
    return const_cast<QSettings&>(settings);
}



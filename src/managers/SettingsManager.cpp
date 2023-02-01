#include <QString>
#include <QException>
#include <QDebug>

#include "SettingsManager.h"

QMap<QString, QVariant> SettingsManager::defaultSettings = {{
    {"pine/host", ""},
    {"pine/port", ""},

    {"rest_api/host", 	"0.0.0.0"},
    {"rest_api/port", 	8000},
    {"rest_api/args", 	""},

    {"dicom/host", 		  "0.0.0.0"},
    {"dicom/port", 		  9000},
    {"dicom/asc_config",  "C:/work/clsa/cypress/dcmtk-3.6.7/dcmnet/etc/storescp.cfg"},
    {"dicom/log_config",  "./logger.cfg"},
    {"dicom/out_dir", 	  "C:/Users/antho/Documents/PACS"},
    {"dicom/working_dir", "C:/ProgramData/chocolatey/bin"},
    {"dicom/program", 	  "C:/ProgramData/chocolatey/bin/dcmrecv.exe"},



    {"grip_strength/workingDir", 	"C:/Program Files (x86)/Tracker 5"},
    {"grip_strength/runnableName", 	"C:/Program Files (x86)/Tracker 5/WTS.exe"},
    {"grip_strength/databaseDir", 	"C:/Program Files (x86)/Tracker 5/Data/Tracker 5 Data"},
    {"grip_strength/databaseHost", 	"localhost"},
    {"grip_strength/databaseName",  "Tracker 5"},
    {"grip_strength/databasePort",  ""},
    {"grip_strength/databaseUser",  "Anthony"},
    {"grip_strength/databasePassword", ""},

    {"retinal_camera/patient_id", 		    "11111111-2222-3333-4444-555555555555"},
    {"retinal_camera/exe", 				    ""},
    {"retinal_camera/database/version", 	"MSSQL Server 2008 R2 Express"},
    {"retinal_camera/database/driver", 	    "QODBC"},
    {"retinal_camera/database/host", 		"DESKTOP-OL8FLRR\\SQLEXPRESS"},
    {"retinal_camera/database/port", 		1433},
    {"retinal_camera/database/database", 	"IMAGEnet_R4"},

    {"dxa/patscandb/version", 	"MS Access"},
    {"dxa/patscandb/driver", 	"QODBC"},
    {"dxa/patscandb/host", 		""},
    {"dxa/patscandb/port", 		""},
    {"dxa/patscandb/user", 		""},
    {"dxa/patscandb/password", 	""},
    {"dxa/patscandb/database", 	""},

    {"dxa/refscandb/version", 	"MS Access"},
    {"dxa/refscandb/driver", 	"QODBC"},
    {"dxa/refscandb/host", 		""},
    {"dxa/refscandb/port", 		""},
    {"dxa/refscandb/user", 		""},
    {"dxa/refscandb/password", 	""},
    {"dxa/refscandb/database", 	""},
}};


SettingsManager::SettingsManager(QObject *parent,
                                 QSettings::Format format,
                                 QSettings::Scope scope,
                                 QString orgName,
                                 QString appName): QObject{parent},
      m_format(format),
      m_scope(scope),
      m_orgName(orgName),
      m_appName(appName)
{
    m_settings.reset(new QSettings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress"));
    setDefaultSettings();
}

// .ini file can be found at C:\Users\<User>\AppData\Roaming\CLSA\Cypress.ini
bool SettingsManager::setDefaultSettings()
{
    try {
        QMap<QString, QVariant>::const_iterator i;
        for (i = defaultSettings.constBegin(); i != defaultSettings.constEnd(); ++i) {
            if (!m_settings->contains(i.key())) {
                m_settings->setValue(i.key(), i.value());
            }
        }
    }
    catch (QException& e)
    {
        qDebug() << e.what();
        return false;
    }

    return true;
}

bool SettingsManager::isDefined(QString& key)
{
   return m_settings->contains(key);
}

bool SettingsManager::setSetting(QString& key, QVariant& value)
{
    try {
        m_settings->setValue(key, value);
    }
    catch (QException& e)
    {
        qDebug() << e.what();
        return false;
    }

    return true;
}

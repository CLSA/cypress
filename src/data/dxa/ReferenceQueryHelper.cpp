#include "ReferenceQueryHelper.h"

ReferenceQueryHelper::ReferenceQueryHelper(QSettings& settings)
{
   const QString databaseDriver = settings.value("dxa/ref/driver", "QODBC").toString();
   const QString hostname = settings.value("dxa/ref/hostname", "").toString();
   const QString database = settings.value("dxa/ref/database", "").toString();
   const QString username = settings.value("dxa/ref/username", "").toString();
   const QString password = settings.value("dxa/ref/password", "").toString();

   m_database = QSqlDatabase::addDatabase(databaseDriver);
   m_database.setHostName(hostname);
   m_database.setDatabaseName(database);
   m_database.setUserName(username);
   m_database.setPassword(password);
}

bool ReferenceQueryHelper::connect()
{
    if (!m_database.isValid()) return false;

    return m_database.open();
}

void ReferenceQueryHelper::disconnect()
{
    m_database.close();
}

QMap<QString, QVariant> ReferenceQueryHelper::getScanAnalysisData(QString patientKey, QString scanType)
{
    return QMap<QString, QVariant> {{}};
}

QMap<QString, QVariant> ReferenceQueryHelper::getScanData(QString patientKey, QString scanType)
{
    return QMap<QString, QVariant> {{}};
}

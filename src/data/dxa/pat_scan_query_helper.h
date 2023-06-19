#ifndef PAT_SCAN_QUERY_HELPER_H
#define PAT_SCAN_QUERY_HELPER_H

#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>

#include <QMap>
#include <QString>
#include <QVariant>
#include <QSettings>
#include <QVariantMap>

class PatScanQueryHelper
{
public:
    explicit PatScanQueryHelper(QSettings& settings);
    ~PatScanQueryHelper() = default;

    QVariantMap getParticipantData(QString participantId);
    QVariantMap getScanAnalysisData(QString patientKey, QString scanType);
    QVariantMap getScanData(QString patientKey, QString scanType);

private:
    bool connect();
    void disconnect();

    QSqlDatabase m_database;
};




#endif // PAT_SCAN_QUERY_HELPER_H

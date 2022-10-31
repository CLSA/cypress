#ifndef PATSCANQUERYHELPER_H
#define PATSCANQUERYHELPER_H

#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>

#include <QMap>
#include <QString>
#include <QVariant>
#include <QSettings>

class PatScanQueryHelper
{
public:
    explicit PatScanQueryHelper(QSettings& settings);
    ~PatScanQueryHelper() = default;

    QMap<QString, QVariant> getScanAnalysisData(QString patientKey, QString scanType);
    QMap<QString, QVariant> getScanData(QString patientKey, QString scanType);

private:
    bool connect();
    void disconnect();

    QSqlDatabase m_database;
};




#endif // PATSCANQUERYHELPER_H

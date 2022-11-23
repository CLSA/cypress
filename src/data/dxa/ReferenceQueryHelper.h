#ifndef REFERENCEQUERYHELPER_H
#define REFERENCEQUERYHELPER_H

#include <QSqlDatabase>
#include <QSqlQuery>

#include <QMap>
#include <QString>
#include <QVariant>
#include <QSettings>

class ReferenceQueryHelper
{
public:
    explicit ReferenceQueryHelper(QSettings& settings);
    ~ReferenceQueryHelper() = default;

    QMap<QString, QVariant> getScanAnalysisData(QString patientKey, QString scanType);
    QMap<QString, QVariant> getScanData(QString patientKey, QString scanType);

private:
    bool connect();
    void disconnect();

    QSqlDatabase m_database;
};

#endif // REFERENCEQUERYHELPER_H

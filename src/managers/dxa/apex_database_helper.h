#ifndef APEXDATABASEHELPER_H
#define APEXDATABASEHELPER_H

#include <QString>
#include <QVariantMap>
#include <QSqlDatabase>

namespace ApexDatabaseHelper
{
    QVariantMap extractScanAnalysis(const QSqlDatabase& db, const QString& patientKey, const QString& scanType);
    QVariantMap computeTZValues(const QSqlDatabase& db);

    void tScoreQuery1(const QSqlDatabase& db);
    void tScoreQuery2(const QSqlDatabase& db);

    void zScoreQuery1(const QSqlDatabase& db);
    void zScoreQuery2(const QSqlDatabase& db);
    void zScoreQuery3(const QSqlDatabase& db);

    double computeYearsDifference();
}

#endif // APEXDATABASEHELPER_H

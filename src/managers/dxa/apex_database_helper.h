#ifndef APEXDATABASEHELPER_H
#define APEXDATABASEHELPER_H

#include <QString>
#include <QVariantMap>
#include <QSqlDatabase>


namespace ApexDatabaseHelper
{
    struct Scan {
        const QString name;
        const QString bodyPart;
        const int scanType;
        const QString refType;
        const QString refSource;
    };

    struct SpineScan: public Scan {
        const QString name { "SP" };
        const QString bodyPart { "SPINE" };

        const int scanType { 1 };
        const QString refType { "S" };
        const QString refSource { "Hologic" };
    };

    struct LeftHipScan: public Scan {
        const QString name { "L_HIP" };
        const QString bodyPart { "HIP" };

        const int scanType { 2 };
        const QString refType { "H" };
        const QString refSource { "NHANES" };
    };

    struct RightHipScan: public Scan {
        const QString name { "R_HIP" };
        const QString bodyPart { "HIP" };

        const int scanType { 3 };
        const QString refType { "H" };
        const QString refSource { "NHANES" };
    };

    struct LeftForearmScan: public Scan {
        const QString name { "L_FA" };
        const QString bodyPart { "ARM" };

        const int scanType { 6 };
        const QString refType { "R" };
        const QString refSource { "Hologic" };
    };

    struct RightForearmScan: public Scan {
        const QString name { "R_FA" };
        const QString bodyPart { "ARM" };

        const int scanType { 7 };
        const QString refType { "R" };
        const QString refSource { "Hologic" };
    };


    struct WholeBodyScan: public Scan {
        const QString name { "WB" };
        const QString bodyPart { "WBODY" };

        const int scanType { 5 };
        const QString refType { "W" };
        const QString refSource { "NHANES" };
    };


    struct IVAImageScan: public Scan {
        const QString name { "DEL" };
        const QString bodyPart { "LSPINE" };

        const int scanType { 29 };
        const QString refType { "L" };
        const QString refSource { "NHANES" };
    };

    // Get patient key
    void patientScanQuery(const QSqlDatabase& db, const QString& participantId, QJsonObject& out);

    // Use patient key and scan type to get scan id(s)
    void scanAnalysisQuery(const QSqlDatabase& db, const QString& patientKey, int scanType, QJsonObject& out);

    // Using patient key and scan id get the variables
    void spineScanQuery(const QSqlDatabase& db, const QString& patientKey, const QString& scanId, QJsonObject& out);
    void hipScanQuery(const QSqlDatabase& db, const QString& patientKey, const QString& scanId, QJsonObject& out);
    void hipHsaQuery(const QSqlDatabase& db, const QString& patientKey, const QString& scanId, QJsonObject& out);
    void wBodyQuery(const QSqlDatabase& db, const QString& patientKey, const QString& scanId, QJsonObject& out);
    void wBodyCompositionQuery(const QSqlDatabase& db, const QString& patientKey, const QString& scanId, QJsonObject& out);
    void wBodySubRegionBoneQuery(const QSqlDatabase& db, const QString& patientKey, const QString& scanId, QJsonObject& out);
    void wBodySubRegionCompositionQuery(const QSqlDatabase& db, const QString& patientKey, const QString& scanId, QJsonObject& out);
    void wBodyObesityIndicesQuery(const QSqlDatabase& db, const QString& patientKey, const QString& scanId, QJsonObject& out);
    void wBodyAndroidGynoidCompositionQuery(const QSqlDatabase& db, const QString& patientKey, const QString& scanId, QJsonObject& out);
    void forearmQuery(const QSqlDatabase& db, const QString& patientKey, const QString& scanId, QJsonObject& out);

    // using the variables compute t and z score
    void computeTZValues(const Scan& scan, const QDateTime& scanDate, const QJsonObject& inputData, QJsonObject& out, const QSqlDatabase& db);

    double computeYearsDifference(const QDateTime& date1, const QDateTime& date2);
}

#endif // APEXDATABASEHELPER_H

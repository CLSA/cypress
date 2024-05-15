#include "apex_database_helper.h"
#include <QSqlDatabase>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QException>
#include <QJsonObject>
#include <QList>

static QJsonObject ranges = {
    // forearm
    {"RU13TOT_BMD", "1.."},
    {"RUMIDTOT_BMD", ".2."},
    {"RUUDTOT_BMD", "..3"},
    {"RUTOT_BMD", "123"},
    {"R_13_BMD", "R.."},
    {"R_MID_BMD", ".R."},
    {"R_UD_BMD", "..R"},
    {"RTOT_BMD", "RRR"},
    {"U_13_BMD", "U.."},
    {"U_MID_BMD", ".U."},
    {"U_UD_BMD", "..U"},
    {"UTOT_BMD", "UUU"},

    // whole body
    {"WBTOT_BMD", "NULL"},

    // hip
    {"NECK_BMD", "1..."},
    {"TROCH_BMD", ".2.."},
    {"INTER_BMD", "..3."},
    {"WARDS_BMD", "...4"},
    {"HTOT_BMD", "123."},

    // ap lumbar spine
    {"L1_BMD", "1..."},
    {"L2_BMD", ".2.."},
    {"L3_BMD", "..3."},
    {"L4_BMD", "...4"},
    {"TOT_BMD", "1234"},
    {"TOT_L1_BMD", "1..."},
    {"TOT_L2_BMD", ".2.."},
    {"TOT_L3_BMD", "..3."},
    {"TOT_L4_BMD", "...4"},
    {"TOT_L1L2_BMD", "12.."},
    {"TOT_L1L3_BMD", "1.3."},
    {"TOT_L1L4_BMD", "1..4"},
    {"TOT_L2L3_BMD", ".23."},
    {"TOT_L2L4_BMD", ".2.4"},
    {"TOT_L3L4_BMD", "..34"},
    {"TOT_L1L2L3_BMD", "123."},
    {"TOT_L1L2L4_BMD", "12.4"},
    {"TOT_L1L3L4_BMD", "1.34"},
    {"TOT_L2L3L4_BMD", ".234"}
};

struct AgeBracket {
    double ageMin { -DBL_MAX };
    double ageMax { DBL_MAX };
    double ageSpan { 0.0 };

    void compute(double age, const QList<double>& ageTable) {
        for (int i = 0; i < ageTable.size() - 1; i++) {
            double min = ageTable.at(i);
            double max = ageTable.at(i + 1);

            if (age >= min && age <= max) {
                ageMin = min;
                ageMax = age == min ? min : max;
            }
            else if (age > max) {
                ageMin = max;
                ageMax = max;
            }
        }

        if (ageMin == -DBL_MAX) ageMin = age;
        if (ageMax == DBL_MAX) ageMax = age;
        ageSpan = ageMax - ageMin;
    }

};

void ApexDatabaseHelper::patientScanQuery(const QSqlDatabase& db, const QString& participantId, QJsonObject& out) {
    QSqlQuery query(db);

    query.prepare("SELECT PATIENT_KEY, BIRTHDATE, SEX, ETHNICITY, WEIGHT, HEIGHT FROM PATIENT WHERE IDENTIFIER1 = :participantId");
    query.bindValue(":participantId", participantId);

    if (!query.exec()) {
        qDebug() << query.lastError();
        throw QException();
    }

    if (!query.first()) {
        return;
    }

    out.insert("PATIENT_KEY", query.value("PATIENT_KEY").toString());
    out.insert("BIRTHDATE",   query.value("BIRTHDATE").toString());
    out.insert("SEX",         query.value("SEX").toString());
    out.insert("ETHNICITY",   query.value("ETHNICITY").toString());
    out.insert("WEIGHT", 	  query.value("WEIGHT").toDouble());
    out.insert("HEIGHT",      query.value("HEIGHT").toDouble());
}

void ApexDatabaseHelper::scanAnalysisQuery(const QSqlDatabase& db, const QString& patientKey, int scanType, QJsonObject& out) {
    QSqlQuery query(db);

    query.prepare("SELECT SCANID, SCAN_MODE, SCAN_DATE FROM ScanAnalysis WHERE PATIENT_KEY = :patientKey AND SCAN_TYPE = :scanType");
    query.bindValue(":patientKey", patientKey);
    query.bindValue(":scanType", scanType);

    if (!query.exec()) {
        throw QException();
    }

    QString scanId;
    QString scanMode;
    QString scanDate;

    while (query.next()) {
        scanId = query.value("SCANID").toString();
        scanMode = query.value("SCAN_MODE").toString();
        scanDate = query.value("SCAN_DATE").toString();

        out.insert("SCANID", scanId);
        out.insert("SCAN_MODE", scanMode);
        out.insert("SCAN_DATE", scanDate);
    }
}

void ApexDatabaseHelper::spineScanQuery(const QSqlDatabase& db, const QString& patientKey, const QString& scanId, QJsonObject& out) {
    QSqlQuery query(db);

    // Spine
    query.prepare("SELECT * FROM Spine WHERE PATIENT_KEY = :patientKey AND SCANID = :scanId");
    query.bindValue(":patientKey", patientKey);
    query.bindValue(":scanId", scanId);

    if (!query.exec()) {
        qDebug() << "spine query failed" << query.lastError().text();
        throw QException();
    }

    if (!query.first()) {
        return;
    }

    out.insert("PHYSICIAN_COMMENT", query.value("PHYSICIAN_COMMENT").toString());

    out.insert("NO_REGIONS",      query.value("NO_REGIONS").toLongLong());
    out.insert("STARTING_REGION", query.value("STARTING_REGION").toLongLong());

    out.insert("L1_INCLUDED", query.value("L1_INCLUDED").toBool());
    out.insert("L1_AREA",     query.value("L1_AREA").toDouble());
    out.insert("L1_BMC",      query.value("L1_BMC").toDouble());
    out.insert("L1_BMD",      query.value("L1_BMD").toDouble());

    out.insert("L2_INCLUDED", query.value("L2_INCLUDED").toBool());
    out.insert("L2_AREA",     query.value("L2_AREA").toDouble());
    out.insert("L2_BMC",      query.value("L2_BMC").toDouble());
    out.insert("L2_BMD",      query.value("L2_BMD").toDouble());

    out.insert("L3_INCLUDED", query.value("L3_INCLUDED").toBool());
    out.insert("L3_AREA",     query.value("L3_AREA").toDouble());
    out.insert("L3_BMC",      query.value("L3_BMC").toDouble());
    out.insert("L3_BMD",      query.value("L3_BMD").toDouble());

    out.insert("L4_INCLUDED", query.value("L4_INCLUDED").toBool());
    out.insert("L4_AREA",     query.value("L4_AREA").toDouble());
    out.insert("L4_BMC",      query.value("L4_BMC").toDouble());
    out.insert("L4_BMD",      query.value("L4_BMD").toDouble());

    out.insert("TOT_AREA",    query.value("TOT_AREA").toDouble());
    out.insert("TOT_BMC",     query.value("TOT_BMC").toDouble());
    out.insert("TOT_BMD",     query.value("TOT_BMD").toDouble());
    out.insert("STD_TOT_BMD", query.value("STD_TOT_BMD").toDouble());

    out.insert("ROI_TYPE",    query.value("ROI_TYPE").toLongLong());
    out.insert("ROI_WIDTH",   query.value("ROI_WIDTH").toDouble());
    out.insert("ROI_HEIGHT",  query.value("ROI_HEIGHT").toDouble());
}

void ApexDatabaseHelper::hipScanQuery(const QSqlDatabase& db, const QString& patientKey, const QString& scanId, QJsonObject& out) {
    QSqlQuery query(db);

    query.prepare("SELECT * FROM Hip WHERE PATIENT_KEY = :patientKey AND SCANID = :scanId");
    query.bindValue(":patientKey", patientKey);
    query.bindValue(":scanId", scanId);

    if (!query.exec()) {
        qDebug() << "hip query failed: " << query.lastError().text();
        throw QException();
    }

    if (!query.first()) {
        return;
    }

    out.insert("TROCH_AREA", query.value("TROCH_AREA").toDouble());
    out.insert("TROCH_BMC",  query.value("TROCH_BMC").toDouble());
    out.insert("TROCH_BMD",  query.value("TROCH_BMD").toDouble());

    out.insert("INTER_AREA", query.value("INTER_AREA").toDouble());
    out.insert("INTER_BMC",  query.value("INTER_BMC").toDouble());
    out.insert("INTER_BMD",  query.value("INTER_BMD").toDouble());

    out.insert("NECK_AREA",  query.value("NECK_AREA").toDouble());
    out.insert("NECK_BMC",   query.value("NECK_BMC").toDouble());
    out.insert("NECK_BMD",   query.value("NECK_BMD").toDouble());

    out.insert("WARDS_AREA", query.value("WARDS_AREA").toDouble());
    out.insert("WARDS_BMC",  query.value("WARDS_BMC").toDouble());
    out.insert("WARDS_BMD",  query.value("WARDS_BMD").toDouble());

    out.insert("HTOT_AREA",  query.value("HTOT_AREA").toDouble());
    out.insert("HTOT_BMC",   query.value("HTOT_BMC").toDouble());
    out.insert("HTOT_BMD",   query.value("HTOT_BMD").toDouble());

    out.insert("ROI_TYPE",   query.value("ROI_TYPE").toLongLong());
    out.insert("ROI_WIDTH",  query.value("ROI_WIDTH").toDouble());
    out.insert("ROI_HEIGHT", query.value("ROI_HEIGHT").toDouble());

    out.insert("AXIS_LENGTH",       query.value("AXIS_LENGTH").toDouble());
    out.insert("PHYSICIAN_COMMENT", query.value("PHYSICIAN_COMMENT").toString());
}

void ApexDatabaseHelper::hipHsaQuery(const QSqlDatabase& db, const QString& patientKey, const QString& scanId, QJsonObject& out) {
    QSqlQuery query(db);
    query.prepare("SELECT * FROM HipHSA WHERE PATIENT_KEY = :patientKey AND SCANID = :scanId");
    query.bindValue(":patientKey", patientKey);
    query.bindValue(":scanId", scanId);

    if (!query.exec()) {
        qDebug() << "hipHSA query failed" << query.lastError().text();
        throw QException();
    }

    if (!query.first()) {
        return;
    }

    out.insert("NN_BMD",      query.value("NN_BMD").toDouble());
    out.insert("NN_CSA",      query.value("NN_CSA").toDouble());
    out.insert("NN_CSMI",     query.value("NN_CSMI").toDouble());
    out.insert("NN_WIDTH",    query.value("NN_WIDTH").toDouble());
    out.insert("NN_ED",       query.value("NN_ED").toDouble());
    out.insert("NN_ACT",      query.value("NN_ACT").toDouble());
    out.insert("NN_PCD",      query.value("NN_PCD").toDouble());
    out.insert("NN_CMP",      query.value("NN_CMP").toDouble());
    out.insert("NN_SECT_MOD", query.value("NN_SECT_MOD").toDouble());
    out.insert("NN_BR", 	  query.value("NN_BR").toDouble());

    out.insert("IT_BMD",      query.value("IT_BMD").toDouble());
    out.insert("IT_CSA",      query.value("IT_CSA").toDouble());
    out.insert("IT_CSMI",     query.value("IT_CSMI").toDouble());
    out.insert("IT_WIDTH",    query.value("IT_WIDTH").toDouble());
    out.insert("IT_ED",       query.value("IT_ED").toDouble());
    out.insert("IT_ACT",      query.value("IT_ACT").toDouble());
    out.insert("IT_PCD",      query.value("IT_PCD").toDouble());
    out.insert("IT_CMP",      query.value("IT_CMP").toDouble());
    out.insert("IT_SECT_MOD", query.value("IT_SECT_MOD").toDouble());
    out.insert("IT_BR",       query.value("IT_BR").toDouble());

    out.insert("FS_BMD",      query.value("FS_BMD").toDouble());
    out.insert("FS_CSA",      query.value("FS_CSA").toDouble());
    out.insert("FS_CSMI",     query.value("FS_CSMI").toDouble());
    out.insert("FS_WIDTH",    query.value("FS_WIDTH").toDouble());
    out.insert("FS_ED", 	  query.value("FS_ED").toDouble());
    out.insert("FS_ACT",      query.value("FS_ACT").toDouble());
    out.insert("FS_PCD",      query.value("FS_PCD").toDouble());
    out.insert("FS_CMP",      query.value("FS_CMP").toDouble());
    out.insert("FS_SECT_MOD", query.value("FS_SECT_MOD").toDouble());
    out.insert("FS_BR",       query.value("FS_BR").toDouble());

    out.insert("SHAFT_NECK_ANGLE", query.value("SHAFT_NECK_ANGLE").toDouble());
}

void ApexDatabaseHelper::wBodyQuery(const QSqlDatabase& db, const QString& patientKey, const QString& scanId, QJsonObject& out) {
    // Whole body
    QSqlQuery query(db);

    query.prepare("SELECT * FROM Wbody WHERE PATIENT_KEY = :patientKey AND SCANID = :scanId");
    query.bindValue(":patientKey", patientKey);
    query.bindValue(":scanId", scanId);

    if (!query.exec()) {
        qDebug() << query.lastError().text();
        throw QException();
    }

    if (!query.first()) {
        return;
    }

    out.insert("WBTOT_AREA", query.value("WBTOT_AREA").toDouble());
    out.insert("WBTOT_BMC",  query.value("WBTOT_BMC").toDouble());
    out.insert("WBTOT_BMD",  query.value("WBTOT_BMD").toDouble());

    out.insert("SUBTOT_AREA", query.value("SUBTOT_AREA").toDouble());
    out.insert("SUBTOT_BMC",  query.value("SUBTOT_BMC").toDouble());
    out.insert("SUBTOT_BMD",  query.value("SUBTOT_BMD").toDouble());

    out.insert("HEAD_AREA", query.value("HEAD_AREA").toDouble());
    out.insert("HEAD_BMC",  query.value("HEAD_BMC").toDouble());
    out.insert("HEAD_BMD",  query.value("HEAD_BMD").toDouble());

    out.insert("LARM_AREA", query.value("LARM_AREA").toDouble());
    out.insert("LARM_BMC",  query.value("LARM_BMC").toDouble());
    out.insert("LARM_BMD",  query.value("LARM_BMD").toDouble());

    out.insert("RARM_AREA", query.value("RARM_AREA").toDouble());
    out.insert("RARM_BMC",  query.value("RARM_BMC").toDouble());
    out.insert("RARM_BMD",  query.value("RARM_BMD").toDouble());

    out.insert("LRIB_AREA", query.value("LRIB_AREA").toDouble());
    out.insert("LRIB_BMC",  query.value("LRIB_BMC").toDouble());
    out.insert("LRIB_BMD",  query.value("LRIB_BMD").toDouble());

    out.insert("RRIB_AREA", query.value("RRIB_AREA").toDouble());
    out.insert("RRIB_BMC",  query.value("RRIB_BMC").toDouble());
    out.insert("RRIB_BMD",  query.value("RRIB_BMD").toDouble());

    out.insert("T_S_AREA",  query.value("T_S_AREA").toDouble());
    out.insert("T_S_BMC",   query.value("T_S_BMC").toDouble());
    out.insert("T_S_BMD",   query.value("T_S_BMD").toDouble());
    out.insert("L_S_AREA",  query.value("L_S_AREA").toDouble());
    out.insert("L_S_BMC",   query.value("L_S_BMC").toDouble());
    out.insert("L_S_BMD",   query.value("L_S_BMD").toDouble());

    out.insert("PELV_AREA", query.value("PELV_AREA").toDouble());
    out.insert("PELV_BMC",  query.value("PELV_BMC").toDouble());
    out.insert("PELV_BMD",  query.value("PELV_BMD").toDouble());

    out.insert("LLEG_AREA", query.value("LLEG_AREA").toDouble());
    out.insert("LLEG_BMC",  query.value("LLEG_BMC").toDouble());
    out.insert("LLEG_BMD",  query.value("LLEG_BMD").toDouble());

    out.insert("RLEG_AREA", query.value("RLEG_AREA").toDouble());
    out.insert("RLEG_BMC",  query.value("RLEG_BMC").toDouble());
    out.insert("RLEG_BMD",  query.value("RLEG_BMD").toDouble());

    out.insert("PHYSICIAN_COMMENT", query.value("PHYSICIAN_COMMENT").toString());
}

void ApexDatabaseHelper::wBodyCompositionQuery(const QSqlDatabase& db, const QString& patientKey, const QString& scanId, QJsonObject& out) {
    QSqlQuery query(db);
    query.prepare("SELECT * FROM WbodyComposition WHERE PATIENT_KEY = :patientKey AND SCANID = :scanId");
    query.bindValue(":patientKey", patientKey);
    query.bindValue(":scanId", scanId);

    if (!query.exec()) {
        qDebug() << query.lastError().text();
        throw QException();
    }

    if (!query.first()) {
        return;
    }

    out.insert("FAT_STD",     query.value("FAT_STD").toDouble());
    out.insert("LEAN_STD",    query.value("LEAN_STD").toDouble());
    out.insert("BRAIN_FAT",   query.value("BRAIN_FAT").toDouble());
    out.insert("WATER_LBM",   query.value("WATER_LBM").toDouble());

    out.insert("HEAD_FAT",    query.value("HEAD_FAT").toDouble());
    out.insert("HEAD_LEAN",   query.value("HEAD_LEAN").toDouble());
    out.insert("HEAD_MASS",   query.value("HEAD_MASS").toDouble());
    out.insert("HEAD_PFAT",   query.value("HEAD_PFAT").toDouble());

    out.insert("LARM_FAT",    query.value("LARM_FAT").toDouble());
    out.insert("LARM_LEAN",   query.value("LARM_LEAN").toDouble());
    out.insert("LARM_MASS",   query.value("LARM_MASS").toDouble());
    out.insert("LARM_PFAT",   query.value("LARM_PFAT").toDouble());

    out.insert("RARM_FAT",    query.value("RARM_FAT").toDouble());
    out.insert("RARM_LEAN",   query.value("RARM_LEAN").toDouble());
    out.insert("RARM_MASS",   query.value("RARM_MASS").toDouble());
    out.insert("RARM_PFAT",   query.value("RARM_PFAT").toDouble());

    out.insert("TRUNK_FAT",   query.value("TRUNK_FAT").toDouble());
    out.insert("TRUNK_LEAN",  query.value("TRUNK_LEAN").toDouble());
    out.insert("TRUNK_MASS",  query.value("TRUNK_MASS").toDouble());
    out.insert("TRUNK_PFAT",  query.value("TRUNK_PFAT").toDouble());

    out.insert("L_LEG_FAT",   query.value("L_LEG_FAT").toDouble());
    out.insert("L_LEG_LEAN",  query.value("L_LEG_LEAN").toDouble());
    out.insert("L_LEG_MASS",  query.value("L_LEG_MASS").toDouble());
    out.insert("L_LEG_PFAT",  query.value("L_LEG_PFAT").toDouble());

    out.insert("R_LEG_FAT",   query.value("R_LEG_FAT").toDouble());
    out.insert("R_LEG_LEAN",  query.value("R_LEG_LEAN").toDouble());
    out.insert("R_LEG_MASS",  query.value("R_LEG_MASS").toDouble());
    out.insert("R_LEG_PFAT",  query.value("R_LEG_PFAT").toDouble());

    out.insert("SUBTOT_FAT",  query.value("SUBTOT_FAT").toDouble());
    out.insert("SUBTOT_LEAN", query.value("SUBTOT_LEAN").toDouble());
    out.insert("SUBTOT_MASS", query.value("SUBTOT_MASS").toDouble());
    out.insert("SUBTOT_PFAT", query.value("SUBTOT_PFAT").toDouble());

    out.insert("WBTOT_FAT",   query.value("WBTOT_FAT").toDouble());
    out.insert("WBTOT_LEAN",  query.value("WBTOT_LEAN").toDouble());
    out.insert("WBTOT_MASS",  query.value("WBTOT_MASS").toDouble());
    out.insert("WBTOT_PFAT",  query.value("WBTOT_PFAT").toDouble());

    out.insert("PHYSICIAN_COMMENT", query.value("PHYSICIAN_COMMENT").toString());
}

void ApexDatabaseHelper::wBodySubRegionBoneQuery(const QSqlDatabase& db, const QString& patientKey, const QString& scanId, QJsonObject& out) {
    QSqlQuery query(db);
    query.prepare("SELECT * FROM SubRegionBone WHERE PATIENT_KEY = :patientKey AND SCANID = :scanId");
    query.bindValue(":patientKey", patientKey);
    query.bindValue(":scanId", scanId);

    if (!query.exec()) {
        qDebug() << query.lastError().text();
        throw QException();
    }

    if (!query.first()) {
        return;
    }

    out.insert("PHYSICIAN_COMMENT", query.value("PHYSICIAN_COMMENT").toString());
    out.insert("NO_REGIONS",        query.value("NO_REGIONS").toLongLong());

    out.insert("NET_AVG_AREA", query.value("NET_AVG_AREA").toDouble());
    out.insert("NET_AVG_BMC",  query.value("NET_AVG_BMC").toDouble());
    out.insert("NET_AVG_BMD",  query.value("NET_AVG_BMD").toDouble());

    out.insert("GLOBAL_AREA",  query.value("GLOBAL_AREA").toDouble());
    out.insert("GLOBAL_BMC",   query.value("GLOBAL_AREA").toDouble());
    out.insert("GLOBAL_BMD",   query.value("GLOBAL_AREA").toDouble());

    out.insert("REG1_NAME",  query.value("REG1_NAME").toString());
    out.insert("REG1_AREA",  query.value("REG1_AREA").toDouble());
    out.insert("REG1_BMC",   query.value("REG1_BMC").toDouble());
    out.insert("REG1_BMD",   query.value("REG1_BMD").toDouble());

    out.insert("REG2_NAME",  query.value("REG2_NAME").toString());
    out.insert("REG2_AREA",  query.value("REG2_AREA").toDouble());
    out.insert("REG2_BMC",   query.value("REG2_BMC").toDouble());
    out.insert("REG2_BMD",   query.value("REG2_BMD").toDouble());

    out.insert("REG3_NAME",  query.value("REG3_NAME").toString());
    out.insert("REG3_AREA",  query.value("REG3_AREA").toDouble());
    out.insert("REG3_BMC",   query.value("REG3_BMC").toDouble());
    out.insert("REG3_BMD",   query.value("REG3_BMD").toDouble());

    out.insert("REG3_NAME",  query.value("REG3_NAME").toString());
    out.insert("REG3_AREA",  query.value("REG3_AREA").toDouble());
    out.insert("REG3_BMC",   query.value("REG3_BMC").toDouble());
    out.insert("REG3_BMD",   query.value("REG3_BMD").toDouble());

    out.insert("REG4_NAME",  query.value("REG4_NAME").toString());
    out.insert("REG4_AREA",  query.value("REG4_AREA").toDouble());
    out.insert("REG4_BMC",   query.value("REG4_BMC").toDouble());
    out.insert("REG4_BMD",   query.value("REG4_BMD").toDouble());

    out.insert("REG5_NAME",  query.value("REG5_NAME").toString());
    out.insert("REG5_AREA",  query.value("REG5_AREA").toDouble());
    out.insert("REG5_BMC",   query.value("REG5_BMC").toDouble());
    out.insert("REG5_BMD",   query.value("REG5_BMD").toDouble());

    out.insert("REG6_NAME",  query.value("REG6_NAME").toString());
    out.insert("REG6_AREA",  query.value("REG6_AREA").toDouble());
    out.insert("REG6_BMC",   query.value("REG6_BMC").toDouble());
    out.insert("REG6_BMD",   query.value("REG6_BMD").toDouble());

    out.insert("REG7_NAME",  query.value("REG7_NAME").toString());
    out.insert("REG7_AREA",  query.value("REG7_AREA").toDouble());
    out.insert("REG7_BMC",   query.value("REG7_BMC").toDouble());
    out.insert("REG7_BMD",   query.value("REG7_BMD").toDouble());

    out.insert("REG8_NAME",  query.value("REG8_NAME").toString());
    out.insert("REG8_AREA",  query.value("REG8_AREA").toDouble());
    out.insert("REG8_BMC",   query.value("REG8_BMC").toDouble());
    out.insert("REG8_BMD",   query.value("REG8_BMD").toDouble());

    out.insert("REG9_NAME",  query.value("REG9_NAME").toString());
    out.insert("REG9_AREA",  query.value("REG9_AREA").toDouble());
    out.insert("REG9_BMC",   query.value("REG9_BMC").toDouble());
    out.insert("REG9_BMD",   query.value("REG9_BMD").toDouble());

    out.insert("REG10_NAME", query.value("REG10_NAME").toString());
    out.insert("REG10_AREA", query.value("REG10_AREA").toDouble());
    out.insert("REG10_BMC",  query.value("REG10_BMC").toDouble());
    out.insert("REG10_BMD",  query.value("REG10_BMD").toDouble());

    out.insert("REG11_NAME", query.value("REG11_NAME").toString());
    out.insert("REG11_AREA", query.value("REG11_AREA").toDouble());
    out.insert("REG11_BMC",  query.value("REG11_BMC").toDouble());
    out.insert("REG11_BMD",  query.value("REG11_BMD").toDouble());

    out.insert("REG12_NAME", query.value("REG12_NAME").toString());
    out.insert("REG12_AREA", query.value("REG12_AREA").toDouble());
    out.insert("REG12_BMC",  query.value("REG12_BMC").toDouble());
    out.insert("REG12_BMD",  query.value("REG12_BMD").toDouble());

    out.insert("REG13_NAME", query.value("REG13_NAME").toString());
    out.insert("REG13_AREA", query.value("REG13_AREA").toDouble());
    out.insert("REG13_BMC",  query.value("REG13_BMC").toDouble());
    out.insert("REG13_BMD",  query.value("REG13_BMD").toDouble());

    out.insert("REG14_NAME", query.value("REG14_NAME").toString());
    out.insert("REG14_AREA", query.value("REG14_AREA").toDouble());
    out.insert("REG14_BMC",  query.value("REG14_BMC").toDouble());
    out.insert("REG14_BMD",  query.value("REG14_BMD").toDouble());
}

void ApexDatabaseHelper::wBodySubRegionCompositionQuery(const QSqlDatabase& db, const QString& patientKey, const QString& scanId, QJsonObject& out) {
    QSqlQuery query(db);
    query.prepare("SELECT * FROM SubRegionComposition WHERE PATIENT_KEY = :patientKey AND SCANID = :scanId");
    query.bindValue(":patientKey", patientKey);
    query.bindValue(":scanId", scanId);

    if (!query.exec()) {
        qDebug() << query.lastError().text();
        throw QException();
    }

    if (!query.first()) {
        return;
    }

    out.insert("NET_AVG_FAT",  query.value("NET_AVG_FAT").toDouble());
    out.insert("NET_AVG_LEAN", query.value("NET_AVG_LEAN").toDouble());
    out.insert("NET_AVG_MASS", query.value("NET_AVG_MASS").toDouble());
    out.insert("NET_AVG_PFAT", query.value("NET_AVG_PFAT").toDouble());

    out.insert("GLOBAL_FAT",  query.value("GLOBAL_FAT").toDouble());
    out.insert("GLOBAL_LEAN", query.value("GLOBAL_LEAN").toDouble());
    out.insert("GLOBAL_MASS", query.value("GLOBAL_MASS").toDouble());
    out.insert("GLOBAL_PFAT", query.value("GLOBAL_PFAT").toDouble());

    out.insert("NO_REGIONS", query.value("NO_REGIONS").toLongLong());

    out.insert("TISSUE_ANALYSIS_METHOD", query.value("TISSUE_ANALYSIS_METHOD").toInt());
    out.insert("PHYSICIAN_COMMENT", query.value("PHYSICIAN_COMMENT").toString());

    out.insert("REG1_NAME", query.value("REG1_NAME").toString());
    out.insert("REG1_FAT",  query.value("REG1_FAT").toDouble());
    out.insert("REG1_LEAN", query.value("REG1_LEAN").toDouble());
    out.insert("REG1_MASS", query.value("REG1_MASS").toDouble());
    out.insert("REG1_PFAT", query.value("REG1_PFAT").toDouble());

    out.insert("REG2_NAME", query.value("REG2_NAME").toString());
    out.insert("REG2_FAT",  query.value("REG2_FAT").toDouble());
    out.insert("REG2_LEAN", query.value("REG2_LEAN").toDouble());
    out.insert("REG2_MASS", query.value("REG2_MASS").toDouble());
    out.insert("REG2_PFAT", query.value("REG2_PFAT").toDouble());

    out.insert("REG3_NAME", query.value("REG3_NAME").toString());
    out.insert("REG3_FAT",  query.value("REG3_FAT").toDouble());
    out.insert("REG3_LEAN", query.value("REG3_LEAN").toDouble());
    out.insert("REG3_MASS", query.value("REG3_MASS").toDouble());
    out.insert("REG3_PFAT", query.value("REG3_PFAT").toDouble());

    out.insert("REG4_NAME", query.value("REG4_NAME").toString());
    out.insert("REG4_FAT",  query.value("REG4_FAT").toDouble());
    out.insert("REG4_LEAN", query.value("REG4_LEAN").toDouble());
    out.insert("REG4_MASS", query.value("REG4_MASS").toDouble());
    out.insert("REG4_PFAT", query.value("REG4_PFAT").toDouble());

    out.insert("REG5_NAME", query.value("REG5_NAME").toString());
    out.insert("REG5_FAT",  query.value("REG5_FAT").toDouble());
    out.insert("REG5_LEAN", query.value("REG5_LEAN").toDouble());
    out.insert("REG5_MASS", query.value("REG5_MASS").toDouble());
    out.insert("REG5_PFAT", query.value("REG5_PFAT").toDouble());

    out.insert("REG6_NAME", query.value("REG6_NAME").toString());
    out.insert("REG6_FAT",  query.value("REG6_FAT").toDouble());
    out.insert("REG6_LEAN", query.value("REG6_LEAN").toDouble());
    out.insert("REG6_MASS", query.value("REG6_MASS").toDouble());
    out.insert("REG6_PFAT", query.value("REG6_PFAT").toDouble());

    out.insert("REG7_NAME", query.value("REG7_NAME").toString());
    out.insert("REG7_FAT",  query.value("REG7_FAT").toDouble());
    out.insert("REG7_LEAN", query.value("REG7_LEAN").toDouble());
    out.insert("REG7_MASS", query.value("REG7_MASS").toDouble());
    out.insert("REG7_PFAT", query.value("REG7_PFAT").toDouble());

    out.insert("REG8_NAME", query.value("REG8_NAME").toString());
    out.insert("REG8_FAT",  query.value("REG8_FAT").toDouble());
    out.insert("REG8_LEAN", query.value("REG8_LEAN").toDouble());
    out.insert("REG8_MASS", query.value("REG8_MASS").toDouble());
    out.insert("REG8_PFAT", query.value("REG8_PFAT").toDouble());

    out.insert("REG9_NAME", query.value("REG9_NAME").toString());
    out.insert("REG9_FAT",  query.value("REG9_FAT").toDouble());
    out.insert("REG9_LEAN", query.value("REG9_LEAN").toDouble());
    out.insert("REG9_MASS", query.value("REG9_MASS").toDouble());
    out.insert("REG9_PFAT", query.value("REG9_PFAT").toDouble());

    out.insert("REG10_NAME", query.value("REG10_NAME").toString());
    out.insert("REG10_FAT",  query.value("REG10_FAT").toDouble());
    out.insert("REG10_LEAN", query.value("REG10_LEAN").toDouble());
    out.insert("REG10_MASS", query.value("REG10_MASS").toDouble());
    out.insert("REG10_PFAT", query.value("REG10_PFAT").toDouble());

    out.insert("REG11_NAME", query.value("REG11_NAME").toString());
    out.insert("REG11_FAT",  query.value("REG11_FAT").toDouble());
    out.insert("REG11_LEAN", query.value("REG11_LEAN").toDouble());
    out.insert("REG11_MASS", query.value("REG11_MASS").toDouble());
    out.insert("REG11_PFAT", query.value("REG11_PFAT").toDouble());

    out.insert("REG12_NAME", query.value("REG12_NAME").toString());
    out.insert("REG12_FAT",  query.value("REG12_FAT").toDouble());
    out.insert("REG12_LEAN", query.value("REG12_LEAN").toDouble());
    out.insert("REG12_MASS", query.value("REG12_MASS").toDouble());
    out.insert("REG12_PFAT", query.value("REG12_PFAT").toDouble());

    out.insert("REG13_NAME", query.value("REG13_NAME").toString());
    out.insert("REG13_FAT",  query.value("REG13_FAT").toDouble());
    out.insert("REG13_LEAN", query.value("REG13_LEAN").toDouble());
    out.insert("REG13_MASS", query.value("REG13_MASS").toDouble());
    out.insert("REG13_PFAT", query.value("REG13_PFAT").toDouble());

    out.insert("REG14_NAME", query.value("REG14_NAME").toString());
    out.insert("REG14_FAT",  query.value("REG14_FAT").toDouble());
    out.insert("REG14_LEAN", query.value("REG14_LEAN").toDouble());
    out.insert("REG14_MASS", query.value("REG14_MASS").toDouble());
    out.insert("REG14_PFAT", query.value("REG14_PFAT").toDouble());
}


void ApexDatabaseHelper::wBodyObesityIndicesQuery(const QSqlDatabase& db, const QString& patientKey, const QString& scanId, QJsonObject& out) {
    QSqlQuery query(db);
    query.prepare("SELECT * FROM ObesityIndices WHERE PATIENT_KEY = :patientKey AND SCANID = :scanId");
    query.bindValue(":patientKey", patientKey);
    query.bindValue(":scanId", scanId);

    if (!query.exec()) {
        qDebug() << query.lastError().text() << patientKey << scanId;
        throw QException();
    }

    if (!query.first()) {
        qDebug() << "wBodyObesityIndicesQuery: not found" << patientKey << scanId;
        return;
    }

    out.insert("FAT_STD", query.value("FAT_STD").toDouble());
    out.insert("LEAN_STD", query.value("LEAN_STD").toDouble());
    out.insert("BRAIN_FAT", query.value("BRAIN_FAT").toDouble());
    out.insert("WATER_LBM", query.value("WATER_LBM").toDouble());
    out.insert("TOTAL_PERCENT_FAT", query.value("TOTAL_PERCENT_FAT").toDouble());
    out.insert("BODY_MASS_INDEX", query.value("BODY_MASS_INDEX").toDouble());

    out.insert("ANDROID_GYNOID_RATIO", query.value("ANDROID_GYNOID_RATIO").toDouble());
    out.insert("ANDROID_PERCENT_FAT", query.value("ANDROID_PERCENT_FAT").toDouble());
    out.insert("GYNOID_PERCENT_FAT", query.value("GYNOID_PERCENT_FAT").toDouble());

    out.insert("FAT_MASS", query.value("FAT_MASS").toDouble());
    out.insert("TRUNK_LIMB_FAT_MASS_RATIO", query.value("TRUNK_LIMB_FAT_MASS_RATIO").toDouble());
    out.insert("FAT_MASS_HEIGHT_SQUARED", query.value("FAT_MASS_HEIGHT_SQUARED").toDouble());
    out.insert("TOTAL_FAT_MASS", query.value("TOTAL_FAT_MASS").toDouble());

    out.insert("LEAN_MASS_HEIGHT_SQUARED", query.value("LEAN_MASS_HEIGHT_SQUARED").toDouble());
    out.insert("APPENDAGE_LEAN_MASS_HEIGHT_2", query.value("APPENDAGE_LEAN_MASS_HEIGHT_2").toDouble());
    out.insert("TOTAL_LEAN_MASS", query.value("TOTAL_LEAN_MASS").toDouble());

    out.insert("PHYSICIAN_COMMENT", query.value("PHYSICIAN_COMMENT").toString());
}


void ApexDatabaseHelper::wBodyAndroidGynoidCompositionQuery(const QSqlDatabase& db, const QString& patientKey, const QString& scanId, QJsonObject& out) {
    QSqlQuery query(db);

    query.prepare("SELECT * FROM AndroidGynoidComposition WHERE PATIENT_KEY = :patientKey AND SCANID = :scanId");
    query.bindValue(":patientKey", patientKey);
    query.bindValue(":scanId", scanId);

    if (!query.exec()) {
        qDebug() << query.lastError().text() << patientKey << scanId;
        throw QException();
    }

    if (!query.first()) {
        qDebug() << "wBodyAndroidGynoidCompositionQuery: " << patientKey << scanId;
        return;
    }

    out.insert("ANDROID_FAT",  query.value("ANDROID_FAT").toDouble());
    out.insert("ANDROID_LEAN", query.value("ANDROID_LEAN").toDouble());

    out.insert("GYNOID_FAT",   query.value("GYNOID_FAT").toDouble());
    out.insert("GYNOID_LEAN",  query.value("GYNOID_LEAN").toDouble());

    out.insert("PHYSICIAN_COMMENT", query.value("PHYSICIAN_COMMENT").toString());
}

void ApexDatabaseHelper::forearmQuery(const QSqlDatabase& db, const QString& patientKey, const QString& scanId, QJsonObject& out) {
    QSqlQuery query(db);

    query.prepare("SELECT * FROM Forearm WHERE PATIENT_KEY = :patientKey AND SCANID = :scanId");
    query.bindValue(":patientKey", patientKey);
    query.bindValue(":scanId", scanId);

    if (!query.exec()) {
        qDebug() << "forearmQuery: " << patientKey << scanId;
        throw QException();
    }

    if (!query.first()) {
        return;
    }

    out.insert("R_13_AREA", query.value("R_13_AREA").toDouble());
    out.insert("R_13_BMC",  query.value("R_13_BMC").toDouble());
    out.insert("R_13_BMD",  query.value("R_13_BMD").toDouble());

    out.insert("R_MID_AREA", query.value("R_MID_AREA").toDouble());
    out.insert("R_MID_BMC",  query.value("R_MID_BMC").toDouble());
    out.insert("R_MID_BMD",  query.value("R_MID_BMD").toDouble());

    out.insert("R_UD_AREA",  query.value("R_UD_AREA").toDouble());
    out.insert("R_UD_BMC",   query.value("R_UD_BMC").toDouble());
    out.insert("R_UD_BMD",   query.value("R_UD_BMD").toDouble());

    out.insert("U_13_AREA",  query.value("U_13_AREA").toDouble());
    out.insert("U_13_BMC",   query.value("U_13_BMC").toDouble());
    out.insert("U_13_BMD",   query.value("U_13_BMD").toDouble());

    out.insert("U_MID_AREA", query.value("U_MID_AREA").toDouble());
    out.insert("U_MID_BMC",  query.value("U_MID_BMC").toDouble());
    out.insert("U_MID_BMD",  query.value("U_MID_BMD").toDouble());

    out.insert("U_UD_AREA",  query.value("U_UD_AREA").toDouble());
    out.insert("U_UD_BMC",   query.value("U_UD_BMC").toDouble());
    out.insert("U_UD_BMD",   query.value("U_UD_BMD").toDouble());

    out.insert("RTOT_AREA",  query.value("RTOT_AREA").toDouble());
    out.insert("RTOT_BMC",   query.value("RTOT_BMC").toDouble());
    out.insert("RTOT_BMD",   query.value("RTOT_BMD").toDouble());

    out.insert("UTOT_AREA",  query.value("UTOT_AREA").toDouble());
    out.insert("UTOT_BMC",   query.value("UTOT_BMC").toDouble());
    out.insert("UTOT_BMD",   query.value("UTOT_BMD").toDouble());

    out.insert("RU13TOT_AREA", query.value("RU13TOT_AREA").toDouble());
    out.insert("RU13TOT_BMC",  query.value("RU13TOT_BMC").toDouble());
    out.insert("RU13TOT_BMD",  query.value("RU13TOT_BMD").toDouble());

    out.insert("RUMIDTOT_AREA", query.value("RUMIDTOT_AREA").toDouble());
    out.insert("RUMIDTOT_BMC",  query.value("RUMIDTOT_BMC").toDouble());
    out.insert("RUMIDTOT_BMD",  query.value("RUMIDTOT_BMD").toDouble());

    out.insert("RUUDTOT_AREA", query.value("RUUDTOT_AREA").toDouble());
    out.insert("RUUDTOT_BMC",  query.value("RUUDTOT_BMC").toDouble());
    out.insert("RUUDTOT_BMD",  query.value("RUUDTOT_BMD").toDouble());

    out.insert("RUUDTOT_BMD", query.value("RUUDTOT_BMD").toDouble());
    out.insert("RUTOT_BMC",   query.value("RUTOT_BMC").toDouble());
    out.insert("RUTOT_BMD",   query.value("RUTOT_BMD").toDouble());

    out.insert("ROI_TYPE",    query.value("ROI_TYPE").toDouble());
    out.insert("ROI_WIDTH",   query.value("ROI_WIDTH").toDouble());
    out.insert("ROI_HEIGHT",  query.value("ROI_HEIGHT").toDouble());

    out.insert("ARM_LENGTH",        query.value("ARM_LENGTH").toDouble());
    out.insert("PHYSICIAN_COMMENT", query.value("PHYSICIAN_COMMENT").toString());
}

void ApexDatabaseHelper::computeTZValues(const Scan& scan, const QDateTime& scanDate, const QJsonObject& inputData, QJsonObject& out, const QSqlDatabase& db)
{
    /*
     * INPUT
     * refType
     * sex
     * ethnicity
     * method
     * ageYoung (x-value)
     * curveId
     * UNIQUE_ID
     * AGE_YOUNG
     * X_VALUE
     * Y_VALUE
     * L_VALUE
     * STD
    */

    // AP lumbar spine:
    // - identify the included vertebral levels
    // - sum the area and sum the bmc of the included vertebral levels
    // - compute the revised total bmd from summed bmc / summed area
    // - provide the proper bone range code for total bmd

    QMap<QString, double> bmdData;
    QString prefix;
    QString source;

    if (scan.refType == "S") {
        bool included_array[] {false, false, false, false};

        double area_array[] {0.0, 0.0, 0.0, 0.0};
        double bmc_array[] {0.0, 0.0, 0.0, 0.0};
        double tot_bmd = 0.0;

        for (QJsonObject::const_iterator it = out.begin(); it != out.end(); ++it) {
            QString key = it.key();
            QJsonValue value = it.value();

            int index = -1;

            qDebug() << "Key:" << key << ", Value:" << value;

            if (key.startsWith("L1"))
                index = 0;
            else if (key.startsWith("L2"))
                index = 1;
            else if (key.startsWith("L3"))
                index = 2;
            else if (key.startsWith("L4"))
                index = 3;

            if (index != -1) {
                if (key.endsWith("_INCLUDED"))
                    included_array[index] = value.toBool();
                else if (key.endsWith("_AREA"))
                    area_array[index] = value.toDouble();
                else if (key.endsWith("_BMC"))
                    bmc_array[index] = value.toDouble();
            }

            if (key.endsWith("_BMD")) {
                key = key.replace(prefix, "");
                if (key == "TOT_BMD")
                    tot_bmd = value.toDouble();
                else {
                    if (ranges.contains(key)) {
                        bmdData.insert(key, value.toDouble());
                    }
                }
            }
        }

        double tot_area = 0.0;
        double tot_bmc = 0.0;

        for (int i = 0; i < 4; i++) {
            if (included_array[i]) {
                tot_area += area_array[i];
                tot_bmc += bmc_array[i];
            }
        }

        if (0.0 != tot_area) {
            double last_tot_bmd = tot_bmd;
            tot_bmd = tot_bmc / tot_area;

            qDebug() << "updating ap lumbar spine total bmd from " + QString::number(last_tot_bmd) + " to " + QString::number(tot_bmd);
        }


        QString tot_key = "TOT_BMD";
        if( included_array[0] && !(included_array[1] || included_array[2] || included_array[3]))
            //_bonerange="1..."
            tot_key="TOT_L1_BMD";
        else if( included_array[1] && !(included_array[0] || included_array[2] || included_array[3]))
            // bonerange=".2.."
            tot_key="TOT_L2_BMD";
        else if( included_array[2] && !(included_array[0] || included_array[1] || included_array[3]))
            // bonerange="..3."
            tot_key="TOT_L3_BMD";
        else if( included_array[3] && !(included_array[0] || included_array[1] || included_array[2]))
            // bonerange="...4"
            tot_key="TOT_L4_BMD";
        else if( included_array[0] && included_array[1] && !(included_array[2] || included_array[3]))
            // bonerange="12.."
            tot_key="TOT_L1L2_BMD";
        else if( included_array[0] && included_array[2] && !(included_array[1] || included_array[3]))
            // bonerange="1.3."
            tot_key="TOT_L1L3_BMD";
        else if( included_array[0] && included_array[3] && !(included_array[1] || included_array[2]))
            // bonerange="1..4"
            tot_key="TOT_L1L4_BMD";
        else if( included_array[1] && included_array[2] && !(included_array[0] || included_array[3]))
            // bonerange=".23."
            tot_key="TOT_L2L3_BMD";
        else if( included_array[1] && included_array[3] && !(included_array[0] || included_array[2]))
            // bonerange=".2.4"
            tot_key="TOT_L2L4_BMD";
        else if( included_array[2] && included_array[3] && !(included_array[0] || included_array[1]))
            // bonerange="..34"
            tot_key="TOT_L3L4_BMD";
        else if( included_array[0] && included_array[1] && included_array[2] && !included_array[3])
            // bonerange="123."
            tot_key="TOT_L1L2L3_BMD";
        else if( included_array[0] && included_array[1] && included_array[3] && !included_array[2])
            // bonerange="12.4"
            tot_key="TOT_L1L2L4_BMD";
        else if( included_array[0] && included_array[2] && included_array[3] && !included_array[1])
            // bonerange="1.34"
            tot_key="TOT_L1L3L4_BMD";
        else if( included_array[1] && included_array[2] && included_array[3] && !included_array[0])
            // bonerange=".234"
            tot_key="TOT_L2L3L4_BMD";
        else
            // bonerange="1234"
            tot_key="TOT_BMD";

        if (ranges.contains(tot_key)) {
            bmdData.insert(tot_key, tot_bmd);
            qDebug() << "ranges contains key: " + tot_key;
        }

    } else {
        for (QJsonObject::const_iterator it = out.begin(); it != out.end(); ++it) {
            QString key = it.key();
            QJsonValue value = it.value();

            if (key.endsWith("_BMD")) {
                qDebug() << "key pre: " + key + ", new key: " + key.replace(prefix, "");
                key = key.replace(prefix, "");
                if (ranges.contains(key)) {
                    bmdData.insert(key, value.toDouble());
                    qDebug() << "ranges contains key: " << key;
                }
            }
        }
    }

    qDebug() << prefix + " data contains: " << QString::number(out.size()) + " possible entries to get bmd values from";
    qDebug() << prefix + " bmddata contains: " << QString::number(bmdData.size()) + " entries to get tz";

    AgeBracket bracket;
    double age {};

    age = computeYearsDifference(scanDate, QDateTime::fromString(inputData.value("dob").toString()));

    qDebug() << "computed age from scandate and dob: " + QString::number(age);

    QSqlQuery query(db);

    QMap<QString, double>::iterator i;
    for (i = bmdData.begin(); i != bmdData.end(); ++i) {
        QString bmdBoneRangeKey  = i.key();
        double bmdValue = i.value();

        qDebug() << "working on range key: " << bmdBoneRangeKey + "with value: " + QString::number(bmdValue);

        QString method = " AND METHOD IS NULL";
        if (scan.refType == "S" && (bmdBoneRangeKey.contains("L1_") || bmdBoneRangeKey.contains("L4_"))) {
            method = " AND METHOD = 'APEX'";
        }

        QString sql = "SELECT UNIQUE_ID, AGE_YOUNG FROM ReferenceCurve";
        sql += " WHERE REFTYPE = '" + scan.refType + "'";
        sql += " AND IF_CURRENT = 1 AND SEX = 'F' AND ETHNIC IS NULL";
        sql += method;
        sql += " AND SOURCE LIKE '%" + source + "%'";
        sql += " AND Y_LABEL = 'IDS_REF_LBL_BMD'";
        sql += " AND BONERANGE ";
        sql += (ranges.value(bmdBoneRangeKey) == "NULL" ? ("IS NULL") : ("= '" + ranges.value(bmdBoneRangeKey).toString() + "'"));

        qDebug() << "first query (t score): " + sql;

        // do query
        query.prepare(sql);
        query.exec();

        QString curveId = query.value("UNIQUE_ID").toString();
        double ageYoung = query.value("AGE_YOUNG").toDouble();


        qDebug() << "second query (T score): " + sql;

        sql = "SELECT Y_VALUE, L_VALUE, STD FROM Points WHERE UNIQUE_ID = " + curveId;
        sql += " AND X_VALUE = " + QString::number(ageYoung);

        query.prepare(sql);
        if (!query.exec()) {
            qDebug() << query.lastError().text();
            throw QException();
        }

        query.first();

        QList<double> bmdValues;
        bmdValues.append(query.value("Y_VALUE").toDouble());
        bmdValues.append(query.value("L_VALUE").toDouble());
        bmdValues.append(query.value("STD").toDouble());

        double X_value = bmdValue;
        double M_value = bmdValues.at(0);
        double L_value = bmdValues.at(1);
        double sigma = bmdValues.at(2);

        double T_score = M_value * (pow(X_value / M_value, L_value) - 1.0) / (L_value * sigma);
        T_score = qRound(T_score);

        if (0.0 == abs(T_score)) T_score = 0.;

        QString varName = scan.name + "_";
        if (scan.refType == "S" && bmdBoneRangeKey.startsWith("TOT_"))
            varName += "TOT_T";
        else
            varName += bmdBoneRangeKey.replace("_BMD", "_T");

        if (out.contains(varName))
            throw QException();

        out.insert(varName, T_score);

        double Z_score {};
        varName = scan.name + "_";

        if (scan.refType == "S" && bmdBoneRangeKey.startsWith("TOT_"))
            varName += "TOT_Z";
        else
            varName += bmdBoneRangeKey.replace("_BMD", "_Z");

        if (out.contains(varName))
            throw QException();

        QString sex = inputData.value("sex").toString();
        sex = sex.toUpper();

        if (0 == sex.length() || sex.startsWith("F"))
            sex = " AND SEX = 'F'";
        else if (sex.startsWith("M")) {
            if (bmdBoneRangeKey == "U_UD_BMD") {
                out.insert(varName, 0.0);
                continue;
            }
            sex = " AND SEX = 'M'";
        }

        QString ethnicity = inputData.value("ethnicity").toString();
        if (ethnicity.isNull()) ethnicity = "";
        ethnicity = ethnicity.toUpper();

        if (0 == ethnicity.length() || ethnicity == "W" || ethnicity == "O" || ethnicity == "P" || ethnicity == "I" ||
            (scan.refType == "R" && (ethnicity == "H" || ethnicity == "B")))
            ethnicity = " AND ETHNIC IS NULL";
        else
            ethnicity = " AND ETHNIC = '" + ethnicity + "'";

        sql = "SELECT UNIQUE_ID, AGE_YOUNG FROM ReferenceCurve";
        sql += " WHERE REFTYPE = '" + scan.refType + "'";
        sql += " AND IF_CURRENT = 1";
        sql += sex ;
        sql += ethnicity;
        sql += method;
        sql += " AND SOURCE LIKE '%" + scan.refSource + "'";
        sql += " AND Y_LABEL = 'IDS_REF_LBL_BMD'";
        sql += " AND BONERANGE ";
        sql += (ranges.value(bmdBoneRangeKey).toString() == "NULL" ? ("IS NULL") : ("= '" + ranges.value(bmdBoneRangeKey).toString() + "'"));

        qDebug() << "first query (z score): " + sql;

        query.prepare(sql);
        if (!query.exec()) {
            qDebug() << query.lastError().text();
            throw QException();
        }
        query.first();
        curveId = query.value("UNIQUE_ID").toString();

        // Determine the age values (X axis variable) of the curve
        //
        sql = "SELECT X_VALUE FROM Points WHERE UNIQUE_ID = " + curveId;
        qDebug() << "second query (z score): " + sql;
        query.prepare(sql);
        if (!query.exec()) {
            qDebug() << query.lastError().text();
            throw QException();
        }

        QList<double> ageTable;
        while (query.next())
            ageTable.append(query.value("X_VALUE").toDouble());

        bracket.compute(age, ageTable);
        if (0.0 != bracket.ageSpan) {
            // Determine the bmd, skewness factor and standard deviation
            // at the bracketing and peak bmd age values.
            //
            sql = "SELECT Y_VALUE, L_VALUE, STD FROM Points WHERE UNIQUE_ID = " + curveId;
            sql += " AND X_VALUE = ";

            QList<double> x_value_array { bracket.ageMin, bracket.ageMax };

            bmdValues.clear();

            for (int i = 0; i < x_value_array.count(); i++) {
                qDebug() << "third query (z score) iter: " + QString::number(i) + " : " + sql + QString::number(x_value_array[i]);;

                query.prepare(sql + QString::number(x_value_array[i]));
                if (query.exec()) {
                    qDebug() << query.lastError().text();
                    throw QException();
                }

                query.first();

                bmdValues.append(query.value("Y_VALUE").toDouble());
                bmdValues.append(query.value("L_VALUE").toDouble());
                bmdValues.append(query.value("STD").toDouble());
            }

            double u = (age - bracket.ageMin) / bracket.ageSpan;
            QList<double> interpValues;
            for (int i = 0; i < bmdValues.size() / 2; i++)
                interpValues.append((1.0 - u) * bmdValues.at(i) + u * bmdValues.at(i + 3));

            M_value = interpValues.at(0);
            L_value = interpValues.at(1);
            sigma = interpValues.at(2);

            Z_score = M_value * (pow(X_value / M_value, L_value) - 1.0) / (L_value * sigma);
            Z_score = qRound(Z_score);
            if (0.0 == abs(Z_score)) Z_score = 0.0;
        }

        out.insert(varName, Z_score);

        qDebug() << varName + " = " << QString::number(Z_score);
        qDebug() << "finished current key: " << bmdBoneRangeKey;
    }
}

double ApexDatabaseHelper::computeYearsDifference(const QDateTime& first, const QDateTime& second)
{
    Q_UNUSED(first)
    Q_UNUSED(second)
    //double diff = first.time().msec() - second;
    return 0.0;
}

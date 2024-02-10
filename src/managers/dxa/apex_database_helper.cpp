#include "apex_database_helper.h"
#include <QSqlDatabase>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QException>
#include <QJsonObject>

QVariantMap ApexDatabaseHelper::extractScanAnalysis(const QSqlDatabase& db, const QString &patientKey,
                                                    const QString &scanType)
{
    Q_UNUSED(patientKey)
    Q_UNUSED(scanType)

    QVariantMap results;
    return results;
}

QSharedPointer<QJsonObject> spineScanQuery(QString patientKey, QString scanId) {
    QSqlQuery query;

    // Spine
    query.prepare("SELECT * FROM Spine WHERE PATIENT_KEY = :patientKey AND SCANID = :scanId");
    query.bindValue(":patientKey", patientKey);
    query.bindValue(":scanId", scanId);

    if (!query.exec()) {
        qDebug() << "spine query failed" << query.lastError().text();
        throw QException();
    }

    long NO_REGIONS;
    long STARTING_REGION;

    bool L1_INCLUDED;
    double L1_AREA;
    double L1_BMC;
    double L1_BMD;

    bool L2_INCLUDED;
    double L2_AREA;
    double L2_BMC;
    double L2_BMD;

    bool L3_INCLUDED;
    double L3_AREA;
    double L3_BMC;
    double L3_BMD;

    bool L4_INCLUDED;
    double L4_AREA;
    double L4_BMC;
    double L4_BMD;

    double TOT_AREA;
    double TOT_BMC;
    double TOT_BMD;
    double STD_TOT_BMD;

    long ROI_TYPE;
    double ROI_WIDTH;
    double ROI_HEIGHT;
    QString PHYSICIAN_COMMENT;
}

QSharedPointer<QJsonObject> hipScanQuery() {
    // Hip
    QSqlQuery query;
    query.prepare("SELECT * FROM Hip WHERE PATIENT_KEY = :patientKey AND SCANID = :scanId");
    if (!query.exec()) {
        qDebug() << "hip query failed: " << query.lastError().text();
        throw QException();
    }

    auto results = QSharedPointer<QJsonObject>::create();

    results->insert("TROCH_AREA", query.value("TROCH_AREA").toDouble());
    results->insert("TROCH_BMC",  query.value("TROCH_BMC").toDouble());
    results->insert("TROCH_BMD",  query.value("TROCH_BMD").toDouble());

    results->insert("INTER_AREA", query.value("INTER_AREA").toDouble());
    results->insert("INTER_BMC",  query.value("INTER_BMC").toDouble());
    results->insert("INTER_BMD",  query.value("INTER_BMD").toDouble());

    results->insert("NECK_AREA",  query.value("NECK_AREA").toDouble());
    results->insert("NECK_BMC",   query.value("NECK_BMC").toDouble());
    results->insert("NECK_BMD",   query.value("NECK_BMD").toDouble());

    results->insert("WARDS_AREA", query.value("WARDS_AREA").toDouble());
    results->insert("WARDS_BMC",  query.value("WARDS_BMC").toDouble());
    results->insert("WARDS_BMD",  query.value("WARDS_BMD").toDouble());

    results->insert("HTOT_AREA",  query.value("HTOT_AREA").toDouble());
    results->insert("HTOT_BMC",   query.value("HTOT_BMC").toDouble());
    results->insert("HTOT_BMD",   query.value("HTOT_BMD").toDouble());

    results->insert("ROI_TYPE",   query.value("ROI_TYPE").toLongLong());
    results->insert("ROI_WIDTH",  query.value("ROI_WIDTH").toDouble());
    results->insert("ROI_HEIGHT", query.value("ROI_HEIGHT").toDouble());

    results->insert("AXIS_LENGTH",       query.value("AXIS_LENGTH").toDouble());
    results->insert("PHYSICIAN_COMMENT", query.value("PHYSICIAN_COMMENT").toString());

    return results;
}

QSharedPointer<QJsonObject> hipHsaQuery() {
    QSqlQuery query;
    query.prepare("SELECT * FROM HipHSA WHERE PATIENT_KEY = :patientKey AND SCANID = :scanId");
    if (!query.exec()) {
        qDebug() << "hipHSA query failed" << query.lastError().text();
        throw QException();
    }

    auto results = QSharedPointer<QJsonObject>::create();

    results->insert("NN_BMD",      query.value("NN_BMD").toDouble());
    results->insert("NN_CSA",      query.value("NN_CSA").toDouble());
    results->insert("NN_CSMI",     query.value("NN_CSMI").toDouble());
    results->insert("NN_WIDTH",    query.value("NN_WIDTH").toDouble());
    results->insert("NN_ED",       query.value("NN_ED").toDouble());
    results->insert("NN_ACT",      query.value("NN_ACT").toDouble());
    results->insert("NN_PCD",      query.value("NN_PCD").toDouble());
    results->insert("NN_CMP",      query.value("NN_CMP").toDouble());
    results->insert("NN_SECT_MOD", query.value("NN_SECT_MOD").toDouble());
    results->insert("NN_BR", 	   query.value("NN_BR").toDouble());

    results->insert("IT_BMD",      query.value("IT_BMD").toDouble());
    results->insert("IT_CSA",      query.value("IT_CSA").toDouble());
    results->insert("IT_CSMI",     query.value("IT_CSMI").toDouble());
    results->insert("IT_WIDTH",    query.value("IT_WIDTH").toDouble());
    results->insert("IT_ED",       query.value("IT_ED").toDouble());
    results->insert("IT_ACT",      query.value("IT_ACT").toDouble());
    results->insert("IT_PCD",      query.value("IT_PCD").toDouble());
    results->insert("IT_CMP",      query.value("IT_CMP").toDouble());
    results->insert("IT_SECT_MOD", query.value("IT_SECT_MOD").toDouble());
    results->insert("IT_BR",       query.value("IT_BR").toDouble());

    results->insert("FS_BMD",      query.value("FS_BMD").toDouble());
    results->insert("FS_CSA",      query.value("FS_CSA").toDouble());
    results->insert("FS_CSMI",     query.value("FS_CSMI").toDouble());
    results->insert("FS_WIDTHS",   query.value("FS_WIDTHS").toDouble());
    results->insert("FS_ED", 	   query.value("FS_ED").toDouble());
    results->insert("FS_ACT",      query.value("FS_ACT").toDouble());
    results->insert("FS_PCD",      query.value("FS_PCD").toDouble());
    results->insert("FS_CMP",      query.value("FS_CMP").toDouble());
    results->insert("FS_SECT_MOD", query.value("FS_SECT_MOD").toDouble());
    results->insert("FS_BR",       query.value("FS_BR").toDouble());

    results->insert("SHAFT_NECK_ANGLE", query.value("SHAFT_NECK_ANGLE").toDouble());

    return results;
}

QSharedPointer<QJsonObject> wBodyQuery() {
    // Whole body
    QSqlQuery query;
    query.prepare("SELECT * FROM Wbody WHERE PATIENT_KEY = :patientKey AND SCANID = :scanId");
    if (!query.exec()) {
        qDebug() << query.lastError().text();
        throw QException();
    }

    auto results = QSharedPointer<QJsonObject>::create();

    results->insert("WBTOT_AREA", query.value("WBTOT_AREA").toDouble());
    results->insert("WBTOT_BMC",  query.value("WBTOT_BMC").toDouble());
    results->insert("WBTOT_BMD",  query.value("WBTOT_BMD").toDouble());

    results->insert("SUBTOT_AREA", query.value("SUBTOT_AREA").toDouble());
    results->insert("SUBTOT_BMC",  query.value("SUBTOT_BMC").toDouble());
    results->insert("SUBTOT_BMD",  query.value("SUBTOT_BMD").toDouble());

    results->insert("HEAD_AREA", query.value("HEAD_AREA").toDouble());
    results->insert("HEAD_BMC",  query.value("HEAD_BMC").toDouble());
    results->insert("HEAD_BMD",  query.value("HEAD_BMD").toDouble());

    results->insert("LARM_AREA", query.value("LARM_AREA").toDouble());
    results->insert("LARM_BMC",  query.value("LARM_BMC").toDouble());
    results->insert("LARM_BMD",  query.value("LARM_BMD").toDouble());

    results->insert("RARM_AREA", query.value("RARM_AREA").toDouble());
    results->insert("RARM_BMC",  query.value("RARM_BMC").toDouble());
    results->insert("RARM_BMD",  query.value("RARM_BMD").toDouble());

    results->insert("LRIB_AREA", query.value("LRIB_AREA").toDouble());
    results->insert("LRIB_BMC",  query.value("LRIB_BMC").toDouble());
    results->insert("LRIB_BMD",  query.value("LRIB_BMD").toDouble());

    results->insert("RRIB_AREA", query.value("RRIB_AREA").toDouble());
    results->insert("RRIB_BMC",  query.value("RRIB_BMC").toDouble());
    results->insert("RRIB_BMD",  query.value("RRIB_BMD").toDouble());

    results->insert("T_S_AREA",  query.value("T_S_AREA").toDouble());
    results->insert("T_S_BMC",   query.value("T_S_BMC").toDouble());
    results->insert("T_S_BMD",   query.value("T_S_BMD").toDouble());
    results->insert("L_S_AREA",  query.value("L_S_AREA").toDouble());
    results->insert("L_S_BMC",   query.value("L_S_BMC").toDouble());
    results->insert("L_S_BMD",   query.value("L_S_BMD").toDouble());

    results->insert("PELV_AREA", query.value("PELV_AREA").toDouble());
    results->insert("PELV_BMC",  query.value("PELV_BMC").toDouble());
    results->insert("PELV_BMD",  query.value("PELV_BMD").toDouble());

    results->insert("LLEG_AREA", query.value("LLEG_AREA").toDouble());
    results->insert("LLEG_BMC",  query.value("LLEG_BMC").toDouble());
    results->insert("LLEG_BMD",  query.value("LLEG_BMD").toDouble());

    results->insert("RLEG_AREA", query.value("RLEG_AREA").toDouble());
    results->insert("RLEG_BMC",  query.value("RLEG_BMC").toDouble());
    results->insert("RLEG_BMD",  query.value("RLEG_BMD").toDouble());

    results->insert("PHYSICIAN_COMMENT", query.value("PHYSICIAN_COMMENT").toString());

    return results;
}

QSharedPointer<QJsonObject> wBodyCompositionQuery() {
    QSqlQuery query;
    query.prepare("SELECT * FROM WbodyComposition WHERE PATIENT_KEY = :patientKey AND SCANID = :scanId");
    if (!query.exec()) {
        qDebug() << query.lastError().text();
        throw QException();
    }

    auto results = QSharedPointer<QJsonObject>::create();

    results->insert("FAT_STD",   query.value("FAT_STD").toDouble());
    results->insert("LEAN_STD",  query.value("LEAN_STD").toDouble());
    results->insert("BRAIN_FAT", query.value("BRAIN_FAT").toDouble());
    results->insert("WATER_LBM", query.value("WATER_LBM").toDouble());

    results->insert("HEAD_FAT",  query.value("HEAD_FAT").toDouble());
    results->insert("HEAD_LEAN", query.value("HEAD_LEAN").toDouble());
    results->insert("HEAD_MASS", query.value("HEAD_MASS").toDouble());
    results->insert("HEAD_PFAT", query.value("HEAD_PFAT").toDouble());

    results->insert("LARM_FAT",  query.value("LARM_FAT").toDouble());
    results->insert("LARM_LEAN", query.value("LARM_LEAN").toDouble());
    results->insert("LARM_MASS", query.value("LARM_MASS").toDouble());
    results->insert("LARM_PFAT", query.value("LARM_PFAT").toDouble());

    results->insert("RARM_FAT",  query.value("RARM_FAT").toDouble());
    results->insert("RARM_LEAN", query.value("RARM_LEAN").toDouble());
    results->insert("RARM_MASS", query.value("RARM_MASS").toDouble());
    results->insert("RARM_PFAT", query.value("RARM_PFAT").toDouble());

    results->insert("TRUNK_FAT",  query.value("TRUNK_FAT").toDouble());
    results->insert("TRUNK_LEAN", query.value("TRUNK_LEAN").toDouble());
    results->insert("TRUNK_MASS", query.value("TRUNK_MASS").toDouble());
    results->insert("TRUNK_PFAT", query.value("TRUNK_PFAT").toDouble());

    results->insert("L_LEG_FAT",  query.value("L_LEG_FAT").toDouble());
    results->insert("L_LEG_LEAN", query.value("L_LEG_LEAN").toDouble());
    results->insert("L_LEG_MASS", query.value("L_LEG_MASS").toDouble());
    results->insert("L_LEG_PFAT", query.value("L_LEG_PFAT").toDouble());

    results->insert("R_LEG_FAT",  query.value("R_LEG_FAT").toDouble());
    results->insert("R_LEG_LEAN", query.value("R_LEG_LEAN").toDouble());
    results->insert("R_LEG_MASS", query.value("R_LEG_MASS").toDouble());
    results->insert("R_LEG_PFAT", query.value("R_LEG_PFAT").toDouble());

    results->insert("SUBTOT_FAT",  query.value("SUBTOT_FAT").toDouble());
    results->insert("SUBTOT_LEAN", query.value("SUBTOT_LEAN").toDouble());
    results->insert("SUBTOT_MASS", query.value("SUBTOT_MASS").toDouble());
    results->insert("SUBTOT_PFAT", query.value("SUBTOT_PFAT").toDouble());

    results->insert("WBTOT_FAT",  query.value("WBTOT_FAT").toDouble());
    results->insert("WBTOT_LEAN", query.value("WBTOT_LEAN").toDouble());
    results->insert("WBTOT_MASS", query.value("WBTOT_MASS").toDouble());
    results->insert("WBTOT_PFAT", query.value("WBTOT_PFAT").toDouble());

    results->insert("PHYSICIAN_COMMENT", query.value("PHYSICIAN_COMMENT").toString());

    return results;
}

QSharedPointer<QJsonObject> subRegionBoneQuery() {
    QSqlQuery query;
    query.prepare("SELECT * FROM SubRegionBone WHERE PATIENT_KEY = :patientKey AND SCANID = :scanId");

    if (query.exec()) {
        qDebug() << query.lastError().text();
        throw QException();
    }

    auto results = QSharedPointer<QJsonObject>::create();

    results->insert("NET_AVG_AREA", query.value("NET_AVG_AREA").toDouble());
    results->insert("NET_AVG_BMC",  query.value("NET_AVG_BMC").toDouble());
    results->insert("NET_AVG_BMD",  query.value("NET_AVG_BMD").toDouble());

    results->insert("GLOBAL_AREA", query.value("GLOBAL_AREA").toDouble());
    results->insert("GLOBAL_BMC",  query.value("GLOBAL_AREA").toDouble());
    results->insert("GLOBAL_BMD",  query.value("GLOBAL_AREA").toDouble());

    results->insert("NO_REGIONS",  query.value("NO_REGIONS").toLongLong());
    results->insert("PHYSICIAN_COMMENT", query.value("PHYSICIAN_COMMENT").toString());

    results->insert("REG1_NAME", query.value("REG1_NAME").toString());
    results->insert("REG1_AREA", query.value("REG1_AREA").toDouble());
    results->insert("REG1_BMC",  query.value("REG1_BMC").toDouble());
    results->insert("REG1_BMD",  query.value("REG1_BMD").toDouble());

    results->insert("REG2_NAME", query.value("REG2_NAME").toString());
    results->insert("REG2_AREA", query.value("REG2_AREA").toDouble());
    results->insert("REG2_BMC",  query.value("REG2_BMC").toDouble());
    results->insert("REG2_BMD",  query.value("REG2_BMD").toDouble());

    results->insert("REG3_NAME", query.value("REG3_NAME").toString());
    results->insert("REG3_AREA", query.value("REG3_AREA").toDouble());
    results->insert("REG3_BMC",  query.value("REG3_BMC").toDouble());
    results->insert("REG3_BMD",  query.value("REG3_BMD").toDouble());

    results->insert("REG3_NAME", query.value("REG3_NAME").toString());
    results->insert("REG3_AREA", query.value("REG3_AREA").toDouble());
    results->insert("REG3_BMC",  query.value("REG3_BMC").toDouble());
    results->insert("REG3_BMD",  query.value("REG3_BMD").toDouble());

    results->insert("REG4_NAME", query.value("REG4_NAME").toString());
    results->insert("REG4_AREA", query.value("REG4_AREA").toDouble());
    results->insert("REG4_BMC",  query.value("REG4_BMC").toDouble());
    results->insert("REG4_BMD",  query.value("REG4_BMD").toDouble());

    results->insert("REG5_NAME", query.value("REG5_NAME").toString());
    results->insert("REG5_AREA", query.value("REG5_AREA").toDouble());
    results->insert("REG5_BMC",  query.value("REG5_BMC").toDouble());
    results->insert("REG5_BMD",  query.value("REG5_BMD").toDouble());

    results->insert("REG6_NAME", query.value("REG6_NAME").toString());
    results->insert("REG6_AREA", query.value("REG6_AREA").toDouble());
    results->insert("REG6_BMC",  query.value("REG6_BMC").toDouble());
    results->insert("REG6_BMD",  query.value("REG6_BMD").toDouble());

    results->insert("REG7_NAME", query.value("REG7_NAME").toString());
    results->insert("REG7_AREA", query.value("REG7_AREA").toDouble());
    results->insert("REG7_BMC",  query.value("REG7_BMC").toDouble());
    results->insert("REG7_BMD",  query.value("REG7_BMD").toDouble());

    results->insert("REG8_NAME", query.value("REG8_NAME").toString());
    results->insert("REG8_AREA", query.value("REG8_AREA").toDouble());
    results->insert("REG8_BMC",  query.value("REG8_BMC").toDouble());
    results->insert("REG8_BMD",  query.value("REG8_BMD").toDouble());

    results->insert("REG9_NAME", query.value("REG9_NAME").toString());
    results->insert("REG9_AREA", query.value("REG9_AREA").toDouble());
    results->insert("REG9_BMC",  query.value("REG9_BMC").toDouble());
    results->insert("REG9_BMD",  query.value("REG9_BMD").toDouble());

    results->insert("REG10_NAME", query.value("REG10_NAME").toString());
    results->insert("REG10_AREA", query.value("REG10_AREA").toDouble());
    results->insert("REG10_BMC",  query.value("REG10_BMC").toDouble());
    results->insert("REG10_BMD",  query.value("REG10_BMD").toDouble());

    results->insert("REG11_NAME", query.value("REG11_NAME").toString());
    results->insert("REG11_AREA", query.value("REG11_AREA").toDouble());
    results->insert("REG11_BMC",  query.value("REG11_BMC").toDouble());
    results->insert("REG11_BMD",  query.value("REG11_BMD").toDouble());

    results->insert("REG12_NAME", query.value("REG12_NAME").toString());
    results->insert("REG12_AREA", query.value("REG12_AREA").toDouble());
    results->insert("REG12_BMC",  query.value("REG12_BMC").toDouble());
    results->insert("REG12_BMD",  query.value("REG12_BMD").toDouble());

    results->insert("REG13_NAME", query.value("REG13_NAME").toString());
    results->insert("REG13_AREA", query.value("REG13_AREA").toDouble());
    results->insert("REG13_BMC",  query.value("REG13_BMC").toDouble());
    results->insert("REG13_BMD",  query.value("REG13_BMD").toDouble());

    results->insert("REG14_NAME", query.value("REG14_NAME").toString());
    results->insert("REG14_AREA", query.value("REG14_AREA").toDouble());
    results->insert("REG14_BMC",  query.value("REG14_BMC").toDouble());
    results->insert("REG14_BMD",  query.value("REG14_BMD").toDouble());

    return results;
}

QSharedPointer<QJsonObject> subRegionCompositionQuery() {
    QSqlQuery query;
    query.prepare("SELECT * FROM SubRegionComposition WHERE PATIENT_KEY = :patientKey AND SCANID = :scanId");
    if (!query.exec()) {
        qDebug() << query.lastError().text();
        throw QException();
    }

    auto results = QSharedPointer<QJsonObject>::create();

    results->insert("NET_AVG_FAT",  query.value("NET_AVG_FAT").toDouble());
    results->insert("NET_AVG_LEAN", query.value("NET_AVG_LEAN").toDouble());
    results->insert("NET_AVG_MASS", query.value("NET_AVG_MASS").toDouble());
    results->insert("NET_AVG_PFAT", query.value("NET_AVG_PFAT").toDouble());

    results->insert("GLOBAL_FAT",  query.value("GLOBAL_FAT").toDouble());
    results->insert("GLOBAL_LEAN", query.value("GLOBAL_LEAN").toDouble());
    results->insert("GLOBAL_MASS", query.value("GLOBAL_MASS").toDouble());
    results->insert("GLOBAL_PFAT", query.value("GLOBAL_PFAT").toDouble());

    results->insert("NO_REGIONS", query.value("NO_REGIONS").toLongLong());

    results->insert("TISSUE_ANALYSIS_METHOD", query.value("TISSUE_ANALYSIS_METHOD").toInt());
    results->insert("PHYSICIAN_COMMENT", query.value("PHYSICIAN_COMMENT").toString());

    results->insert("REG1_NAME", query.value("REG1_NAME").toString());
    results->insert("REG1_FAT",  query.value("REG1_FAT").toDouble());
    results->insert("REG1_LEAN", query.value("REG1_LEAN").toDouble());
    results->insert("REG1_MASS", query.value("REG1_MASS").toDouble());
    results->insert("REG1_PFAT", query.value("REG1_PFAT").toDouble());

    results->insert("REG2_NAME", query.value("REG2_NAME").toString());
    results->insert("REG2_FAT",  query.value("REG2_FAT").toDouble());
    results->insert("REG2_LEAN", query.value("REG2_LEAN").toDouble());
    results->insert("REG2_MASS", query.value("REG2_MASS").toDouble());
    results->insert("REG2_PFAT", query.value("REG2_PFAT").toDouble());

    results->insert("REG3_NAME", query.value("REG3_NAME").toString());
    results->insert("REG3_FAT",  query.value("REG3_FAT").toDouble());
    results->insert("REG3_LEAN", query.value("REG3_LEAN").toDouble());
    results->insert("REG3_MASS", query.value("REG3_MASS").toDouble());
    results->insert("REG3_PFAT", query.value("REG3_PFAT").toDouble());

    results->insert("REG4_NAME", query.value("REG4_NAME").toString());
    results->insert("REG4_FAT",  query.value("REG4_FAT").toDouble());
    results->insert("REG4_LEAN", query.value("REG4_LEAN").toDouble());
    results->insert("REG4_MASS", query.value("REG4_MASS").toDouble());
    results->insert("REG4_PFAT", query.value("REG4_PFAT").toDouble());

    results->insert("REG5_NAME", query.value("REG5_NAME").toString());
    results->insert("REG5_FAT",  query.value("REG5_FAT").toDouble());
    results->insert("REG5_LEAN", query.value("REG5_LEAN").toDouble());
    results->insert("REG5_MASS", query.value("REG5_MASS").toDouble());
    results->insert("REG5_PFAT", query.value("REG5_PFAT").toDouble());

    results->insert("REG6_NAME", query.value("REG6_NAME").toString());
    results->insert("REG6_FAT",  query.value("REG6_FAT").toDouble());
    results->insert("REG6_LEAN", query.value("REG6_LEAN").toDouble());
    results->insert("REG6_MASS", query.value("REG6_MASS").toDouble());
    results->insert("REG6_PFAT", query.value("REG6_PFAT").toDouble());

    results->insert("REG7_NAME", query.value("REG7_NAME").toString());
    results->insert("REG7_FAT",  query.value("REG7_FAT").toDouble());
    results->insert("REG7_LEAN", query.value("REG7_LEAN").toDouble());
    results->insert("REG7_MASS", query.value("REG7_MASS").toDouble());
    results->insert("REG7_PFAT", query.value("REG7_PFAT").toDouble());

    results->insert("REG8_NAME", query.value("REG8_NAME").toString());
    results->insert("REG8_FAT",  query.value("REG8_FAT").toDouble());
    results->insert("REG8_LEAN", query.value("REG8_LEAN").toDouble());
    results->insert("REG8_MASS", query.value("REG8_MASS").toDouble());
    results->insert("REG8_PFAT", query.value("REG8_PFAT").toDouble());

    results->insert("REG9_NAME", query.value("REG9_NAME").toString());
    results->insert("REG9_FAT",  query.value("REG9_FAT").toDouble());
    results->insert("REG9_LEAN", query.value("REG9_LEAN").toDouble());
    results->insert("REG9_MASS", query.value("REG9_MASS").toDouble());
    results->insert("REG9_PFAT", query.value("REG9_PFAT").toDouble());

    results->insert("REG10_NAME", query.value("REG10_NAME").toString());
    results->insert("REG10_FAT",  query.value("REG10_FAT").toDouble());
    results->insert("REG10_LEAN", query.value("REG10_LEAN").toDouble());
    results->insert("REG10_MASS", query.value("REG10_MASS").toDouble());
    results->insert("REG10_PFAT", query.value("REG10_PFAT").toDouble());

    results->insert("REG11_NAME", query.value("REG11_NAME").toString());
    results->insert("REG11_FAT",  query.value("REG11_FAT").toDouble());
    results->insert("REG11_LEAN", query.value("REG11_LEAN").toDouble());
    results->insert("REG11_MASS", query.value("REG11_MASS").toDouble());
    results->insert("REG11_PFAT", query.value("REG11_PFAT").toDouble());

    results->insert("REG12_NAME", query.value("REG12_NAME").toString());
    results->insert("REG12_FAT",  query.value("REG12_FAT").toDouble());
    results->insert("REG12_LEAN", query.value("REG12_LEAN").toDouble());
    results->insert("REG12_MASS", query.value("REG12_MASS").toDouble());
    results->insert("REG12_PFAT", query.value("REG12_PFAT").toDouble());

    results->insert("REG13_NAME", query.value("REG13_NAME").toString());
    results->insert("REG13_FAT",  query.value("REG13_FAT").toDouble());
    results->insert("REG13_LEAN", query.value("REG13_LEAN").toDouble());
    results->insert("REG13_MASS", query.value("REG13_MASS").toDouble());
    results->insert("REG13_PFAT", query.value("REG13_PFAT").toDouble());

    results->insert("REG14_NAME", query.value("REG14_NAME").toString());
    results->insert("REG14_FAT",  query.value("REG14_FAT").toDouble());
    results->insert("REG14_LEAN", query.value("REG14_LEAN").toDouble());
    results->insert("REG14_MASS", query.value("REG14_MASS").toDouble());
    results->insert("REG14_PFAT", query.value("REG14_PFAT").toDouble());

    return results;
}


void obesityIndicesQuery() {
    QSqlQuery query;
    query.prepare("SELECT * FROM ObesityIndices WHERE PATIENT_KEY = :patientKey AND SCANID = :scanId");
    if (!query.exec()) {
        qDebug() << query.lastError().text();
        throw QException();
    }

    double FAT_STD;
    double LEAN_STD;
    double BRAIN_FAT;
    double WATER_LBM;
    double TOTAL_PERCENT_FAT;
    double BODY_MASS_INDEX;
    double ANDROID_GYNOID_RATIO;
    double ANDROID_PERCENT_FAT;
    double GYNOID_PERCENT_FAT;
    double FAT_MASS_RATIO;
    double TRUNK_LIMB_FAT_MASS_RATIO;
    double FAT_MASS_HEIGHT_SQUARED;
    double TOTAL_FAT_MASS;
    double LEAN_MASS_HEIGHT_SQUARED;
    double APPENDAGE_LEAN_MASS_HEIGHT_2;
    double TOTAL_LEAN_MASS;
    QString PHYSICIAN_COMMENT;
}


void androidGynoidCompositionQuery() {
    QSqlQuery query;
    query.prepare("SELECT * FROM AndroidGynoidComposition WHERE PATIENT_KEY = :patientKey AND SCANID = :scanId");
    if (!query.exec()) {
        qDebug() << query.lastError().text();
    }

    double ANDROID_FAT;
    double ANDROID_LEAN;
    double GYNOID_FAT;
    double GYNOID_LEAN;
    QString PHYSICIAN_COMMENT;
}

void forearmQuery() {
    QSqlQuery query;
    query.prepare("SELECT * FROM Forearm WHERE PATIENT_KEY = :patientKey AND SCANID = :scanId");
    if (!query.exec()) {
        qDebug() << query.lastError().text();
    }

    double R_13_AREA;
    double R_13_BMC;
    double R_13_BMD;
    double R_MID_AREA;
    double R_MID_BMC;
    double R_MID_BMD;
    double R_UD_AREA;
    double R_UD_BMC;
    double R_UD_BMD;
    double U_13_AREA;
    double U_13_BMC;
    double U_13_BMD;
    double U_MID_AREA;
    double U_MID_BMC;
    double U_MID_BMD;
    double U_UD_AREA;
    double U_UD_BMC;
    double U_UD_BMD;
    double RTOT_AREA;
    double RTOT_BMC;
    double RTOT_BMD;
    double UTOT_AREA;
    double UTOT_BMC;
    double UTOT_BMD;
    double RU13TOT_AREA;
    double RU13TOT_BMC;
    double RU13TOT_BMD;
    double RUMIDTOT_AREA;
    double RUMIDTOT_BMC;
    double RUMIDTOT_BMD;
    double RUUDTOT_AREA;
    double RUUDTOT_BMC;
    double RUUDTOT_BMD;
    double RUTOT_AREA;
    double RUTOT_BMC;
    double RUTOT_BMD;
    double ROI_TYPE;
    double ROI_WIDTH;
    double ROI_HEIGHT;
    double ARM_LENGTH;
    QString PHYSICIAN_COMMENT;
}

void ApexDatabaseHelper::tScoreQuery1(const QSqlDatabase& db) {
    /* First query (t score)
      SELECT UNIQUE_ID, AGE_YOUNG FROM ReferenceCurve WHERE REFTYPE = ?
        AND IF_CURRENT = 1 AND SEX = 'F'
        AND ETHNIC IS NULL
        AND METHOD IS [NULL, APEX]
        AND SOURCE LIKE '%' + source + '%'
        AND Y_LABEL = 'IDS_REF_LBL_BMD'
        AND BONERANGE
        sql += (ranges.get(bmdBoneRangeKey).equals("NULL") ? ("IS NULL") : ("= '" + ranges.get(bmdBoneRangeKey) + "'"));
    */
    QSqlQuery query;

    query.prepare("SELECT UNIQUE_ID, AGE_YOUNG FROM ReferenceCurve "
                  "WHERE REFTYPE = :refType "
                  "AND IF_CURRENT = 1 "
                  "AND SEX = 'F' "
                  "AND ETHNIC IS NULL "
                  "AND METHOD IS [NULL, APEX] "
                  "AND SOURCE LIKE '%:source%'"
                  "AND Y_LABEL = 'IDS_REF_LBL_BMD'"
                  "AND BONERANGE IS NULL");

    query.bindValue(":refType", 0);
    query.bindValue(":source", "source");

    if (!query.exec()) {
        qDebug() << "SQL Error: " << query.lastError().text();
        throw QException();
    }

    //results["unique_id"] = query.value("UNIQUE_ID");
    //results["age_young"] = query.value("AGE_YOUNG");
}

void ApexDatabaseHelper::tScoreQuery2(const QSqlDatabase& db) {
    // Second query (t score)
    // SELECT Y_VALUE, L_VALUE, STD FROM Points WHERE UNIQUE_ID = [curveID] AND X_VALUE = [ageYoung]

    QSqlQuery query;
    query.prepare("SELECT Y_VALUE, L_VALUE, STD FROM Points WHERE UNIQUE_ID = :curveID AND X_VALUE = :ageYoung");
    query.bindValue(":curveId", 0);
    query.bindValue(":ageYoung", 0);

    if (!query.exec()) {
        qDebug() << "SQL Error: " << query.lastError().text();
        throw QException();
    }

    //results["y_value"] = query.value("Y_VALUE");
    //results["l_value"] = query.value("L_VALUE");
    //results["std"] = query.value("STD");
}

void ApexDatabaseHelper::zScoreQuery1(const QSqlDatabase& db) {
    // First query (z score)
    //log.info("first query (Z score): " + sql);
    //sql = "SELECT UNIQUE_ID, AGE_YOUNG FROM ReferenceCurve";
    //sql += " WHERE REFTYPE = '" + getRefType() + "'";
    //sql += " AND IF_CURRENT = 1";
    //sql += gender;
    //sql += ethnicity;
    //sql += method;
    //sql += " AND SOURCE LIKE '%" + getRefSource() + "%'";
    //sql += " AND Y_LABEL = 'IDS_REF_LBL_BMD'";
    //sql += " AND BONERANGE ";
    //sql += (ranges.get(bmdBoneRangeKey).equals("NULL") ? ("IS NULL") : ("= '" + ranges.get(bmdBoneRangeKey) + "'"));

    QSqlQuery query;
    query.prepare("SELECT UNIQUE_ID, AGE_YOUNG FROM ReferenceCurve "
                  "WHERE REFTYPE = :refType "
                  "AND IF_CURRENT = 1"
                  "AND SEX = :sex"
                  "AND ETHNIC = :ethnicity"
                  "AND METHOD IS :method"
                  "AND SOURCE LIKE '%:refSource%'"
                  "AND Y_LABEL = 'IDS_REF_LBL_BMD'"
                  "AND BONERANGE IS NULL");

    query.bindValue(":refSource", 0);
    query.bindValue(":sex", "F");
    query.bindValue(":ethnicity", "");
    query.bindValue(":method", "");
    query.bindValue(":source", "");

    if (!query.exec()) {
        qDebug() << "SQL Error: " << query.lastError().text();
        throw QException();
    }
}

void ApexDatabaseHelper::zScoreQuery2(const QSqlDatabase& db) {
    QSqlQuery query;
    // Second query (z score)
    // sql = "SELECT X_VALUE FROM Points WHERE UNIQUE_ID = " + curveId;
    query.prepare("SELECT X_VALUE FROM Points WHERE UNIQUE_ID = :curveId");
    query.bindValue(":curveId", "");
    if (!query.exec()) {
        qDebug() << "SQL Error: " << query.lastError().text();
        throw QException();
    }
}

void ApexDatabaseHelper::zScoreQuery3(const QSqlDatabase& db) {
    QSqlQuery query;

    // Third query (z score)
    //sql = "SELECT Y_VALUE, L_VALUE, STD FROM Points WHERE UNIQUE_ID = " + curveId;
    //sql += " AND X_VALUE = ";
    query.prepare("SELECT Y_VALUE, L_VALUE, STD FROM Points WHERE UNIQUE_ID = :curveId AND X_VALUE = :xValue");
    query.bindValue(":curveId", "");
    query.bindValue(":xValue", "");

    if (!query.exec()) {
        qDebug() << "SQL Error: " << query.lastError().text();
        throw QException();
    }
}

QVariantMap ApexDatabaseHelper::computeTZValues(const QSqlDatabase& db)
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

    if (!db.isOpen()) {
        throw QException();
    }

    // if type == "S"
    // AP lumbar spine:
    // - identify the included vertebral levels
    // - sum the area and sum the bmc of the included vertebral levels
    // - compute the revised total bmd from summed bmc / summed area
    // - provide the proper bone range code for total bmd
    //
    // for each key in bmdData, query the bonerange key and compute BMD scores

    QVariantMap results;
    QSqlQuery query;

    tScoreQuery1(db);
    tScoreQuery2(db);

    zScoreQuery1(db);
    zScoreQuery2(db);
    zScoreQuery3(db);


    return results;
}

double ApexDatabaseHelper::computeYearsDifference()
{
    return 0;
}

#include "dxa_measurement.h"

#include "dcmtk/dcmdata/dcmetinf.h"

#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QSqlQuery>
#include <QException>
#include <QSqlError>

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

DXAMeasurement::DXAMeasurement()
{

}

QString DXAMeasurement::toString() const
{
   return "";
};

QStringList DXAMeasurement::toStringList(const bool& no_keys) const
{
    Q_UNUSED(no_keys)
    return QStringList {{}};
}

void DXAMeasurement::getScanAnalysisData(
    const QSqlDatabase &patscanDb,
    const QSqlDatabase& referenceDb,
    const QJsonObject& patientData
) {
    QSqlQuery query(patscanDb);

    query.prepare("SELECT SCANID, SCAN_MODE, SCAN_DATE FROM ScanAnalysis WHERE PATIENT_KEY = :patientKey AND SCAN_TYPE = :scanType");
    query.bindValue(":patientKey", patientData.value("PATIENT_KEY").toString());
    query.bindValue(":scanType",   getScanType());

    if (!query.exec()) {
        qDebug() << query.lastError().text();
        throw QException();
    }


    qDebug() << "Patient: " << patientData.value("PATIENT_KEY").toString() << "scanType: " << getScanType();
    qDebug() << "Number of scans: " << query.size();

    QString scanId;
    QString scanMode;
    QString scanDate;

    while (query.next()) {

        scanId = query.value("SCANID").toString();
        scanMode = query.value("SCAN_MODE").toString();
        scanDate = query.value("SCAN_DATE").toString();

        setAttribute("SCANID", scanId);
        setAttribute("SCAN_MODE", scanMode);
        setAttribute("SCAN_DATE", scanDate);
    }

    qDebug() << "Scan used: " << scanId << scanMode << scanDate;

    if (!hasAttribute("SCANID") || !hasAttribute("SCAN_MODE") || !hasAttribute("SCAN_DATE")) {
        qDebug() << "Not found..";
    }
    else {
        qDebug() << "Found" << scanMode << "scan for : " << patientData << "SCANID: " << scanId << "SCAN_DATE: " << scanDate;
    }

    getScanData(patscanDb, patientData.value("PATIENT_KEY").toString(), scanId);
    computeTZScore(referenceDb, patientData, scanDate);
}

void DXAMeasurement::computeTZScore(const QSqlDatabase &referenceDb, const QJsonObject& patientData, const QString& scanDate)
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

    qDebug() << "t and z score patient data: " << patientData;

    QMap<QString, double> bmdData;
    QString prefix = getName() + "_";
    QString source = getRefSource();

    if (getRefType() == "S") {
        bool included_array[] {false, false, false, false};

        double area_array[] {0.0, 0.0, 0.0, 0.0};
        double bmc_array[] {0.0, 0.0, 0.0, 0.0};
        double tot_bmd = 0.0;

        for (QMap<QString, Value>::const_iterator it = m_attributes.begin(); it != m_attributes.end(); ++it) {
            QString key = it.key();
            Value value = it.value();

            int index = -1;

            qDebug() << "Key:" << key << ", Value:" << value.toString();

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
                    included_array[index] = value.value().toBool();
                else if (key.endsWith("_AREA"))
                    area_array[index] = value.value().toDouble();
                else if (key.endsWith("_BMC"))
                    bmc_array[index] = value.value().toDouble();
            }

            if (key.endsWith("_BMD")) {
                key = QString(key).replace(prefix, "");
                if (key == "TOT_BMD")
                    tot_bmd = value.value().toDouble();
                else {
                    if (ranges.contains(key)) {
                        bmdData.insert(key, value.value().toDouble());
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
        for (QMap<QString, Value>::const_iterator it = m_attributes.begin(); it != m_attributes.end(); ++it) {
            QString key = it.key();
            Value value = it.value();

            if (key.endsWith("_BMD")) {
                qDebug() << "key pre: " + key + ", new key: " + QString(key).replace(prefix, "");
                key = QString(key).replace(prefix, "");
                if (ranges.contains(key)) {
                    bmdData.insert(key, value.value().toDouble());
                    qDebug() << "ranges contains key: " << key;
                }
            }
        }
    }

    qDebug() << prefix + " data contains: " << QString::number(m_attributes.size()) + " possible entries to get bmd values from";
    qDebug() << prefix + " bmddata contains: " << QString::number(bmdData.size()) + " entries to get tz";

    qDebug() << "bmdData: " << bmdData;

    AgeBracket bracket;
    double age {};

    age = computeYearsDifference(
        scanDate,
        patientData.value("BIRTHDATE").toString());

    qDebug() << "computed age from scandate and dob: " + QString::number(age);

    QSqlQuery query(referenceDb);

    QMap<QString, double>::iterator i;
    for (i = bmdData.begin(); i != bmdData.end(); ++i) {
        QString bmdBoneRangeKey  = i.key();
        double bmdValue = i.value();

        qDebug() << "working on range key: " << bmdBoneRangeKey + "with value: " + QString::number(bmdValue);

        QString method = " AND METHOD IS NULL";
        if (getRefType() == "S" && (bmdBoneRangeKey.contains("L1_") || bmdBoneRangeKey.contains("L4_"))) {
            method = " AND METHOD = 'APEX'";
        }

        QString sql = "SELECT UNIQUE_ID, AGE_YOUNG FROM ReferenceCurve";
        sql += " WHERE REFTYPE = '" + getRefType() + "'";
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

        if (!query.first()) {
            qDebug() << "first query (t score) failed to find results";
        }

        QString curveId = query.value("UNIQUE_ID").toString();
        double ageYoung = query.value("AGE_YOUNG").toDouble();

        qDebug() << "first query unique_id: " << curveId << "age young:" << ageYoung;


        sql = "SELECT Y_VALUE, L_VALUE, STD FROM Points WHERE UNIQUE_ID = " + curveId;
        sql += " AND X_VALUE = " + QString::number(ageYoung);

        qDebug() << "second query (T score): " + sql;

        query.prepare(sql);
        if (!query.exec()) {
            qDebug() << query.lastError().text();
            //throw QException();
            continue;
        }

        if (!query.first()) {
            qDebug() << "second query t score no results";
            continue;
        }

        QList<double> bmdValues;
        bmdValues.append(query.value("Y_VALUE").toDouble());
        bmdValues.append(query.value("L_VALUE").toDouble());
        bmdValues.append(query.value("STD").toDouble());

        double X_value = bmdValue;
        double M_value = bmdValues.at(0);
        double L_value = bmdValues.at(1);
        double sigma = bmdValues.at(2);

        double T_score = M_value * (pow(X_value / M_value, L_value) - 1.0) / (L_value * sigma);

        qDebug() << "M_value: " << M_value << "X_value: " << X_value << "L_value: " << "L_value" << "sigma: " << sigma;
        qDebug() << "T SCORE: " << T_score;

        QString varName = "";
        if (getRefType() == "S" && bmdBoneRangeKey.startsWith("TOT_"))
            varName += "TOT_T";
        else
            varName += QString(bmdBoneRangeKey).replace("_BMD", "_T");

        if (m_attributes.contains(varName)) {
            qDebug() << "exception 416";
            //throw QException();
            continue;
        }

        setAttribute(varName, T_score);

        double Z_score {};

        varName = "";
        if (getRefType() == "S" && bmdBoneRangeKey.startsWith("TOT_"))
            varName += "TOT_Z";
        else
            varName += QString(bmdBoneRangeKey).replace("_BMD", "_Z");

        if (m_attributes.contains(varName)) {
            qDebug() << "exception 432";
            continue;
        }
            //throw QException();

        QString sex = patientData.value("SEX").toString();
        sex = sex.toUpper();

        if (0 == sex.length() || sex.startsWith("F"))
            sex = " AND SEX = 'F'";
        else if (sex.startsWith("M")) {
            if (bmdBoneRangeKey == "U_UD_BMD") {
                setAttribute(varName, 0.0);
                continue;
            }
            sex = " AND SEX = 'M'";
        }

        QString ethnicity = patientData.value("ETHNICITY").toString();
        if (ethnicity.isNull()) ethnicity = "";
        ethnicity = ethnicity.toUpper();

        if (0 == ethnicity.length() || ethnicity == "W" || ethnicity == "O" || ethnicity == "P" || ethnicity == "I" ||
            (getRefType() == "R" && (ethnicity == "H" || ethnicity == "B")))
            ethnicity = " AND ETHNIC IS NULL";
        else
            ethnicity = " AND ETHNIC = '" + ethnicity + "'";

        sql = "SELECT UNIQUE_ID, AGE_YOUNG FROM ReferenceCurve";
        sql += " WHERE REFTYPE = '" + getRefType() + "'";
        sql += " AND IF_CURRENT = 1";
        sql += sex;
        sql += ethnicity;
        sql += method;
        sql += " AND SOURCE LIKE '%" + getRefSource() + "%'";
        sql += " AND Y_LABEL = 'IDS_REF_LBL_BMD'";
        sql += " AND BONERANGE ";
        sql += (ranges.value(bmdBoneRangeKey).toString() == "NULL" ? ("IS NULL") : ("= '" + ranges.value(bmdBoneRangeKey).toString() + "'"));

        qDebug() << "first query (z score): " + sql;

        query.prepare(sql);
        if (!query.exec()) {
            qDebug() << query.lastError().text();
            continue;
        }

        if (!query.first()) {
            qWarning() << "no results for first z score query..";
            continue;
        }

        curveId = query.value("UNIQUE_ID").toString();

        // Determine the age values (X axis variable) of the curve
        //
        sql = "SELECT X_VALUE FROM Points WHERE UNIQUE_ID = " + curveId;
        qDebug() << "second query (z score): " + sql;
        query.prepare(sql);
        if (!query.exec()) {
            qDebug() << query.lastError().text();
            continue;
            //throw QException();
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
                if (!query.exec()) {
                    qDebug() << query.lastError().text();
                    continue;
                }

                if (!query.first()) {
                    qDebug() << "no results for third query (z score)..";
                    continue;
                }

                bmdValues.append(query.value("Y_VALUE").toDouble());
                bmdValues.append(query.value("L_VALUE").toDouble());
                bmdValues.append(query.value("STD").toDouble());
            }

            double u = (age - bracket.ageMin) / bracket.ageSpan;

            qDebug() << "age bracket: " << bracket.ageMin << bracket.ageMax << bracket.ageSpan;
            qDebug() << "u: " << u << "bmdValues size: " << bmdValues.size() / 2;

            QList<double> interpValues;

            if (bmdValues.size() < 4) {
                qDebug() << "not enough bmd values for interpretation";
                continue;
            }

            for (int i = 0; i < bmdValues.size() / 2; i++) {
                interpValues.append((1.0 - u) * bmdValues.at(i) + u * bmdValues.at(i + 3));
            }

            M_value = interpValues.at(0);
            L_value = interpValues.at(1);
            sigma = interpValues.at(2);

            Z_score = M_value * (pow(X_value / M_value, L_value) - 1.0) / (L_value * sigma);
            qDebug() << "z score: " << Z_score;

            // TODO ?
            //Z_score = static_cast<double>(static_cast<int>(Z_score * 10.0)) / 10.0;;
            //if (0.0 == abs(Z_score)) Z_score = 0.0;
        }

        setAttribute(varName, Z_score);

        qDebug() << varName + " = " << QString::number(Z_score);
        qDebug() << "finished current key: " << bmdBoneRangeKey;
    }

};

bool DXAMeasurement::isValid() const
{
    return false;
}

double DXAMeasurement::computeYearsDifference(const QString& first, const QString& second)
{
    QDateTime firstDt = QDateTime::fromString(first, Qt::ISODateWithMs);
    QDateTime secondDt = QDateTime::fromString(second, Qt::ISODateWithMs);

    if (!firstDt.isValid()) {
        qWarning() << "first datetime is not valid" << first;
        return 0.0;
    }

    if (!secondDt.isValid()) {
        qWarning() << "second datetime is not valid" << second;
        return 9.9;
    }

    const qint64 diff = firstDt.toSecsSinceEpoch() - secondDt.toSecsSinceEpoch();
    const double yearsDiff = diff / (60.0 * 60.0 * 24.0 * 365.25);

    return yearsDiff;
}

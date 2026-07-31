import logging

from typing import Literal

from PySide6.QtSql import QSqlDatabase, QSqlQuery

from pathlib import Path

logger = logging.getLogger("DXA")


class ReferenceDB:
    def __init__(self, db_path: Path):
        self.db = QSqlDatabase.addDatabase("QODBC", "reference")
        self.db.setDatabaseName(
            (
                f"Driver={{Microsoft Access Driver (*.mdb, *.accdb)}};"
                f"DBQ={str(db_path.resolve())};"
            )
        )

    def open(self) -> tuple[bool, str | None]:
        logger.debug("ReferenceDB::open")

        if not self.db.open():
            logger.critical(f"Error: {self.db.lastError().text()}")
            return False, "Could not open reference.mdb database"

        return True, None

    def close(self):
        logger.debug("ReferenceDB::close")

        self.db.close()

    def select_curve(
        self,
        method: Literal["NULL", "APEX"],
        ref_type: str,
        ref_source: str,
        bone_range: str | None,
    ):
        logger.debug(
            f"reference_db: select_curve - method {method} ref_type {ref_type} source {ref_source} bone_range {bone_range}"
        )

        sex = "AND SEX = 'F'"
        ethnic = "AND ETHNIC IS NULL"
        method = "AND METHOD IS NULL" if method == "NULL" else "AND METHOD = 'APEX'"
        bonerange = "AND BONERANGE IS NULL" if bone_range is None else f"AND BONERANGE = '{bone_range}'"

        query: QSqlQuery = QSqlQuery(db=self.db)
        if not query.exec(
            "SELECT UNIQUE_ID, AGE_YOUNG FROM ReferenceCurve "
            f"WHERE REFTYPE = '{ref_type}' "
            "AND IF_CURRENT = 1 "
            f"{sex} "
            f"{ethnic} "
            f"{method} "
            f"AND SOURCE LIKE '%{ref_source}%' "
            "AND Y_LABEL = 'IDS_REF_LBL_BMD' "
            f"{bonerange} "
        ):
            logger.critical(f"select_curve: {query.lastError().text()}")
            return False, "Could not complete analysis"

        print(query.lastQuery())

        if not query.first():
            logger.critical("select_curve: no results found")
            return False, "Could not complete analysis"

        logger.debug("size: ", query.size())

        return True, {
            "UNIQUE_ID": str(query.value("UNIQUE_ID")),
            "AGE_YOUNG": float(query.value("AGE_YOUNG")),
        }

    # def other_curve(self, patient_data, ref_type):
    #    sex = str(patient_data["SEX"])
    #    sex = sex.upper()

    #    if 0 == len(sex) or sex.startsWith("F"):
    #        sex = " AND SEX = 'F'"
    #    elif sex.startsWith("M") {
    #        if (bmdBoneRangeKey == "U_UD_BMD") {
    #            setAttribute(varName, 0.0);
    #            continue;
    #        }
    #        sex = " AND SEX = 'M'";
    #    }

    #    ethnicity = str(patient_data["ETHNICITY"])
    #    if ethnicity.isNull():
    #        ethnicity = "";
    #    ethnicity = ethnicity.toUpper();

    #    if 0 == len(ethnicity) or ethnicity == "W" or ethnicity == "O" or ethnicity == "P" or ethnicity == "I" or (ref_type == "R" and (ethnicity == "H" or ethnicity == "B")):
    #        ethnicity = " AND ETHNIC IS NULL"
    #    else:
    #        ethnicity = " AND ETHNIC = '" + ethnicity + "'"

    #    sql = "SELECT UNIQUE_ID, AGE_YOUNG FROM ReferenceCurve";
    #    sql += " WHERE REFTYPE = '" + getRefType() + "'";
    #    sql += " AND IF_CURRENT = 1";
    #    sql += sex;
    #    sql += ethnicity;
    #    sql += method;
    #    sql += " AND SOURCE LIKE '%" + getRefSource() + "%'";
    #    sql += " AND Y_LABEL = 'IDS_REF_LBL_BMD'";
    #    sql += " AND BONERANGE ";
    #    sql += (ranges.value(bmdBoneRangeKey).toString() == "NULL" ? ("IS NULL") : ("= '" + ranges.value(bmdBoneRangeKey).toString() + "'"));

    #    qDebug() << "first query (z score): " + sql;

    #    query.prepare(sql);
    #    if (!query.exec()) {
    #        qDebug() << query.lastError().text();
    #        continue;
    #    }

    #    if (!query.first()) {
    #        qWarning() << "no results for first z score query..";
    #        continue;
    #    }

    def select_points(self, unique_id: str, age_young: str):
        logger.debug(
            f"reference_db::select_points - unique_id {unique_id} age_young {age_young}"
        )

        query: QSqlQuery = QSqlQuery(db=self.db)

        query.prepare(
            "SELECT Y_VALUE, L_VALUE, STD "
            "FROM Points "
            "WHERE UNIQUE_ID = :unique_id"
            "AND X_VALUE = :age_young"
        )

        query.bindValue(":unique_id", unique_id)
        query.bindValue(":x_value", age_young)

        if not query.exec():
            logger.critical(f"select_points: {query.lastError().text()}")
            return False, "Could not perform analysis"

        if not query.first():
            logger.critical(f"select_points: no results found")
            return False, "Could not perform analysis"

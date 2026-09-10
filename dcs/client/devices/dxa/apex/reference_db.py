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

    def select_reference_curve(
        self,
        method: Literal["NULL", "APEX"],
        sex: Literal["F", "M"],
        ethnicity: str | None,
        ref_type: str,
        ref_source: str,
        bone_range: str | None,
    ) -> tuple[bool, dict | None]:
        logger.debug(
            f"reference_db: select_curve - method {method} \n sex {sex} \n ethnicity {ethnicity} \n ref_type {ref_type} \n source {ref_source} \n bone_range {bone_range} \n"
        )

        sex = f"AND SEX = '{sex}'"
        ethnicity = "AND ETHNIC IS NULL" if ethnicity is None else f"AND ETHNIC = '{ethnicity}'"
        method = "AND METHOD IS NULL" if method == "NULL" else "AND METHOD = 'APEX'"
        bone_range = (
            "AND BONERANGE IS NULL"
            if bone_range is None
            else f"AND BONERANGE = '{bone_range}'"
        )

        query: QSqlQuery = QSqlQuery(db=self.db)
        if not query.exec(
            "SELECT UNIQUE_ID, AGE_YOUNG FROM ReferenceCurve "
            f"WHERE REFTYPE = '{ref_type}' "
            "AND IF_CURRENT = 1 "
            f"{sex} "
            f"{ethnicity} "
            f"{method} "
            f"AND SOURCE LIKE '%{ref_source}%' "
            "AND Y_LABEL = 'IDS_REF_LBL_BMD' "
            f"{bone_range} "
        ):
            logger.error(f"select_curve: {query.lastError().text()}")
            return False, None

        logger.debug(query.lastQuery())

        if not query.first():
            logger.error("select_curve: no results found")
            return False, None

        return True, {
            "UNIQUE_ID": str(query.value("UNIQUE_ID")),
            "AGE_YOUNG": float(query.value("AGE_YOUNG")),
        }

    def select_point_from_curve(self, curve_id: str, age: float):
        logger.debug(
            f"select x_values from {curve_id}"
        )

        query: QSqlQuery = QSqlQuery(db=self.db)

        query.prepare(
            "SELECT Y_VALUE, L_VALUE, STD FROM Points WHERE UNIQUE_ID = :curve_id AND X_VALUE = :age"
        )
        query.bindValue(":curve_id", curve_id)
        query.bindValue(":age", age)

        if not query.exec():
            logger.error(f"select_point_from_curve: {curve_id} {age} {query.lastError().text()}")
            return False, None

        #logger.debug(query.lastQuery())

        if not query.first():
            logger.error("select_point_from_curve: {curve_id} {age_young} no results found")
            return False, None

        return True, {
            "Y_VALUE": float(query.value("Y_VALUE")),
            "L_VALUE": float(query.value("L_VALUE")),
            "STD": float(query.value("STD"))
        }


    def select_x_values_from_curve(self, curve_id: str) -> tuple[bool, dict | None]:
        logger.debug(
            f"select_x_values_from_curve: curve_id = {curve_id}"
        )

        query: QSqlQuery = QSqlQuery(db=self.db)

        query.prepare(
            "SELECT X_VALUE FROM Points WHERE UNIQUE_ID = :curve_id"
        )
        query.bindValue(":curve_id", curve_id)

        if not query.exec():
            logger.error(query.lastError().text())
            return False, None

        logger.debug(query.lastQuery())

        res = []
        while query.next():
            res.append(float(query.value("X_VALUE")))

        return True, res


    def select_points(self, unique_id: str, age_young: str) -> tuple[bool, dict | None]:
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
            logger.error(f"select_points: {query.lastError().text()}")
            return False, None

        logger.debug(query.lastQuery())

        if not query.first():
            logger.error(f"select_points: no results found")
            return False, None

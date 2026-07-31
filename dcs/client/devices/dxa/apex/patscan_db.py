import logging

from typing import Literal

from PySide6.QtSql import QSqlDatabase, QSqlQuery
from PySide6.QtCore import Qt

from pathlib import Path

logger = logging.getLogger("DXA")


class PatScanDB:
    def __init__(self, db_path: Path):
        self.db = QSqlDatabase.addDatabase("QODBC", "patscan")
        self.db.setDatabaseName(
            (
                f"Driver={{Microsoft Access Driver (*.mdb, *.accdb)}};"
                f"DBQ={str(db_path.resolve())};"
            )
        )

    def open(self) -> bool:
        logger.debug("opening patscan.mdb")
        if not self.db.open():
            logger.critical(f"Error: {self.db.lastError().text()}")
            return False
        return True

    def close(self):
        logger.debug("closing patscan.mdb")

        self.db.close()

    def get_patient_info(self, barcode: str) -> tuple[bool, dict | str]:
        query = QSqlQuery(self.db)

        query.prepare(
            "SELECT PATIENT_KEY, BIRTHDATE, SEX, ETHNICITY, WEIGHT, HEIGHT "
            "FROM PATIENT "
            "WHERE IDENTIFIER1 = :barcode"
        )

        query.bindValue(":barcode", barcode)
        if not query.exec():
            logger.critical(f"get_patient_info: {query.lastError().text()}")
            return False, "Could not retrieve patient info"

        if query.size() > 1:
            logger.warning(f"get_patient_info: more than 1 result for {barcode}")

        record = {}
        while query.next():
            record["PATIENT_KEY"] = query.value("PATIENT_KEY")
            record["BIRTHDATE"] = query.value("BIRTHDATE").toString(
                Qt.DateFormat.ISODate
            )
            record["SEX"] = query.value("SEX")
            record["ETHNICITY"] = query.value("ETHNICITY")
            record["WEIGHT"] = query.value("WEIGHT")
            record["HEIGHT"] = query.value("HEIGHT")

        return True, record

    def get_scan_analysis(
        self, patient_key: str, scan_type: int
    ) -> tuple[bool, dict | str]:

        query = QSqlQuery(self.db)

        query.prepare(
            "SELECT SCANID, SCAN_TYPE, SCAN_MODE, SCAN_DATE "
            "FROM ScanAnalysis "
            "WHERE PATIENT_KEY = :patient_key "
            "AND SCAN_TYPE = :scan_type "
            "ORDER BY SCAN_DATE DESC"
        )

        query.bindValue(":patient_key", patient_key)
        query.bindValue(":scan_type", scan_type)

        if not query.exec():
            logger.critical(f"get_scan_analysis: {query.lastError().text()}")
            return False, "Could not retrieve scan analysis"

        if query.size() > 1:
            logger.warning(f"found multiple scan analysis for {patient_key}")

        scans = []
        while query.next():
            scans.append(
                {
                    "SCANID": query.value("SCANID"),
                    "SCAN_TYPE": query.value("SCAN_TYPE"),
                    "SCAN_MODE": query.value("SCAN_MODE"),
                    "SCAN_DATE": query.value("SCAN_DATE").toString(
                        Qt.DateFormat.ISODate
                    ),
                }
            )

        return True, scans

    def get_scan_data(
        self,
        table: Literal[
            "Hip",
            "HipHSA",
            "Forearm",
            "Spine",
            "Wbody",
            "WbodyComposition",
            "SubRegionBone",
            "SubRegionComposition",
            "ObesityIndices",
            "AndroidGynoidComposition",
        ],
        patient_key: str,
        scan_id: str,
    ):
        query = QSqlQuery(self.db)

        query.prepare(
            f"SELECT * FROM {table} WHERE PATIENT_KEY = :patientKey AND SCANID = :scanId"
        )
        query.bindValue(":patientKey", patient_key)
        query.bindValue(":scanId", scan_id)

        if not query.exec():
            logger.critical(f"get_scan_data: {query.lastError().text()}")
            return False, "Could not retrieve scan data"

        if query.size() == 0:
            logger.critical(
                f"get_scan_data: could not find a result for {patient_key} {scan_id}"
            )
            return False, "No results found"

        if query.size() > 1:
            logger.warning(f"more than one result found for {patient_key} {scan_id}")

        res = {}
        while query.next():
            record = query.record()
            for i in range(record.count()):
                field_name = record.fieldName(i)
                res[field_name] = record.value(field_name)

        return True, res

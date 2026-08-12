import sys
import logging

from pathlib import Path
from datetime import datetime

from PySide6.QtCore import QCoreApplication
from PySide6.QtSql import QSqlDatabase, QSqlQuery

from typing import Literal
from devices.blood_pressure.settings import DEVICE_NAME

logger = logging.getLogger(DEVICE_NAME)


def datestring_to_epoch_s(date_str: str) -> int:
    dt = datetime.strptime(date_str, "%Y-%m-%d")
    seconds_since_epoch = dt.timestamp()
    return int(seconds_since_epoch)


class BPDatabase:
    def __init__(self, db_path: Path):
        logger.debug(f"BPDatabase::__init__ - {db_path}")

        if not db_path.exists():
            logger.error(f"{db_path} does not exist")
            raise ValueError(f"{db_path} does not exist")

        if not db_path.is_file():
            logger.error(f"{db_path} is not a file")
            raise ValueError(f"{db_path} is not a file")

        self.db = QSqlDatabase.addDatabase("QSQLITE")
        self.db.setDatabaseName(str(db_path.resolve()))

    def open(self):
        if not self.db.open():
            logger.error(self.db.lastError().text())
            return False
        return True

    def close(self):
        self.db.close()

    def insert_patient(
        self,
        name: str,
        barcode: str,
        gender: Literal["male", "female"],
        dob: str,
        physician: str,
    ):
        ###
        #  INSERT INTO Patient (Name, ID, Gender, DOB, Physician) values (:name, :id, :sex, :dob, :physician);
        ###

        logger.debug(
            f"database - inserting patient - {name} {barcode} {gender} {dob} {physician}"
        )

        query: QSqlQuery = QSqlQuery()

        query.prepare("""
            INSERT INTO Patient
            (Name, ID, Gender, DOB, Physician)
            VALUES
            (:name, :id, :gender, :dob, :physician);
        """)

        query.bindValue(":name", name)
        query.bindValue(":id", barcode)
        query.bindValue(":gender", 1 if gender == "female" else 0)
        query.bindValue(
            ":dob", datestring_to_epoch_s(datetime.strftime(dob, format="%Y-%m-%d"))
        )
        query.bindValue(":physician", physician)

        if not query.exec():
            logger.error(query.lastError().text())
            return False, None

        return True, query.lastInsertId()

    def get_patient_key(self, barcode: str) -> tuple[bool, int | None]:
        query = QSqlQuery()
        query.prepare("SELECT [Index] FROM Patient WHERE ID = :barcode")

        query.bindValue(":barcode", barcode)

        if not query.exec():
            logger.error(f"get_patient_key: {query.lastError().text()}")
            return False, None

        if query.size() > 1:
            logger.error(
                f"query returned multiple patient keys for barcode {barcode}"
            )
            return False, None

        if not query.first():
            logger.error(f"get_patient_key: no records for barcode {barcode}")
            return False, None

        return True, query.record().value(0)

    def get_measurements(self, patient_key: int) -> tuple[bool, list[dict]]:
        ###
        # SELECT * FROM data WHERE ID = barcode;
        ###

        logger.debug(f"BPDatabase::get_measurements - {patient_key}")

        query = QSqlQuery()
        query.prepare("SELECT * FROM Data WHERE Patient = :patient_key")
        query.bindValue(":patient_key", patient_key)

        results = []

        if not query.exec():
            logger.error(query.lastError().text())
            return False, []

        while query.next():
            row = {}

            for i in range(query.record().count()):
                row[query.record().fieldName(i)] = query.value(i)

            results.append(row)

        return True, results
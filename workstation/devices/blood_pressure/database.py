import logging

from pathlib import Path
from datetime import datetime

from PySide6.QtSql import QSqlDatabase, QSqlQuery

from typing import Literal
from devices.blood_pressure.settings import DEVICE_NAME

logger = logging.getLogger(DEVICE_NAME)


class BPDatabase:
    def __init__(self, db_path: Path):
        logger.debug(f"BPDatabase::__init__ - {db_path}")

        if not db_path.exists():
            logger.error(f"{db_path} does not exist")
            raise ValueError()

        if not db_path.is_file():
            logger.error(f"{db_path} is not a file")
            raise ValueError()

        self.db = QSqlDatabase.addDatabase("QSQLITE")
        self.db.setDatabaseName(str(db_path.resolve()))

        if not self.db.open():
            logger.error("database failed to open")
            raise Exception()

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
            f"BPDatabase::insert_patient - {name} {barcode} {gender} {dob} {physician}"
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
        query.bindValue(":dob", dob)
        query.bindValue(":physician", physician)

        return query.exec()

    def get_measurements(self, barcode):
        ###
        # SELECT * FROM data WHERE ID = barcode;
        ###

        logger.debug(f"BPDatabase::get_measurements - {barcode}")

        results = []

        query = QSqlQuery()
        query.prepare("SELECT * FROM data WHERE ID = :barcode")
        query.bindValue(":barcode", barcode)

        if not query.exec():
            logger.error("failed to get measurements from database")
            return None

        while query.next():
            row = {}

            for i in range(query.record().count()):
                row[query.record().fieldName(i)] = query.value(i)

            results.append(row)

        return results

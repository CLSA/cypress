from pathlib import Path
from typing import Literal

from PySide6.QtCore import QDateTime, Qt
from PySide6.QtSql import QSqlDatabase, QSqlQuery

import logging

from devices.tonometer.settings import DEVICE_NAME
from devices.tonometer.session import TonometerSession

logger = logging.getLogger(DEVICE_NAME)


class TonometerDatabase:
    def __init__(self, db_path: Path):
        logger.debug(f"TonometerDatabase::__init__ - {str(db_path.resolve())}")

        if not db_path.exists() or not db_path.is_file():
            raise FileNotFoundError(f"{str(db_path.resolve())} is not a file")

        self.db = QSqlDatabase.addDatabase("QODBC")
        self.db.setDatabaseName(
            "Driver={Microsoft Access Driver (*.mdb, *.accdb)};DBQ="
            + str(db_path.resolve())
        )

    def open(self) -> bool:
        if not self.db.open():
            logger.critical(self.db.lastError().text())
            return False
        return True

    def close(self):
        self.db.close()

    def insert_participant(self, session: TonometerSession) -> tuple[bool, str | None]:
        query: QSqlQuery = QSqlQuery(self.db)
        query.prepare(
            "INSERT INTO Patients "
            "( Name, BirthDate, Sex, GroupID, ID, RaceID ) "
            "VALUES ( :name, :birthDate, :sex, :groupId, :id, :raceId )"
        )
        query.bindValue(":name", f"{session.barcode},CLSA")
        query.bindValue(":birthDate", str(session.dob))
        query.bindValue(":sex", session.sex.lower()[0] == "m")
        query.bindValue(":groupId", 2)
        query.bindValue(":id", int(session.barcode))
        query.bindValue(":raceId", 1)

        if not query.exec():
            logger.error(query.lastError().text())
            return False, "could not initialize database"

        return True, None

    def get_patient_id(self, barcode) -> tuple[bool, int]:
        query: QSqlQuery = QSqlQuery(self.db)

        query.prepare("SELECT PatientID from Patients WHERE ID = :id")
        query.bindValue(":id", int(barcode))

        if not query.exec():
            logger.error(query.lastError().text())
            return False, "unable to execute query"

        if query.size() > 1:
            logger.error(f"{query.size()} results found")
            return False, "more than one patient found"

        if not query.first():
            logger.error(query.lastError().text())
            return False, "no results found"

        return True, int(query.record().value(0))

    def get_participant(self, barcode) -> tuple[bool, dict | str]:
        query: QSqlQuery = QSqlQuery(self.db)

        query.prepare("SELECT * from Patients WHERE ID = :id")
        query.bindValue(":id", int(barcode))

        if not query.exec():
            logger.error(query.lastError().text())
            return False, "unable load results from database"

        if query.size() > 1:
            logger.error(f"{query.size()} results found")
            return False, "more than one patient found"

        if not query.first():
            logger.error(query.lastError().text())
            return False, "no results found"

        record = query.record()

        participant = {}

        for i in range(record.count()):
            field_name = record.fieldName(i)
            field_value = record.value(field_name)

            if type(field_value) == QDateTime:
                participant[field_name.lower()] = field_value.toString(Qt.DateFormat.ISODate)
            else:
                participant[field_name.lower()] = field_value

        return True, participant

    def get_measures(self, patient_id: int) -> tuple[bool, list[dict] | str]:
        query: QSqlQuery = QSqlQuery(self.db)

        query.prepare(
            "SELECT * from Measures WHERE PatientID = :patient_id ORDER BY MeasureID ASC"
        )
        query.bindValue(":patient_id", patient_id)

        measurements = []

        if not query.exec():
            logger.error(query.lastError().text())
            return False, "could not retrieve measurements"

        while query.next():
            record = query.record()

            measure = {}

            for i in range(record.count()):
                field_name = record.fieldName(i)
                field_value = record.value(field_name)

                if type(field_value) == QDateTime:
                    measure[field_name.lower()] = field_value.toString(Qt.DateFormat.ISODate)
                else:
                    measure[field_name.lower()] = field_value

            measurements.append(measure)

        return True, measurements

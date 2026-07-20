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

    # Attempt to open database, return 0 if
    def open(self) -> bool:
        if not self.db.open():
            logger.critical(self.db.lastError().text())
            return False
        return True

    def close(self):
        self.db.close()

    def insert_participant(self, session: TonometerSession):
        query: QSqlQuery = QSqlQuery()

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
            print(query.lastError().text())
            return False

        return True

    def get_patient_id(self, barcode):
        query: QSqlQuery = QSqlQuery()

        query.prepare("SELECT PatientID from Patients WHERE ID = :id")
        query.bindValue(":id", int(barcode))

        if not query.exec():
            logger.error(query.lastError().text())
            return None

        if query.size() > 1:
            logger.error(f"More than one patient found with ID {barcode}")
            return None

        if not query.first():
            logger.error(query.lastError().text())
            return None

        return query.record().value(0)

    def get_measures(self, patient_id: int, eye: Literal["L", "R"]) -> dict | None:
        query: QSqlQuery = QSqlQuery()

        query.prepare(
            "SELECT * from Measures where Eye = :eye AND PatientID ORDER BY MeasureDate DESC"
        )
        query.bindValue(":eye", eye)
        query.bindValue(":patient_id", patient_id)

        measurements = []

        if not query.exec():
            logger.error(query.lastError().text())
            return None

        if not query.last():
            logger.error(f"No {eye} measures found for {str(patient_id)}")
            return None

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

        return measurements

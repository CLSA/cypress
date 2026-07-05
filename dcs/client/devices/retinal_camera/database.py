from pathlib import Path

from PySide6.QtSql import QSqlDatabase, QSqlQuery

import logging

from devices.retinal_camera.settings import DEVICE_NAME

logger = logging.getLogger(DEVICE_NAME)


class RetinalCameraDatabase:
    def __init__(self, db_name: str):
        logger.debug(f"RetinalCameraDatabase::__init__ - {db_name}")

        if not db_name:
            raise ValueError("db_name must exist")

        self.db = QSqlDatabase.addDatabase("QODBC")
        self.db.setDatabaseName(db_name)

        if not self.db.open():
            logger.error("database failed to open")
            raise Exception()

    def restore_database(self):
        query = QSqlQuery()

        if not self.db.transaction():
            logger.critical("could not start transaction")
            return False

        query.prepare(
            "ALTER DATABASE [IMAGEnet] SET single_user with rollback immediate"
        )
        if not query.exec():
            logger.critical(query.lastError().text())
            return False

        query.prepare(
            "RESTORE DATABASE [IMAGEnet] FROM DISK = N'C:\\Users\\Public\\Documents\\oct.bak' WITH FILE = 1, NOUNLOAD, STATS = 5"
        )
        if not query.exec():
            logger.critical(query.lastError().text())
            return False

        query.prepare("ALTER DATABASE [IMAGEnet] SET multi_user")
        if not query.exec():
            logger.critical(query.lastError().text())
            return False

        if not self.db.commit():
            logger.critical(query.lastError().text())
            return False

    def insert_participant(self, person_uid: str, first_name: str, last_name: str):
        query = QSqlQuery()

        if not self.db.transaction():
            logger.critical("could not start transaction")
            return False

        query.prepare(
            "INSERT INTO IMAGEnet.dbo.Persons (PersonUid, SurName, ForeName) VALUES (:personUid, :firstName, :lastName)"
        )
        query.bindValue(":personUid", "")  # defaultPersonUUID
        query.bindValue(":firstName", "CLSA")
        query.bindValue(":lastName", "Participant")

        if not query.exec():
            logger.critical(query.lastError().text())
            return False

        query.prepare(
            "INSERT INTO IMAGEnet.dbo.Patients (PatientUid, PatientIdentifier, PersonUid) VALUES (:patientUid, :participantId, :personUUID)"
        )
        query.bindValue(":patientUid", "")  # defaultPatientUUID
        query.bindValue(":participantId", "")  # m_session->getBarcode()
        query.bindValue(":personUUID", "")  # defaultPatientUUID

        if not query.exec():
            logger.critical(query.lastError().text())
            return False

        self.db.commit()

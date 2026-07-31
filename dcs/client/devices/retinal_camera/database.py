from pathlib import Path

from PySide6.QtSql import QSqlDatabase, QSqlQuery

import logging

from devices.retinal_camera.settings import DEVICE_NAME
from devices.retinal_camera.session import RetinalCameraSession

logger = logging.getLogger(DEVICE_NAME)

DEFAULT_INSERT_UUID = "11111111-2222-3333-4444-555555555555"

class RetinalCameraDatabase:
    def __init__(self, db_name: str):
        logger.debug(f"RetinalCameraDatabase::__init__ - {db_name}")

        self.db_name = db_name
        self.db = QSqlDatabase.addDatabase("QODBC")
        self.db.setDatabaseName(self.db_name)

    def open(self) -> bool:
        if not self.db.open():
            logger.critical(f"{self.db_name} failed to open")
            return False
        return True

    def close(self):
        self.db.close()

    def restore_database(self, backup_path: Path) -> tuple[bool, str | None]:
        if not backup_path.exists() or not backup_path.is_file():
            logger.critical(f"{str(backup_path.resolve())} is not a file")
            return False

        query: QSqlQuery = QSqlQuery()

        if not self.db.transaction():
            logger.critical(self.db.lastError().text())
            return False, "Could not restore database"

        query.prepare(
            "ALTER DATABASE [IMAGEnet] SET single_user with rollback immediate"
        )

        if not query.exec():
            logger.critical(query.lastError().text())
            return False, "Could not restore database"

        query.prepare(
            "RESTORE DATABASE [IMAGEnet] FROM DISK = :disk WITH FILE = 1, NOUNLOAD, STATS = 5"
        )
        query.bindValue(":disk", str(backup_path.resolve()))

        if not query.exec():
            logger.critical(query.lastError().text())
            return False, "Could not restore database"

        query.prepare("ALTER DATABASE [IMAGEnet] SET multi_user")

        if not query.exec():
            logger.critical(query.lastError().text())
            return False, "Could not restore database"

        if not self.db.commit():
            logger.critical(query.lastError().text())
            return False, "Could not restore database"

        return True, None

    def insert_participant(self, session: RetinalCameraSession) -> tuple[bool, str | None]:
        query = QSqlQuery()

        if not self.db.transaction():
            logger.critical("could not start transaction")
            return False, "Could not restore database"

        query.prepare(
            "INSERT INTO IMAGEnet.dbo.Persons (PersonUid, SurName, ForeName) VALUES (:personUid, :firstName, :lastName)"
        )
        query.bindValue(":personUid", DEFAULT_INSERT_UUID)
        query.bindValue(":firstName", "CLSA")
        query.bindValue(":lastName", "Participant")

        if not query.exec():
            logger.critical(query.lastError().text())
            return False, "Could not restore database"

        query.prepare(
            "INSERT INTO IMAGEnet.dbo.Patients (PatientUid, PatientIdentifier, PersonUid) VALUES (:patientUid, :participantId, :personUUID)"
        )
        query.bindValue(":patientUid", DEFAULT_INSERT_UUID)
        query.bindValue(":participantId", session.barcode)
        query.bindValue(":personUUID", DEFAULT_INSERT_UUID)

        if not query.exec():
            logger.critical(query.lastError().text())
            return False, "Could not restore database"

        if not self.db.commit():
            logger.critical(self.db.lastError().text())
            return False, "Could not restore database"

        return True, None

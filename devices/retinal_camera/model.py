from PySide6.QtSql import QSqlDatabase, QSqlQuery

from pathlib import Path

from instruments.model import Model

class RetinalCameraModel(Model):
    def __init__(self, session_data: dict):
        self.session_data = session_data.copy()
        self.measurements = []

    def restore_database(self, database_name: str) -> bool:
        self.database = QSqlDatabase.addDatabase('QODBC')
        self.database.setDatabaseName(database_name)

        if not self.database.open():
            return False

        if not self.database.transaction():
            return False

        query = QSqlQuery(self.database)

        query.prepare("ALTER DATABASE [IMAGEnet] SET single_user with rollback immediate")
        if not query.exec():
            return False

        query.prepare("RESTORE DATABASE [IMAGEnet] FROM DISK = N'' WITH FILE = 1, NOUNLOAD, STATS = 5")
        if not query.exec():
            return False

        query.prepare("ALTER DATABASE [IMAGEnet] SET multi_user")
        if not query.exec():
            return False

        if not self.database.commit():
            return False


    def clear_output_directory(self) -> bool:
        return True


    def add_patient(self) -> bool:
        if not self.database.transaction():
            return False

        query = QSqlQuery(self.database)
        query.prepare("INSERT INTO IMAGEnet.dbo.Persons (PersonUid, SurName, ForeName) VALUES (:personUid, :firstName, :lastName)")
        query.bindValue(":personUid", "12345678")
        query.bindValue(":firstName", "CLSA")
        query.bindValue(":lastName", "Participant")

        if not query.exec():
            return False

        query.prepare("INSERT INTO IMAGEnet.dbo.Patients (PatientUid, PatientIdentifier, PersonUid) VALUES (:patientUid, :participantId, :personUUID)")
        query.bindValue(":patientUid", "12345678")
        query.bindValue(":participantId", "12345678")
        query.bindValue(":personUUID", "12345678")

        if not query.exec():
            return False

        if not self.database.commit():
            return False

        return True


    def read_results(self, output_directory: Path) -> bool:
        if not output_directory.exists():
            return False

        if not output_directory.is_dir():
            return False

        self.measurements = []

        for file_path in output_directory.iterdir():
            if file_path.suffix == 'dcm':
                name = file_path.name
                name_parts = name.strip().split('_')

                if name_parts[0] != self.session_data.get('barcode'):
                    continue

                if name_parts[1] != self.session_data.get('side'):
                    continue

                if '_OPT_' in name:
                    pass

                elif '_OP_' in name:
                    pass

                else:
                    print("unknown")

                self.measurements.append({
                    'name': file_path.name,
                    'path': str(file_path.resolve()),
                    'size': 'N/A'
                })





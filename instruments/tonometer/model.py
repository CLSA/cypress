from PySide6.QtSql import QSqlDatabase, QSqlQuery
from pathlib import Path

from instruments.model import Model

class TonometerModel(Model):

    def __init__(self, session_data: dict):
        self.session_data = session_data.copy()
        self.measurements = []

    def restore_database(self, database_path: Path, backup_db_path: Path):
        if not backup_db_path.exists():
            print(f'error: {str(backup_db_path.resolve())} does not exist')
            return False

        if not backup_db_path.is_file():
            print(f'error: {str(backup_db_path.resolve())} is not a file')
            return False

        if database_path.exists():
            if not database_path.is_file():
                print(f'error: {str(database_path.resolve())} is not a file')
                return False
            database_path.unlink()

    def configure_database(self, database_path: Path):
        self.database = QSqlDatabase.addDatabase('QODBC')
        self.database.setDatabaseName('Driver={Microsoft Access Driver (*.mdb)\};DBQ=' + str(database_path.resolve()))

    def add_patient(self):
        if not self.database.open():
            print('error: could not open database')
            return False

        query = QSqlQuery(self.database)
        query.prepare(
            'INSERT INTO ' \
            'Patients ( Name, BirthDate, Sex, GroupID, ID, RaceID ) ' \
            'VALUES ( :name, :birthDate, :sex, :groupId, :id, :raceId )'
        )
        query.bindValue(':name',        f'{self.session_data.get('barcode')},CLSA')
        query.bindValue(':birthDate',   self.session_data.get('dob'))
        query.bindValue(':sex',         1 if self.session_data.get('sex').lower()[0] == 'm' else 0)
        query.bindValue(':groupId',     2)
        query.bindValue(':id',          int(self.session_data.get('barcode')))
        query.bindValue(':raceId',      1)

        if not query.exec():
            print(f'error: add_patient failed')
            return False

        return True

    def read_results(self, eye: str = 'L'):
        if not self.database.open():
            print('error: could not open database')
            return False

        query = QSqlQuery(self.database)

        query.prepare('SELECT PatientID from Patients WHERE ID = :id')
        query.bindValue(':id', int(self.session_data.get('barcode')))
        if not query.exec():
            print(f'error: failed to read patient where ID = {self.session_data.get('barcode')}')
            return False

        query.prepare('SELECT * FROM Measures WHERE Eye = :eye ORDER BY MeasureDate desc')
        query.bindValue(':eye', eye)
        if not query.exec():
            print(f'error: failed to read measures where eye = {eye}')
            return False

        measure = {}
        while query.next():
            for index in query.record().count():
                measure.update(query.record().fieldName(index), query.value(index))

        self.measurements.append(measure)

        return True


if __name__ == '__main__':
    model = TonometerModel({

    })

    model.configure_database()
    model.restore_database()
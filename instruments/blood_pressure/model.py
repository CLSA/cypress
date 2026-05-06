from PySide6.QtSql import QSqlDatabase, QSqlQuery

from pathlib import Path

from instruments.model import Model
from instruments.blood_pressure.session import BPSession

class BPModel(Model):
    def __init__(self, session: BPSession):
        self.session = session
        self.measurements = []

    def restore_backup():
        pass

    def configure_database(self, database_path: Path):
        self.database = QSqlDatabase.addDatabase("QSQLITE")
        self.database.setDatabaseName(str(database_path.resolve()))

    def add_patient(self):
        query = QSqlQuery
        query.prepare('INSERT INTO Patient (Name, ID, Gender, DOB, Physician) values (:name, :id, :gender, :dob, :physician)')

        query.bindValue(':name', 'Participant')
        query.bindValue(':id', self.session_data.get('barcode'))
        query.bindValue(':sex', 1 if str(self.session_data.get('sex')) == "female" else 0)
        query.bindValue(':dob', self.session_data.get('dob'))
        query.bindValue(':physician', 'CLSA')

        return query.exec()

    def configure_process(self):
        pass

    def run(self):
        pass

    def reset(self):
        self.measurements = {}


    def read_results(self):
        query = QSqlQuery

        query.prepare(
            'SELECT '      \
            'ID, '         \
            'Patient, '    \
            'SYS, '        \
            'DIA, '        \
            'MAP, '        \
            'PP, '         \
            'Spare5, '     \
            'cSYS, '       \
            'cDIA, '       \
            'cPP, '        \
            'Spare3, '     \
            'Spare4, '     \
            'HR, '         \
            'AFIB, '       \
            'Spare1, '     \
            'CODE, '       \
            'NOTE, '       \
            'Condition, '  \
            'UpdateTime, ' \
            'Device, '     \
            'Date, '       \
            'BPM, '        \
            'AwakeTime, '  \
            'AsleepTime, ' \
            'Interval1, '  \
            'Interval2, '  \
            'Interval1C, ' \
            'Interval2C, ' \
            'Option, '     \
            'Spare2, '     \
            'Spare6, '     \
            'Spare7, '     \
            'Spare8, '     \
            'Spare9, '     \
            'Spare10, '    \
            'Spare11, '    \
            'Spare12, '    \
            'Spare13, '    \
            'Spare14, '    \
            'Spare15, '    \
            'Spare16, '    \
            'Spare17, '    \
            'Spare18, '    \
            'Spare19, '    \
            'Spare20 '     \
            'FROM Data')

        if not query.exec():
            return False

        while query.next():
            measure = {}

            record = query.record()
            for index in record.count():
                measure.update(record.fieldName(index), record.value(index))

            self.measurements.append(measure)

        return True



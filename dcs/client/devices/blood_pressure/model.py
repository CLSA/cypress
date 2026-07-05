from PySide6.QtSql import QSqlDatabase, QSqlQuery

from pathlib import Path

from model import Model
from devices.blood_pressure.session import BPSession


class BPModel(Model):
    def __init__(self, session: BPSession, config):
        super().__init__(session, config)
        self.measurements = []
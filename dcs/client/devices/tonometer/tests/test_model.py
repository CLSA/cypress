import unittest
import json


from PySide6.QtCore import QCoreApplication

from devices.tonometer.session import TonometerSession
from devices.tonometer.model import TonometerModel
from devices.tonometer.config import TonometerConfig

class TestTonometerModel(unittest.TestCase):
    config, errors = TonometerConfig.from_ini()
    app = QCoreApplication()

    def setUp(self):
        self.config.database.unlink(missing_ok=True)
        self.config.backup_database.copy(self.config.database)

        self.session = TonometerSession(
            **{
                "barcode": "50012116",
                "uid": "00000000",
                "answer_id": 1,
                "interviewer": "test",
                "language": "en",
                "sex": "male",
                "dob": "1995-12-06"
            }
        )

        self.model = TonometerModel(session=self.session, config=self.config)

        return super().setUp()

    def tearDown(self):
        pass


    def test_read_results(self):
        self.assertTrue(self.model.read_results())

        print(json.dumps(self.model.to_response(), indent=4))
import unittest
import json

from pathlib import Path

from PySide6.QtCore import QCoreApplication

from devices.blood_pressure.model import BPModel
from devices.blood_pressure.database import BPDatabase
from devices.blood_pressure.session import BPSession
from devices.blood_pressure.config import BPConfig

class TestBloodPressureModel(unittest.TestCase):
    def setUp(self):
        self.app = QCoreApplication()

        config, errors = BPConfig.from_ini()
        if errors:
            raise Exception(errors)
        self.config = config

        self.session = BPSession(
            **{
                "barcode": "00000000",
                "uid": "00000000",
                "answer_id": 1,
                "interviewer": "test",
                "language": "en",
                "sex": "male",
                "dob": "1995-12-06"
            }
        )

        self.model = BPModel(session=self.session, config=self.config)

        return super().setUp()

    def tearDown(self):
        return super().tearDown()

    def test_read_results(self):
        self.db = BPDatabase(Path(__file__).parent / "DataBase.db")

        model = BPModel(session=self.session, config=self.config)

        try:
            if not self.db.open():
                print("Failed to open database")
                return False

            success, patient_key = self.db.get_patient_key(barcode=self.session.barcode)
            self.assertTrue(success)
            self.assertIsNotNone(patient_key)

            success, measures = self.db.get_measurements(patient_key)
            self.assertTrue(success)
            self.assertTrue(len(measures) > 0)

            success, error = model.read_results(db_rows=measures)
            self.assertTrue(success)
            self.assertIsNone(error)

            response = model.to_response()

            metadata = response["value"]["metadata"]
            results = response["value"]["results"]

            self.assertTrue(metadata["first_systolic"]["value"] == results[0]["systolic"]["value"])
            self.assertTrue(metadata["first_diastolic"]["value"] == results[0]["diastolic"]["value"])
            self.assertTrue(metadata["first_pulse"]["value"] == results[0]["pulse"]["value"])

            print(json.dumps(response, indent=4))

        except Exception as e:
            print(e)
            return False

        finally:
            self.db.close()

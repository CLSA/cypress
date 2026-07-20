import unittest
import json

from PySide6.QtCore import QCoreApplication

from devices.tonometer.config import TonometerConfig
from devices.tonometer.session import TonometerSession
from devices.tonometer.database import TonometerDatabase


class TestTonometerDB(unittest.TestCase):
    config, errors = TonometerConfig.from_ini()
    app = QCoreApplication()

    def setUp(self):
        self.config.database.unlink(missing_ok=True)
        self.config.backup_database.copy(self.config.database)

        self.session = TonometerSession(
            **{
                "barcode": "12345678",
                "uid": "00000000",
                "answer_id": 1,
                "interviewer": "test",
                "language": "en",
                "sex": "male",
                "dob": "1995-12-06",
            }
        )

        return super().setUp()

    def tearDown(self):
        pass

    def test_insert_participant(self):
        database = TonometerDatabase(self.config.database)

        try:
            self.assertTrue(database.open())
            inserted = database.insert_participant(self.session)
            self.assertTrue(inserted)
        except Exception as e:
            print(e)
            raise e
        finally:
            database.close()

    def test_get_left_measures(self):
        database = TonometerDatabase(self.config.database)

        try:
            self.assertTrue(database.open())

            inserted = database.insert_participant(self.session)
            self.assertTrue(inserted)
            self.assertEqual(type(inserted), bool)

            patient_id = database.get_patient_id(barcode=self.session.barcode)

            self.assertIsNotNone(patient_id)
            self.assertEqual(type(patient_id), int)

            left_eye_measures = database.get_measures(patient_id, eye="L")
            print(json.dumps(left_eye_measures, indent=4))

        except Exception as e:
            print(e)
            raise e
        finally:
            database.close()

    def test_get_right_measures(self):
        database = TonometerDatabase(self.config.database)

        try:
            self.assertTrue(database.open())

            inserted = database.insert_participant(self.session)
            self.assertTrue(inserted)
            self.assertEqual(type(inserted), bool)

            patient_id = database.get_patient_id(barcode=self.session.barcode)

            self.assertIsNotNone(patient_id)
            self.assertEqual(type(patient_id), int)

            right_eye_measures = database.get_measures(patient_id, eye="R")
            print(json.dumps(right_eye_measures, indent=4))

        except Exception as e:
            print(e)
            raise e

        finally:
            database.close()

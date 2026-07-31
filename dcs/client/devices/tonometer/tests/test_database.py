import unittest
import json

from pathlib import Path

from PySide6.QtCore import QCoreApplication

from devices.tonometer.config import TonometerConfig
from devices.tonometer.session import TonometerSession
from devices.tonometer.database import TonometerDatabase


class TestTonometerDB(unittest.TestCase):

    app = QCoreApplication()

    def setUp(self):
        self.database_path = Path(__file__).parent / "ora.mdb"
        self.backup_path = Path(__file__).parent / "fixtures" / "ora.mdb"

        self.database_path.unlink(missing_ok=True)
        self.backup_path.copy(self.database_path)

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
        database = TonometerDatabase(self.database_path)

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
        database = TonometerDatabase(self.database_path)

        try:
            self.assertTrue(database.open())

            result, error = database.insert_participant(self.session)
            self.assertTrue(result)
            self.assertIsNone(error)
            self.assertEqual(type(result), bool)

            success, result = database.get_patient_id(barcode="12345678")
            self.assertTrue(success)
            self.assertEqual(type(result), int)

            success, result = database.get_measures(result, eye="L")
            self.assertTrue(success)
            self.assertEqual(type(result), list)

            print(json.dumps(result, indent=4))

        except Exception as e:
            print(e)
            raise e
        finally:
            database.close()

    def test_get_right_measures(self):
        database = TonometerDatabase(self.database_path)

        try:
            self.assertTrue(database.open())

            success, error = database.insert_participant(self.session)
            self.assertTrue(success)
            self.assertIsNone(error)
            self.assertEqual(type(success), bool)

            success, result = database.get_patient_id(barcode="12345678")
            self.assertTrue(success)
            self.assertEqual(type(result), int)

            success, result = database.get_measures(result, eye="R")
            self.assertTrue(success)
            self.assertEqual(type(result), list)

            print(json.dumps(result, indent=4))

        except Exception as e:
            print(e)
            raise e
        finally:
            database.close()

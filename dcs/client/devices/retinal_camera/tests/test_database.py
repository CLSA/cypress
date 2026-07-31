import unittest

from PySide6.QtCore import QCoreApplication

from devices.retinal_camera.config import RetinalCameraConfig
from devices.retinal_camera.session import RetinalCameraSession
from devices.retinal_camera.database import RetinalCameraDatabase


class TestRetinalCameraDB(unittest.TestCase):
    config, errors = RetinalCameraConfig.from_ini()
    app = QCoreApplication()

    def setUp(self):
        self.session = RetinalCameraSession(
            **{
                "barcode": "99999999",
                "uid": "00000000",
                "answer_id": 1,
                "interviewer": "test",
                "language": "en",
                "sex": "male",
                "dob": "",
            }
        )

        return super().setUp()

    def test_database(self):
        database = RetinalCameraDatabase(self.config.database_name)

        try:
            self.assertTrue(database.open())
            success, error = database.restore_database(self.config.database_backup)
            self.assertTrue(success)
            self.assertIsNone(error)

            success, error = database.insert_participant(self.session)
            self.assertTrue(success)
            self.assertIsNone(error)

        except Exception as e:
            print(e)
            raise e
        finally:
            database.close()


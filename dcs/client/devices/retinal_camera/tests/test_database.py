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
                "dob": "1995-12-06",
            }
        )

        return super().setUp()

    def tearDown(self):
        pass

    #def test_restore_database(self):
    #    database = RetinalCameraDatabase(self.config.database_name)

    #    try:
    #        self.assertTrue(database.open())
    #        #self.assertTrue(database.restore_database(self.config.database_backup))
    #    except Exception as e:
    #        print(e)
    #        raise e
    #    finally:
    #        database.close()

    def test_insert_participant(self):
        database = RetinalCameraDatabase(self.config.database_name)

        try:
            self.assertTrue(database.open())
            self.assertTrue(database.restore_database(self.config.database_backup))
            self.assertTrue(database.insert_participant(self.session))
        except Exception as e:
            print(e)
            raise e
        finally:
            database.close()


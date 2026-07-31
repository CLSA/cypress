import json
import unittest

from devices.retinal_camera.model import RetinalCameraModel
from devices.retinal_camera.config import RetinalCameraConfig

from devices.retinal_camera.session import RetinalCameraSessionLeft
from devices.retinal_camera.session import RetinalCameraSessionRight

from devices.retinal_camera.tests.fixtures.generate_dicom import (
    generate_fake_dicom_files,
)


class TestModel(unittest.TestCase):

    def setUp(self):
        config, errors = RetinalCameraConfig.from_ini()
        if errors:
            raise Exception(errors)

        self.config = config

        return super().setUp()

    def test_left(self):
        session = RetinalCameraSessionLeft(
            **{
                "barcode": "12345678",
                "uid": "00000000",
                "answer_id": 1,
                "interviewer": "test",
                "language": "en",
                "side": "L",
            }
        )

        generate_fake_dicom_files(
            directory=self.config.export_path, patient_id="12345678", side="L", n=10
        )

        model = RetinalCameraModel(session=session, config=self.config)

        success, error = model.read_results(self.config.export_path)

        self.assertTrue(success)
        self.assertIsNone(error)

        print(json.dumps(model.to_response(), indent=4))

    def test_right(self):
        session = RetinalCameraSessionRight(
            **{
                "barcode": "12345678",
                "uid": "00000000",
                "answer_id": 1,
                "interviewer": "test",
                "language": "en",
                "side": "R",
            }
        )

        generate_fake_dicom_files(
            directory=self.config.export_path,
            patient_id="12345678",
            side="R",
            n=10
        )

        model = RetinalCameraModel(session=session, config=self.config)

        success, error = model.read_results(self.config.export_path)

        self.assertTrue(success)
        self.assertIsNone(error)

        print(json.dumps(model.to_response(), indent=4))

import unittest
import json

from pathlib import Path

from devices.audiometer.model import AudiometerModel
from devices.audiometer.session import AudiometerSession
from devices.audiometer.config import AudiometerConfig


class TestAudiometerModel(unittest.TestCase):
    def setUp(self):
        self.config = AudiometerConfig.from_ini()

        self.session = AudiometerSession(
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

        self.model = AudiometerModel(session=self.session, config=self.config)

        return super().setUp()

    def tearDown(self):
        return super().tearDown()

    def test_read_results(self):
        with open(Path("./devices/audiometer/tests/output.json")) as output_json_file:
            output_json = json.load(output_json_file)
            self.model.parse_output_json(output_json)
            self.assertTrue(self.model.is_valid())
            print(json.dumps(self.model.to_response(), indent=4))
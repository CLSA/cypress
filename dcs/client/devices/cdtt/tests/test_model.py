import unittest
import json

from pathlib import Path

from devices.cdtt.session import CDTTSession
from devices.cdtt.config import CDTTConfig
from devices.cdtt.model import CDTTModel


class TestCDTTModel(unittest.TestCase):
    def setUp(self):
        config, errors = CDTTConfig.from_ini()
        if errors:
            print(errors)
            raise Exception("Config errors")

        self.config = config
        self.session = CDTTSession(
            **{
                "barcode": "00000000",
                "uid": "00000000",
                "answer_id": 1,
                "interviewer": "test",
                "language": "fr",
                "sex": "male",
                "dob": "1960-01-01",
            }
        )

        self.model = CDTTModel(session=self.session, config=self.config)

        return super().setUp()

    def tearDown(self):
        return super().tearDown()

    def test_read_results(self):
        for file_path in Path(__file__).parent.glob("*.xlsx"):
            self.model.read_output(file_path)
            print(json.dumps(self.model.to_response(), indent=4))
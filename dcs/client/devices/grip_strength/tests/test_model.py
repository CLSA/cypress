import unittest

from devices.grip_strength.config import GripStrengthConfig
from devices.grip_strength.session import GripStrengthSession
from devices.grip_strength.model import GripStrengthModel

class TestGripStrengthModel(unittest.TestCase):
    def setUp(self):
        super().setUp()

        self.config = GripStrengthConfig.from_ini()
        self.session = GripStrengthSession(
            **{
                "barcode": "00000000",
                "uid": "00000000",
                "answer_id": 1,
                "interviewer": "test",
                "language": "en",
                "sex": "",
                "dob": ""
            }
        )
        self.model = GripStrengthModel(session=self.session, config=self.config)

    def tearDown(self):
        super().tearDown()

    def test_read_results(self):
        #with open()
        pass


import unittest

from datetime import date

from pathlib import Path

from devices.crt.model import CRTModel
from devices.crt.session import CRTSession
from devices.crt.config import CRTConfig


class TestCRTModel(unittest.TestCase):
    def setUp(self):
        self.config = CRTConfig.from_ini()

        fixtures = sorted(Path("./devices/crt/tests/fixtures").glob("*.csv"))
        if not fixtures:
            raise Exception("Fixtures directory is empty")
        fixture = fixtures[0]

        for fp in self.config.output.iterdir():
            fp.unlink() if fp.exists() and fp.is_file() else None

        fixture.copy_into(self.config.output)

        self.session = CRTSession(
            **{
                "barcode": "00000000",
                "uid": "00000000",
                "answer_id": 1,
                "interviewer": "test",
                "language": "en",
            }
        )

        self.model = CRTModel(session=self.session, config=self.config)

        return super().setUp()

    def tearDown(self):
        return super().tearDown()

    def test_find_result_file(self):
        csv_file = self.model._find_result_file()
        self.assertIsNotNone(csv_file)

    def test_read_results(self):
        self.assertTrue(self.model.read_results())
        self.assertGreater(len(self.model.output), 1)
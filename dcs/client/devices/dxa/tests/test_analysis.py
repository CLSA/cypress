import unittest

from devices.dxa.utils.analysis import compute_age_bracket


class TestAnalysis(unittest.TestCase):

    @classmethod
    def setUpClass(cls):
        cls.age_table: list[float] = [
            20.0,
            25.0,
            30.0,
            35.0,
            40.0,
            45.0,
            50.0,
            55.0,
            60.0,
            65.0,
            70.0,
            75.0,
            80.0,
            85.0,
            90.0,
            95.0,
            100.0,
        ]

        return super().setUpClass()

    def test_age_bracket(self):
        age_bracket = compute_age_bracket(age=64.0, age_table=self.age_table)
        print(
            age_bracket.get("age_min"),
            age_bracket.get("age_max"),
            age_bracket.get("age_span"),
        )

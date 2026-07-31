import json

from pathlib import Path
from typing import override

from model import Model
from measure import Record

from devices.grip_strength.session import GripStrengthSession
from devices.grip_strength.config import GripStrengthConfig
from devices.grip_strength.paradox import ParadoxDb, as_kg, as_newtons


class GripStrengthTest(Record):
    field_map = {
        "ExamID": {"attr": "exam_id", "data_type": int, "units": None},
        "TestID": {"attr": "test_id", "data_type": int, "units": None},
        "Test": {"attr": "test", "data_type": str, "units": None},
        "Rung": {"attr": "rung", "data_type": int, "units": None},
        "Units": {"attr": "units", "data_type": str, "units": None},
        "MaxReps": {"attr": "max_reps", "data_type": int, "units": None},
        "Sequence": {"attr": "sequence", "data_type": str, "units": None},
        "RestTime": {"attr": "rest", "data_type": int, "units": None},
        "Rate": {"attr": "rate", "data_type": int, "units": None},
        "Threshold": {"attr": "threshold", "data_type": int, "units": None},
        "PrimaryStat": {"attr": "primary_stat", "data_type": str, "units": None},
        "NormType": {"attr": "norm_type", "data_type": int, "units": None},
        "Comparison": {"attr": "comparison", "data_type": int, "units": None},
    }

    metadata_attrs_from_measure = set(["average", "maximum", "cv"])

    def __init__(self, raw_data, measure: GripStrengthMeasure):
        super().__init__(raw_data)
        self.measure = measure

    def get_metadata(self):
        test_metadata = self.to_dict()

        for key in list(self.metadata_attrs_from_measure):
            measure_dict = self.measure.to_dict()
            test_metadata[key] = measure_dict.get(key, None)

        return test_metadata


class GripStrengthMeasure(Record):
    field_map = {
        "Position": {"attr": "position", "data_type": int, "units": None},
        "Side": {"attr": "side", "data_type": str, "units": None},
        "Rep1": {"attr": "rep1", "data_type": int, "units": None},
        "Rep2": {"attr": "rep2", "data_type": int, "units": None},
        "Rep3": {"attr": "rep3", "data_type": int, "units": None},
        "Average": {"attr": "average", "data_type": int, "units": None},
        "Maximum": {"attr": "maximum", "data_type": int, "units": None},
        "CV": {"attr": "cv", "data_type": int, "units": None},
    }


class GripStrengthModel(Model):
    def __init__(self, session: GripStrengthSession, config: GripStrengthConfig):
        super().__init__(session=session, config=config)

    def is_valid(self) -> bool:
        return False

    def read_results(self) -> tuple[bool, str | None]:
        self.reset()

        grip_test_db_path = Path(self.config.database_path / "ZGripTest.DB")
        grip_test_data_db_path = Path(self.config.database_path / "ZGripTestData.DB")

        try:
            grip_test_db = ParadoxDb(grip_test_db_path)
            tests = grip_test_db.read_records()
            if len(tests) != 1:
                return False, "More than one test found"

            grip_test_data_db = ParadoxDb(grip_test_data_db_path)
            measures = grip_test_data_db.read_records()
            if len(measures) != 1:
                return False, "More than one result found"

            self.test = GripStrengthTest(
                tests[0], measure=GripStrengthMeasure(measures[0])
            )

            print(self.test.to_dict())

        except FileNotFoundError as e:
            print(e)
            return False, f"{self.config.grip_test_db} not found"
        except ValueError:
            print(e)
            return (
                False,
                f"Something went wrong when loading {self.config.grip_test_db}",
            )
        except Exception as e:
            print(e)
            return False, f"Unknown error: {e}"

        return True, None

    @override
    def to_response(self):
        res = super().to_response()

        units = self.test.units if self.test.units else "kg"

        measure_dict = self.test.measure.to_dict()

        measure_dict["rep1"] = {
            "value": self._convert_units(self.test.measure.rep1),
            "units": units,
        }
        measure_dict["rep2"] = {
            "value": self._convert_units(self.test.measure.rep2),
            "units": units,
        }
        measure_dict["rep3"] = {
            "value": self._convert_units(self.test.measure.rep3),
            "units": units,
        }

        del measure_dict["average"]
        del measure_dict["maximum"]
        del measure_dict["cv"]

        res["value"]["results"].append(measure_dict)

        metadata = self.test.get_metadata()

        metadata["average"] = self._convert_units(metadata["average"])
        metadata["maximum"] = self._convert_units(metadata["maximum"])
        metadata["threshold"] = self._convert_units(self.test.threshold)

        res["value"]["metadata"] = metadata

        return res

    def _convert_units(self, value):
        if self.test.units == "kg":
            return as_kg(value)
        elif self.test.units == "N":
            return as_newtons(value)
        else:
            return value


if __name__ == "__main__":
    config, errors = GripStrengthConfig.from_ini()
    if errors:
        print(errors)

    model = GripStrengthModel(
        session=GripStrengthSession(
            **{
                "barcode": "00000000",
                "uid": "00000000",
                "answer_id": 1,
                "interviewer": "test",
                "language": "en",
            }
        ),
        config=config,
    )

    success, error = model.parse_database()

    if not success:
        print(error)
    else:
        print("OK")
        print(model.test)
        print(model.measure)
        print(json.dumps(model.to_response(), indent=4))

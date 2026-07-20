import traceback
import json

from typing import override
from pathlib import Path

from PySide6.QtCore import QCoreApplication

from model import Model

from devices.blood_pressure.session import BPSession
from devices.blood_pressure.config import BPConfig
from devices.blood_pressure.database import BPDatabase


class Trial:
    def __init__(self, db_row):
        self._parse_db_row(db_row)

    def _parse_db_row(self, db_row):
        res = {}

        for key, value in db_row.items():
            if key == "SYS":
                res["systolic"] = {"value": value, "units": "mmHg"}
            elif key == "DIA":
                res["diastolic"] = {"value": value, "units": "mmHg"}
            elif key == "HR":
                res["pulse"] = {"value": value, "units": "bpm"}
            elif key == "Spare7":
                res["reading_number"] = value

            res[key] = value

        self.data = res

    def get_reading_number(self):
        return self.data["reading_number"]

    def get_systolic(self):
        return self.data["systolic"]

    def get_diastolic(self):
        return self.data["diastolic"]

    def get_pulse(self):
        return self.data["pulse"]

    def get_start_time(self):
        pass

    def get_end_time(self):
        pass

    def to_dict(self):
        return self.__dict__()

    def __dict__(self):
        return {**self.data}


class Test:
    def __init__(self, trials: list[Trial]):
        self.trials = trials

    def get_average(self) -> dict | None:
        avg_count = len(self.trials) - 1
        if avg_count < 1:
            return None

        avg_systolic = 0
        avg_diastolic = 0
        avg_pulse = 0

        for trial in self.trials[1:]:
            avg_systolic += trial.get_systolic()["value"] / avg_count
            avg_diastolic += trial.get_diastolic()["value"] / avg_count
            avg_pulse += trial.get_pulse()["value"] / avg_count

        return {
            "avg_count": avg_count,
            "avg_systolic": { "value": avg_systolic, "units": "mmHg" },
            "avg_diastolic": { "value": avg_diastolic, "units": "mmHg" },
            "avg_pulse": { "value": avg_pulse, "units": "bpm" }
        }

    def get_total_average(self) -> dict:
        avg_count = len(self.trials)

        avg_systolic = 0
        avg_diastolic = 0
        avg_pulse = 0

        for trial in self.trials[1:]:
            avg_systolic += trial.get_systolic()["value"] / avg_count
            avg_diastolic += trial.get_diastolic()["value"] / avg_count
            avg_pulse += trial.get_pulse()["value"] / avg_count

        return {
            "total_avg_count": avg_count,
            "total_avg_systolic": { "value": avg_systolic, "units": "mmHg" },
            "total_avg_diastolic": { "value": avg_diastolic, "units": "mmHg" },
            "total_avg_pulse": { "value": avg_pulse, "units": "bpm" }
        }

    def get_first(self) -> dict:
        return {
            "first_systolic": self.trials[0].get_systolic(),
            "first_diastolic": self.trials[0].get_diastolic(),
            "first_pulse": self.trials[0].get_pulse(),
            "first_end_time": self.trials[0].get_start_time(),
            "first_start_time": self.trials[0].get_end_time(),
        }


class BPModel(Model):
    def __init__(self, session: BPSession, config: BPConfig):
        super().__init__(session, config)

    def read_results(self, db_rows) -> None:
        self.reset()

        trials = []

        for db_row in db_rows:
            trials.append(Trial(db_row))

        self.test = Test(trials=trials)

    @override
    def to_response(self) -> dict:
        if not self.test:
            return None

        res = super().to_response()
        res["value"]["metadata"] = {
            **self.test.get_first(),
            **self.test.get_average(),
            **self.test.get_total_average(),
        }

        res["value"]["results"] = [trial.to_dict() for trial in self.test.trials]

        return res


if __name__ == "__main__":
    app = QCoreApplication()

    model = BPModel(session=None, config=None)

    db = BPDatabase(Path(Path.cwd() / "devices/blood_pressure/tests/DataBase.db"))

    try:
        if db.open():
            records = db.get_measurements(patient_key=6)
            model.read_results(records)

            print(json.dumps(model.to_response(), indent=4))
    except Exception as e:
        print(e)
        print(traceback.print_exc())
        pass
    finally:
        db.close()

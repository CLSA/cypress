from typing import override

from model import Model
from measure import Record

from devices.blood_pressure.session import BPSession
from devices.blood_pressure.config import BPConfig


class BPMeasurement(Record):
    field_map = {
        "SYS": {"attr": "systolic", "data_type": int, "units": "mmHg"},
        "DIA": {"attr": "diastolic", "data_type": int, "units": "mmHg"},
        "HR": {"attr": "pulse", "data_type": int, "units": "bpm"},
        "Spare7": {"attr": "reading_number", "data_type": int, "units": None},
    }


class BPTest:
    def __init__(self, measures: list[BPMeasurement]):
        self.measures = measures

    def get_average(self) -> dict:
        n = len(self.measures) - 1
        if n < 1:
            return {}

        systolic_sum = 0
        diastolic_sum = 0
        pulse_sum = 0

        for measure in self.measures[1:]:
            systolic_sum += measure.systolic["value"]
            diastolic_sum += measure.diastolic["value"]
            pulse_sum += measure.pulse["value"]

        avg_systolic = systolic_sum / n
        avg_diastolic = diastolic_sum / n
        avg_pulse = pulse_sum / n

        return {
            "avg_count": n,
            "avg_systolic": {"value": avg_systolic, "units": "mmHg"},
            "avg_diastolic": {"value": avg_diastolic, "units": "mmHg"},
            "avg_pulse": {"value": avg_pulse, "units": "bpm"},
        }

    def get_total_average(self) -> dict:
        n = len(self.measures)
        if n < 1:
            return {}

        systolic_sum = 0
        diastolic_sum = 0
        pulse_sum = 0

        for measure in self.measures:
            systolic_sum += measure.systolic["value"]
            diastolic_sum += measure.diastolic["value"]
            pulse_sum += measure.pulse["value"]

        avg_systolic = systolic_sum / n
        avg_diastolic = diastolic_sum / n
        avg_pulse = pulse_sum / n

        return {
            "total_avg_count": n,
            "total_avg_systolic": {"value": avg_systolic, "units": "mmHg"},
            "total_avg_diastolic": {"value": avg_diastolic, "units": "mmHg"},
            "total_avg_pulse": {"value": avg_pulse, "units": "bpm"},
        }

    def get_first(self) -> dict:
        return {
            "first_systolic": self.measures[0].systolic,
            "first_diastolic": self.measures[0].diastolic,
            "first_pulse": self.measures[0].pulse,
            # "first_end_time": self.measures[0].start_time,
            # "first_start_time": self.measures[0].end_time,
        }

    @override
    def to_dict(self):
        return {**self.get_first(), **self.get_average(), **self.get_total_average()}


class BPModel(Model):
    def __init__(self, session: BPSession, config: BPConfig):
        super().__init__(session, config)

    def read_results(self, db_rows) -> tuple[bool, str | None]:
        self.reset()

        measures = []
        for db_row in db_rows:
            measures.append(BPMeasurement(db_row))

        self.test = BPTest(measures=measures)

        return True, None

    def set_manual_values(self, data_received: list[dict]):
        self.reset()

        self.manual_entry = True

        measures = []
        for manual_measure in data_received:
            measures.append(BPMeasurement(manual_measure))

        self.test = BPTest(measures=measures)

    @override
    def reset(self):
        super().reset()
        self.test = None
        self.manual_entry = False

    @override
    def to_response(self) -> dict:
        if not self.test:
            return None

        res = super().to_response()

        res["value"]["metadata"] = self.test.to_dict()
        res["value"]["results"] = [measure.to_dict() for measure in self.test.measures]

        return res

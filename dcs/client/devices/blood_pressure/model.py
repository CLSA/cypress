import json
import logging
import datetime

from typing import override

from model import Model
from measure import Record
from devices.blood_pressure.session import BPSession
from devices.blood_pressure.config import BPConfig

from utils import round_half_up


logger = logging.getLogger("blood_pressure")


class BPMeasurement(Record):
    field_map = {
        "SYS": {"attr": "systolic", "data_type": int, "units": "mmHg"},
        "DIA": {"attr": "diastolic", "data_type": int, "units": "mmHg"},
        "HR": {"attr": "pulse", "data_type": int, "units": "bpm"},
        "Spare7": {"attr": "reading_number", "data_type": int, "units": None},
    }

    def __init__(self, raw_data):
        super().__init__(raw_data)
        self.raw_data = raw_data

    def is_valid(self):
        if not self.raw_data:
            return False

        if self.raw_data.get("CODE"):
            logger.warning(f"code: '{self.raw_data.get("CODE")}'")
            return False

        if not self.systolic:
            return False

        if not self.diastolic:
            return False

        if not self.pulse:
            return False

        return all(
            [
                self.systolic.get("value", 0) > 0,
                self.diastolic.get("value", 0) > 0,
                self.pulse.get("value", 0) > 0,
            ]
        )

    @override
    def to_dict(self):
        res = super().to_dict()

        for key, value in self.raw_data.items():
            if key == "Date":
                res["date"] = datetime.datetime.fromtimestamp(value).isoformat()

            res[key] = value

        return dict(sorted(res.items()))


class BPTest:
    def __init__(self, measures: list[BPMeasurement]):
        self.measures = measures

    def get_average(self) -> dict:
        n = len(self.measures) - 1
        if n < 1:
            logger.debug(f"get_average: {n} < 1")
            return {}

        systolic_sum = 0
        diastolic_sum = 0
        pulse_sum = 0

        for measure in self.measures[1:]:
            systolic_sum += measure.systolic["value"]
            diastolic_sum += measure.diastolic["value"]
            pulse_sum += measure.pulse["value"]

        avg_systolic = int(round_half_up(systolic_sum / n))
        avg_diastolic = int(round_half_up(diastolic_sum / n))
        avg_pulse = int(round_half_up(pulse_sum / n))

        logger.debug(
            f"avg_count: {n} "
            f"avg_systolic: {avg_systolic} "
            f"avg_diastolic: {avg_diastolic} "
            f"avg_pulse: {avg_pulse}"
        )

        return {
            "avg_count": n,
            "avg_systolic": {"value": avg_systolic, "units": "mmHg"},
            "avg_diastolic": {"value": avg_diastolic, "units": "mmHg"},
            "avg_pulse": {"value": avg_pulse, "units": "bpm"},
        }

    def get_total_average(self) -> dict:
        n = len(self.measures)
        if n < 1:
            logger.debug(f"get_total_average: {n} < 1")
            return {}

        systolic_sum = 0
        diastolic_sum = 0
        pulse_sum = 0

        for measure in self.measures:
            systolic_sum += measure.systolic["value"]
            diastolic_sum += measure.diastolic["value"]
            pulse_sum += measure.pulse["value"]

        avg_systolic = int(round_half_up(systolic_sum / n))
        avg_diastolic = int(round_half_up(diastolic_sum / n))
        avg_pulse = int(round_half_up(pulse_sum / n))

        logger.debug(
            f"total_avg_count: {n} "
            f"total_avg_systolic: {avg_systolic} "
            f"total_avg_diastolic: {avg_diastolic} "
            f"total_avg_pulse: {avg_pulse}"
        )

        return {
            "total_avg_count": n,
            "total_avg_systolic": {"value": avg_systolic, "units": "mmHg"},
            "total_avg_diastolic": {"value": avg_diastolic, "units": "mmHg"},
            "total_avg_pulse": {"value": avg_pulse, "units": "bpm"},
        }

    def get_first(self) -> dict:
        if not self.measures:
            return {}

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

        self.test = BPTest([])

    def read_results(self, db_rows: list[dict]) -> tuple[bool, str | None]:
        logger.debug(f"read_results: {json.dumps(db_rows, indent=4)}")
        self.reset()
        try:
            measures = []
            if not db_rows:
                logger.error("no db rows found")
                return False, None

            for db_row in db_rows:
                measure = BPMeasurement(db_row)
                if measure.is_valid():
                    measures.append(measure)
                else:
                    logger.warning("invalid measure")

            self.test = BPTest(measures=measures)

            return True, None

        except Exception as e:
            logger.error(f"bp_model - read results: {e}")
            return False, None

    def is_valid(self) -> bool:
        try:
            if not self.test.measures:
                logger.debug("not valid: no measures")
                return False

            if not len(self.test.measures) >= 2:
                return False

            for measure in self.test.measures:
                if not measure.is_valid():
                    logger.debug(f"invalid measure found")
                    return False

            return True

        except Exception as e:
            logger.error(e)
            return False

    def set_manual_entry_data(self, data_received: list[dict]):
        logger.debug(f"set_manual_entry_data: {json.dumps(data_received, indent=4)}")

        self.reset()
        self.manual_entry = True

        measures = []
        for manual_measure in data_received:
            measures.append(BPMeasurement(manual_measure))

        self.test = BPTest(measures=measures)

    def set_manual_entry(self):
        if not self.manual_entry:
            self.reset()

        self.manual_entry = True

    @override
    def reset(self):
        super().reset()
        self.test = BPTest([])
        self.manual_entry = False

    @override
    def to_response(self) -> dict:
        res = super().to_response()

        res["value"]["metadata"] = self.test.to_dict()
        res["value"]["results"] = [measure.to_dict() for measure in self.test.measures]

        return res

import logging

import datetime

from typing import override
from copy import deepcopy

from model import Model
from measure import Record

from devices.weigh_scale.settings import DEVICE_NAME
from devices.weigh_scale.session import WeighScaleSession
from devices.weigh_scale.config import WeighScaleConfig

logger = logging.getLogger(DEVICE_NAME)


class WeightMeasure(Record):
    field_map = {
        "weight": {"attr": "weight", "data_type": float, "units": "kg"},
        "mode": {"attr": "mode", "data_type": str, "units": None},
        "timestamp": {"attr": "timestamp", "data_type": str, "units": None},
    }

    @override
    def to_dict(self):
        return {
            "mode": self.mode,
            "weight": self.weight,
            "timestamp": self.timestamp,
        }


class WeighScaleTest:
    def __init__(self, measures: list[WeightMeasure] = []):
        self.metadata = {}
        self.measures = measures
        self._update_average()

    def add_measurement(self, weight: float, unit: str, mode: str):
        if unit != "kg":
            raise ValueError(f"unit must be kg: {unit}")

        timestamp = datetime.datetime.now().isoformat()
        self.measures.append(
            WeightMeasure(
                raw_data={
                    "weight": weight,
                    "mode": mode,
                    "timestamp": timestamp,
                }
            )
        )

        logger.debug(f"measure added: {weight} {unit} {mode}")

        self._update_average()

        return True, None

    def remove_measurement(self, index):
        del self.measures[index]
        self._update_average()

    def _update_average(self):
        logger.debug("_update_average")

        n = len(self.measures)
        if n < 1:
            return

        total = sum([x.weight["value"] for x in self.measures])
        logger.debug(total)

        self.metadata["average_weight"] = {
            "units": "kg",
            "value": total / n,
        }

    def to_dict(self):
        return self.metadata.copy()


class WeighScaleModel(Model):
    def __init__(self, session: WeighScaleSession, config: WeighScaleConfig):
        super().__init__(session=session, config=config)

        self.test = WeighScaleTest()

    def is_valid(self) -> bool:
        has_test = self.test is not None
        two_measures = len(self.test.measures) == 2
        has_avg_weight = "average_weight" in self.test.to_dict()

        has_zero_weight = any([measure.weight["value"] == 0.0 for measure in self.test.measures])

        return (
            has_test
            and two_measures
            and "average_weight" in self.test.to_dict()
            and not has_zero_weight
            and has_avg_weight
        )

    def add_measurement(
        self, weight: float, unit: str, mode: str
    ) -> tuple[bool, str | None]:
        if unit != "kg":
            return False, "kg"

        if weight == 0:
            return False, "Weight is zero"

        try:
            self.test.add_measurement(weight, unit, mode)
            return True, None
        except ValueError as e:
            logger.error(e)
            return False, "kg"
        except Exception as e:
            logger.error(e)
            return False, "Unknown"

    def delete_measurement(self, index):
        self.test.remove_measurement(index)

    def set_manual_entry_data(self, data: list[tuple[float, str]]):
        self.reset()
        self.manual_entry = True

        timestamp = datetime.datetime.now().isoformat()

        measures: list[WeightMeasure] = []
        for measure in data:
            measures.append(
                WeightMeasure(
                    {
                        "weight": measure[0],
                        "mode": "Gross",
                        "timestamp": timestamp,
                    }
                )
            )

        self.test = WeighScaleTest(measures)

    def set_manual_entry(self):
        if not self.manual_entry:
            self.reset()
        self.manual_entry = True

    @override
    def reset(self):
        super().reset()
        self.test = WeighScaleTest()
        self.manual_entry = False

    @override
    def to_response(self):
        response = super().to_response()

        response["value"]["results"] = [
            measure.to_dict() for measure in self.test.measures
        ]

        response["value"]["metadata"] = self.test.to_dict()

        return response

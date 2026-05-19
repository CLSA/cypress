import logging
from typing import override
from copy import deepcopy

from devices.model import Model

from devices.audiometer.session import AudiometerSession
from devices.audiometer.config import AudiometerConfig

logger = logging.getLogger("audiometer")


class AudiometerModel(Model):
    def __init__(self, session: AudiometerSession, config: AudiometerConfig):
        super().__init__(session=session, config=config)

    def is_valid(self, data) -> bool:
        logger.debug(f"{self.class_name()}::is_valid")
        if len(data) < 14:
            return False

        return True

    def read_output(self) -> bool:
        logger.debug(f"{self.class_name()}::read_output")
        self.reset()

    def set_manual_values(self, data_entered: dict) -> bool:
        """
        Expects data entered to be a dictionary:
        {
            "left": {
                "[Freq]Hz": int
                ...
            },
            "right": {
                "[Freq]Hz": int
                ...
            }
        }
        """
        logger.debug(f"{self.class_name()}::set_manual_entry")
        self.reset()

        self.manual_entry = True

        def read_manual_values(side: str):
            for name, value in data_entered[side].items():
                self.results.append(
                    {
                        "side": side,
                        "test": f"{name.lower().replace("hz", "")} Hz",
                        "error": "",
                        "level": {
                            "value": value,
                            "units": "db",
                        },
                        "passed": value <= 40,
                    }
                )

        read_manual_values("left")
        read_manual_values("right")

    @override
    def to_response(self):
        response = deepcopy(super().to_response())

        results = deepcopy(response["value"]["results"])

        del response["value"]["results"]

        left_results = []
        right_results = []

        for result in results:
            side = result["side"]
            del result["side"]
            if side == "left":
                left_results.append(result)
            elif side == "right":
                right_results.append(result)
            else:
                raise Exception(f"invalid side '{result.side}' for result")

        response["value"]["results"] = {
            "left": deepcopy(left_results),
            "right": deepcopy(right_results),
        }

        return response

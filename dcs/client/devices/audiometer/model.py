import logging
import json

from typing import override
from copy import deepcopy

from model import Model

from devices.audiometer.session import AudiometerSession
from devices.audiometer.config import AudiometerConfig

logger = logging.getLogger("audiometer")


left_keys = {
    "l500": "500 Hz",
    "l1000": "1000 Hz",
    "l2000": "2000 Hz",
    "l3000": "3000 Hz",
    "l4000": "4000 Hz",
    "l6000": "6000 Hz",
    "l8000": "8000 Hz"
}

right_keys = {
    "r500": "500 Hz",
    "r1000": "1000 Hz",
    "r2000": "2000 Hz",
    "r3000": "3000 Hz",
    "r4000": "4000 Hz",
    "r6000": "6000 Hz",
    "r8000": "8000 Hz"
}

class AudiometerModel(Model):
    def __init__(self, session: AudiometerSession, config: AudiometerConfig):
        super().__init__(session=session, config=config)

    def is_valid(self) -> bool:
        logger.debug(f"is_valid")

        if len(self.results) < 14:
            return False

        return True

    def parse_output_json(self, output_json: dict) -> bool:
        self.reset()

        logger.debug(json.dumps(output_json, indent=4))

        self.metadata = output_json

        for key, name in left_keys.items():
            self.results.append(
                {
                    "side": "left",
                    "test": name,
                    "error": "",
                    "level": {
                        "value": output_json[key],
                        "units": "db",
                    },
                    "passed": output_json[key] <= 40,
                }
            )

        for key, name in right_keys.items():
            self.results.append(
                {
                    "side": "right",
                    "test": name,
                    "error": "",
                    "level": {
                        "value": output_json[key],
                        "units": "db",
                    },
                    "passed": output_json[key] <= 40,
                }
            )

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
        logger.debug(f"set_manual_entry")
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
        response = super().to_response()

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

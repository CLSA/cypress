from pathlib import Path

import csv
import json
import sys

from devices.model import Model
from devices.crt.session import CRTSession

class CRTModel(Model):

    def __init__(self, session):
        super().__init__(session)
        self.output = []

    def read_results(self, path: Path):
        self.clear()

        try:
            with open(path, "r") as f:
                reader = csv.reader(f)
                for row in reader:
                    self.output.append(row)

            self._parse_results()

        except Exception as e:
            print(f"error: {e}")
            return False

        return True

    def _parse_results(self):
        if not self.output:
            return False

        self.results.clear()
        self.metadata.clear()

        try:
            clinic_index = -1
            version_index = -1
            userid_index = -1
            interviewer_id_index = -1

            for index, row in enumerate(self.output):
                if not row:
                    raise Exception(f"row is empty")

                test_code = int(row[0])

                if test_code == 10:  # Header
                    clinic_index = row.index("Clinic")
                    version_index = row.index("Version")
                    userid_index = row.index("UserId")
                    interviewer_id_index = row.index("InterviewerId")

                    if not clinic_index:
                        raise Exception("no clinic index found")
                    if not version_index:
                        raise Exception("no version index found")
                    if not userid_index:
                        raise Exception("no userid index found")
                    if not interviewer_id_index:
                        raise Exception("no interviewerid index found")

                elif test_code == 11:  # Measurement
                    if len(row) != 15:
                        raise Exception("length of row is not 15")

                    screen_id = row[7]
                    response_correct = int(row[8])
                    elapsed_time = {"value": int(row[9]), "unit": "ms"}
                    correct_position = row[10].lower()
                    response_stimulus_interval = {"value": int(row[11]), "unit": "ms"}

                    self.results.append(
                        {
                            "screen_id": screen_id,
                            "response_correct": response_correct,
                            "elapsed_time": elapsed_time,
                            "correct_position": correct_position,
                            "response_stimulus_interval": response_stimulus_interval,
                        }
                    )

                elif test_code == 12:  # Last line
                    self.metadata["version"] = row[version_index]
                    self.metadata["clinic"] = row[clinic_index]
                    self.metadata["user_id"] = row[userid_index]
                    self.metadata["interviewer_id"] = row[interviewer_id_index]

                else:
                    raise Exception("unknown line code")

        except Exception as e:
            print(f"error on row {index}: {e}")
            return False

        return True

    def clear(self):
        self.output.clear()
        self.results.clear()
        self.metadata.clear()


if __name__ == "__main__":
    if len(sys.argv) < 2:
        sys.exit(-1)

    model = CRTModel()
    model.read_results(Path(sys.argv[1]))

    print(json.dumps(model.get_response(), indent=4))

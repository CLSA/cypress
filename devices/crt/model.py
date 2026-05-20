import csv
import json
import sys
import logging

from pathlib import Path
from datetime import date

from devices.utils import get_file_info
from devices.model import Model

logger = logging.getLogger("crt")


class CRTModel(Model):
    def __init__(self, session, config):
        super().__init__(session, config)
        self.output = []

    def read_results(self):
        # find the file
        # parse the file
        logger.debug(f"CRTModel::read_results")

        self.reset()

        try:
            path: Path = self._find_result_file()
            if not path:
                logger.critical("output filepath not found")
                return False

            with open(path, "r") as f:
                reader = csv.reader(f)
                for row in reader:
                    self.output.append(row)

            self._parse_results()

        except Exception as error:
            logger.critical(error)
            return False

        return True

    def _find_result_file(self) -> Path | None:
        logger.debug("CRTModel::_find_result_file")

        results_filename = f"CLSA_ELCV_{self.config.clinic}_{date.today().strftime('%Y%m%d')}.csv"
        result_filepath = None
        for path in self.config.output.iterdir():
            if path.is_file():
                if path.name == results_filename:
                    result_filepath = path

        if not result_filepath:
            return None

        logger.debug(f"found {str(result_filepath.resolve())}")

        file_info = get_file_info(result_filepath)
        file_info.send_name = "data"

        self._add_file(file_info)

        return result_filepath

    def _parse_results(self):
        logger.debug("CRTModel::_parse_results")

        if not self.output:
            return False

        try:
            clinic_index = -1
            version_index = -1
            userid_index = -1
            interviewer_id_index = -1

            for row in self.output:
                if not row:
                    raise Exception(f"row is empty")

                test_code = int(row[0])

                if test_code == 10:  # Header
                    if "Clinic" not in row:
                        raise KeyError("no clinic index found")

                    if "Version" not in row:
                        raise KeyError("no version index found")

                    if "UserId" not in row:
                        raise KeyError("no userid index found")

                    if "InterviewerId" not in row:
                        raise KeyError("no interviewer id index found")

                    if "UserStartDateTime" not in row:
                        raise KeyError("no user start date time index found")

                    clinic_index = row.index("Clinic")
                    version_index = row.index("Version")
                    userid_index = row.index("UserId")
                    interviewer_id_index = row.index("InterviewerId")
                    user_start_date_time_index = row.index("UserStartDateTime")

                elif test_code == 11:  # Measurement
                    if len(row) != 15:
                        raise Exception("length of row is not 15")

                    screen_id = row[7]
                    response_correct = int(row[8])
                    elapsed_time = {"value": int(row[9]), "unit": "ms"}
                    correct_position = row[10].lower()
                    response_stimulus_interval = {"value": int(row[11]), "unit": "ms"}

                    clinic = row[clinic_index]
                    version = row[version_index]
                    userid = row[userid_index]
                    interviewer_id = row[interviewer_id_index]
                    start_time = row[user_start_date_time_index]

                    self.metadata["clinic"] = clinic
                    self.metadata["version"] = version
                    self.metadata["user_id"] = userid
                    self.metadata["interviewer_id"] = interviewer_id
                    self.metadata["start_time"] = start_time

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
                    self.metadata["end_date_time"] = row[10]
                else:
                    raise Exception("unknown line code")

            self.metadata["number_of_measurements"] = len(self.results)

        except Exception as error:
            logger.critical(error)
            return False

        return True


if __name__ == "__main__":
    if len(sys.argv) < 2:
        sys.exit(-1)

    model = CRTModel()
    model.read_results(Path(sys.argv[1]))

    print(json.dumps(model.get_response(), indent=4))

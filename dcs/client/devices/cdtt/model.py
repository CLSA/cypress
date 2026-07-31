import json
import logging

from typing import override

from pathlib import Path
from openpyxl import load_workbook

from utils import get_file_info

from model import Model
from measure import Record

from devices.cdtt.config import CDTTConfig
from devices.cdtt.session import CDTTSession

NUM_TRIALS = 24

logger = logging.getLogger("cdtt")


class CDTTTest(Record):
    field_map = {
        "Date & time": {"attr": "datetime", "data_type": str, "units": None},
        "Digits": {"attr": "digits", "data_type": str, "units": None},
        "Language": {"attr": "language", "data_type": str, "units": None},
        "List #": {"attr": "list_number", "data_type": int, "units": None},
        "Mode": {"attr": "mode", "data_type": str, "units": None},
        "MSK level": {"attr": "msk_level", "data_type": float, "units": "dB"},
        "MSK signal": {"attr": "msk_signal", "data_type": str, "units": None},
        "Reversals": {"attr": "reversal_count", "data_type": int, "units": None},
        "SP level": {"attr": "sp_level", "data_type": float, "units": "dB"},
        "SRT": {
            "attr": "speech_reception_threshold",
            "data_type": float,
            "units": "dB",
        },
        "St. Dev.": {"attr": "standard_deviation", "data_type": float, "units": "dB"},
        "Subject ID:": {"attr": "subject_id", "data_type": str, "units": None},
        "Talker": {"attr": "talker", "data_type": str, "units": None},
        "Test Ear": {"attr": "test_ear", "data_type": str, "units": None},
    }


class CDTTMeasure(Record):
    def __init__(self, raw_data):
        self.trial = raw_data.get("trial", None)
        self.stimulus = raw_data.get("stimulus_digits", [])
        self.response = raw_data.get("response_digits", [])

    @override
    def to_dict(self):
        return {
            "trial": self.trial,
            "stimulus": self.stimulus,
            "response": self.response,
        }


class CDTTModel(Model):
    def __init__(self, session: CDTTSession, config: CDTTConfig):
        super().__init__(session, config)
        self.output_filepath: Path | None = None

    def is_valid(self) -> tuple[bool, str | None]:
        if len(self.measures) < NUM_TRIALS:
            return False, "Test was not completed"

        if self.test.subject_id != self.session.barcode:
            return (
                False,
                f"Subject ID: {self.test.subject_id} does not match {self.session.barcode}",
            )

        return True, None

    def read_output(self, output_file: Path) -> tuple[bool, str | None]:
        self.reset()

        self.test = None
        self.measures = []

        try:
            if not output_file.exists():
                return False, "Output file does not exist"

            file_info = get_file_info(output_file)
            file_info.send_name = "cdtt"
            self._add_file(file_info)

            wb = load_workbook(str(output_file.resolve()))

            test_record = {}
            success, error = self._read_barcode(wb, test_record)
            if not success:
                logger.error(error)
                return False, f"Failed to read barcode"

            success, error = self._read_metadata(wb, test_record)
            if not success:
                logger.error(error)
                return False, f"Failed to read metadata"

            success, error = self._read_summary(wb, test_record)
            if not success:
                logger.error(error)
                return False, f"Failed to read summary"

            self.test = CDTTTest(test_record)

            measure_records = []
            success, error = self._read_trial_data(wb, measure_records)
            if not success:
                logger.error(error)
                return False, f"Failed to read trials"

            self.measures = [CDTTMeasure(record) for record in measure_records]

        except FileNotFoundError as e:
            return False, f"File not found"

        except PermissionError as e:
            return False, f"File permission error"

        except (ValueError, IndexError) as e:
            return False, f"Parsing error"

        except Exception as e:
           logger.error(e)
           return False, "Unknown exception"

        return True, None

    def _read_barcode(self, wb, record: dict) -> tuple[bool, str | None]:
        # Read cells A1 and B1 from main
        sheet = wb.active

        barcode_col = sheet["A1"].value
        barcode_val = sheet["B1"].value

        if not barcode_col:
            return False, "No barcode column"

        if not barcode_val:
            return False, "No barcode value"

        record[barcode_col] = barcode_val

        return True, None

    def _read_metadata(self, wb, record: dict) -> tuple[bool, str | None]:
        # Read cells A4 to J5 from main
        sheet = wb.active

        query = sheet["A4:J5"]
        if len(query) < 2:
            return False, "Invalid metadata size"

        headers = []
        for cell in query[0]:
            headers.append(cell.value)

        values = []
        for cell in query[1]:
            values.append(cell.value)

        for header, value in zip(headers, values):
            record[header] = value

        return True, None

    def _read_summary(self, wb, record: dict) -> tuple[bool, str | None]:
        # Read cells K4 to M5 from main
        sheet = wb.active

        query = sheet["K4:M5"]
        if len(query) < 2:
            return False, "Invalid summary size"

        headers = []
        values = []

        for cell in query[0]:
            headers.append(cell.value)

        for cell in query[1]:
            values.append(cell.value)

        for header, value in zip(headers, values):
            record[header] = value

        return True, None

    def _read_trial_data(self, wb, measure_records: list) -> tuple[bool, str | None]:
        # Read trials A13 from sheet [FR_CA | EN_CA]-[Male | Female]
        #
        language = "EN" if self.session.language == "en" else "FR"

        sheet = wb[f"{language}_CA-Male"]

        query = sheet["A13:G36"]

        for row in query:
            trial = int(row[0].value)
            stimulus_digits = [int(cell.value) for cell in row[1:4]]
            response_digits = [int(cell.value) for cell in row[4:7]]

            measure_records.append(
                {
                    "trial": trial,
                    "stimulus_digits": stimulus_digits,
                    "response_digits": response_digits,
                }
            )

        return True, None

    def to_response(self):
        res = super().to_response()

        res["value"]["metadata"] = self.test.to_dict() if self.test else {}
        res["value"]["metadata"]["trial_count"] = (
            len(self.measures) if self.measures else 0
        )
        res["value"]["results"] = (
            [measure.to_dict() for measure in self.measures] if self.measures else []
        )

        return res

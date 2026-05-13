import json
import copy
import sys

from pathlib import Path
from openpyxl import load_workbook

from devices.model import Model

NUM_TRIALS = 24

class CDTTModel(Model):
    output_file_xlsx_path: Path | None = None

    def __init__(self, session, config):
        super().__init__(session, config)
        self.trials = []

    def is_valid(self, barcode: str) -> bool:
        if len(self.trials) < NUM_TRIALS:
            return False

        if self.metadata["Subject ID:"] != barcode:
            print(
                f"subject id {self.metadata['Subject ID:']} does not match {barcode}",
                self.metadata["Subject ID"],
                barcode,
            )
            return False

        return True

    def clear(self) -> None:
        self.metadata.clear()
        self.trials.clear()

    def read_output(self, output_file: Path, language: str) -> bool:
        self.clear()

        try:
            wb = load_workbook(str(output_file.resolve()))
            if not self._read_barcode(wb):
                print(f"could not read barcode..")
                return False
            if not self._read_metadata(wb):
                print(f"could not read metadata..")
                return False
            if not self._read_summary(wb):
                print(f"could not read summary..")
                return False
            if not self._read_trial_data(wb, language):
                print(f"could not read trials..")
                return False
        except FileNotFoundError as e:
            print(e)
            return False
        except PermissionError as e:
            print(e)
            return False
        except (ValueError, IndexError) as e:
            print(e)
            return False
        except Exception as e:
            print(e)
            return False

        return True

    def _read_barcode(self, wb) -> bool:
        # Read cells A1 and B1 from main
        sheet = wb.active

        barcode_col = sheet["A1"].value
        barcode_val = sheet["B1"].value

        self.metadata[barcode_col] = barcode_val

        return True

    def _read_metadata(self, wb) -> bool:
        # Read cells A4 to J5 from main
        sheet = wb.active

        query = sheet["A4:J5"]
        if len(query) < 2:
            return False

        headers = []
        for cell in query[0]:
            headers.append(cell.value)

        values = []
        for cell in query[1]:
            values.append(cell.value)

        for header, value in zip(headers, values):
            self.metadata[header] = value

        return True

    def _read_summary(self, wb) -> bool:
        # Read cells K4 to M5 from main
        sheet = wb.active

        query = sheet["K4:M5"]
        headers = []
        values = []

        for cell in query[0]:
            headers.append(cell.value)

        for cell in query[1]:
            values.append(cell.value)

        for header, value in zip(headers, values):
            self.metadata[header] = value

        return True

    def _read_trial_data(self, wb, language: str) -> bool:
        # Read trials A13 from sheet [FR_CA | EN_CA]-[Male | Female]
        #
        language = "EN" if language == "en" else "FR"

        sheet = wb[f"{language}_CA-Male"]

        query = sheet["A13:G36"]

        for row in query:
            trial = int(row[0].value)
            stimulus_digits = [int(cell.value) for cell in row[1:4]]
            response_digits = [int(cell.value) for cell in row[4:7]]

            self.trials.append(
                {
                    "trial": trial,
                    "stimulus_digits": stimulus_digits,
                    "response_digits": response_digits,
                }
            )
            # print(trial, stimulus_digits, response_digits)

        return True

    def get_response(self):
        output = {"metadata": {}, "results": []}

        for key, value in self.metadata.items():
            key_cleaned = (
                key.lower()
                .strip()
                .replace(" ", "_")
                .replace(".", "")
                .replace("&", "and")
                .replace("#", "number")
                .replace(":", "")
            )

            output["metadata"][key_cleaned] = value

        output["results"] = copy.deepcopy(self.trials)

        return output


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("usage: data directory")
        sys.exit(-1)

    directory = Path(sys.argv[1])
    if not directory.exists():
        print(f"error: {directory} does not exist")
        sys.exit(-1)

    for uid_dir_path in directory.iterdir():
        if uid_dir_path.is_dir():
            uid = uid_dir_path.name

            model = CDTTModel()

            ok = model.read_output(uid_dir_path / 'cdtt.xlsx')

            assert len(model.trials) == 24

            print(json.dumps(model.get_response(), indent=2))

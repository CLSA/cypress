from pathlib import Path

from PySide6.QtCore import QProcess

from devices.model import Model
from devices.frax.session import FRAXSession


class FRAXModel(Model):
    def __init__(self, session: FRAXSession, config):
        super().__init__(config)

        self.session = session
        self.input_line = ""
        self.output_line = ""
        self.output = []
        self.metadata = {}
        self.results = []

    def write_input_file(self, path: Path) -> tuple[str, bool]:
        try:
            with open(path, "w") as f:
                self.input_line = self._get_input_file_values()
                f.write(self.input_line)
                return True, self.input_line

        except Exception as e:
            print(f"error writing input file: {e}")
            return False, ""

    def _get_input_file_values(self) -> str:
        arguments = []

        arguments.append(self.session.test_type)
        arguments.append(self.session.country_code)
        arguments.append(str(self.session.age))

        arguments.append("0" if self.session.sex == "male" else "1")

        arguments.append(str(self.session.body_mass_index))
        arguments.append(str(int(self.session.previous_fracture)))
        arguments.append(str(int(self.session.parent_hip_fracture)))
        arguments.append(str(int(self.session.current_smoker)))
        arguments.append(str(int(self.session.glucocorticoid)))
        arguments.append(str(int(self.session.rheumatoid_arthritis)))
        arguments.append(str(int(False)))
        # arguments.append('_')
        arguments.append(str(int(self.session.alcohol)))
        arguments.append(str(self.session.femoral_neck_tscore))

        contents = ",".join(arguments)
        return contents

    def run(self, executable: Path, working_directory: Path):
        process = QProcess()
        process.setProgram(str(executable.resolve()))
        process.setWorkingDirectory(str(working_directory.resolve()))
        process.start()
        process.waitForFinished()

        return True

    def read_output_file(self, path: Path) -> tuple[str, bool]:
        self.reset()

        try:
            with open(path, "r") as f:
                self.output_line = f.readline()
                parts = self.output_line.split(",")
                for part in parts:
                    self.output.append(part.strip())

                self._parse_results()

                return True, self.output_line

        except Exception as e:
            print(f"error parsing output file: {e}")
            return False, ""

        return False, ""

    def _parse_results(self):
        if not self.output:
            print(f"error: frax output is empty")
            return

        if not len(self.output) == 17:
            print(f"error: frax result length is invalid")
            return

        self.metadata["type"] = self.output[0].lower()
        self.metadata["country_code"] = int(self.output[1])
        self.metadata["age"] = {"value": float(self.output[2]), "unit": "yr"}
        self.metadata["sex"] = int(self.output[3])
        self.metadata["body_mass_index"] = {
            "value": float(self.output[4]),
            "unit": "kg/m2",
        }
        self.metadata["previous_fracture"] = int(self.output[5])
        self.metadata["parent_hip_fracture"] = int(self.output[6])
        self.metadata["current_smoker"] = int(self.output[7])
        self.metadata["glucocorticoid"] = int(self.output[8])
        self.metadata["rheumatoid_arthritis"] = int(self.output[9])
        self.metadata["secondary_osteoporosis"] = int(self.output[10])
        self.metadata["alcohol"] = int(self.output[11])
        self.metadata["femoral_neck_tscore"] = float(self.output[12])

        self.results.append(
            {
                "type": "osteoporotic_fracture",
                "probability": {"value": float(self.output[13]), "unit": "%"},
            }
        )

        self.results.append(
            {
                "type": "hip_fracture",
                "probability": {"value": float(self.output[14]), "unit": "%"},
            }
        )

        self.results.append(
            {
                "type": "osteoporotic_fracture_bmd",
                "probability": {"value": float(self.output[15]), "unit": "%"},
            }
        )

        self.results.append(
            {
                "type": "hip_fracture_bmd",
                "probability": {"value": float(self.output[16]), "unit": "%"},
            }
        )

        interpretation = self.interpret_result(
            p=float(self.output[15]),
            previous_fracture=bool(self.metadata["previous_fracture"]),
            femoral_neck_tscore=float(self.metadata["femoral_neck_tscore"]),
        )
        self.metadata["osteoporotic_fracture_bmd_interp"] = interpretation

    def interpret_result(
        self, p: float, femoral_neck_tscore: float, previous_fracture: bool
    ):
        interpretation = "N/A"

        if p > 20.0 or previous_fracture:
            interpretation = "High"

        elif (p >= 10.0 and p <= 20.0) or femoral_neck_tscore <= -2.5:
            interpretation = "Moderate"

        elif p < 10:
            interpretation = "Low"

        return interpretation

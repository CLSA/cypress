import json
import logging
import xml.etree.ElementTree as ET

from typing import override

from pathlib import Path
from model import Model

from devices.spirometer.session import SpirometerSession
from devices.spirometer.config import SpirometerConfig

from devices.spirometer.emr.output.patient import Patient
from devices.spirometer.emr.output.command import Command

logger = logging.getLogger("spirometer")

class SpirometerModel(Model):
    def __init__(self, session: SpirometerSession, config: SpirometerConfig):
        super().__init__(session=session, config=config)

    def is_valid(self):
        has_files = len(self.files) == 2
        has_pdf = any("pdf" in file_info.extension for file_info in self.files)
        has_data = any("xml" in file_info.extension for file_info in self.files)

        try:
            for file_info in self.files:
                with open(file_info.file_path, "r") as read_file:
                    read_file.read()
        except Exception as e:
            logger.error(f"is_valid: {e}")
            return False

        return all(has_files, has_pdf, has_data)

    def read_results(self, output_file: Path) -> bool:
        if not output_file.exists() or not output_file.is_file():
            raise FileNotFoundError(f"No XML file found at {str(output_file.resolve())}")

        try:
            tree = ET.parse(output_file)
        except ET.ParseError as e:
            raise ValueError(e)

        root = tree.getroot()
        command_el = root.find(".//Command")
        if command_el is None:
            raise ValueError("No command element found")

        self.command = Command(command_el)

        patients_el = root.find(".//Patients")
        if patients_el is None:
            raise ValueError("No patients element found")

        if len(patients_el) > 1:
            raise ValueError("More than one patient exists")

        lung_age = root.find(".//LungAge")
        self.metadata["lung_age"] = { "value": int(lung_age.text), "units": "yr" }
        self.patient = Patient(patients_el[0])

        self.add_file(output_file, "data", ".xml")
        self.add_file(Path(self.command.report_pdf), "report", ".pdf")

    @override
    def to_response(self):
        res = super().to_response()

        # patient_dict = self.patient.to_dict()
        # patient_dict["tests"] = []

        tests = self.patient.get_tests()
        test = sorted(tests, key=lambda test: test.quality_grade)[0]

        test_dict = test.to_dict()
        test_dict["trials"] = []

        device_data = test.get_device()

        best_values = test.get_best_values()
        best_values_dict = {}
        for result in best_values:
            best_values_dict.update(result.to_dict())

        res["value"]["metadata"]["original_quality_grade"] = test.original_quality_grade
        res["value"]["metadata"]["quality_grade"] = test.quality_grade

        res["value"]["metadata"]["test_type"] = test.test_type
        res["value"]["metadata"]["test_date"] = test.test_date

        patient_data_at_test_time = test.get_patient_data_at_test_time()
        res["value"]["metadata"].update(patient_data_at_test_time)

        res["value"]["best_values"] = best_values_dict
        res["value"]["device_data"] = device_data

        trials = []
        for trial in test.get_trials():
            trial_dict = trial.to_dict()

            for result in trial.get_results():
                trial_dict.update(result.to_dict())


            trials.append(trial_dict)

            #patient_dict["tests"].append(test_dict)

        res["value"]["results"] = trials

        return res


if __name__ == "__main__":
    session = SpirometerSession(
        **{
            "answer_id": 1,
            "barcode": "12345678",
            "interviewer": "ant",
            "uid": "12345678",
            "language": "en",
            "dob": "1995-12-06",
            "sex": "male",
            "weight": 50.0,
            "height": 140.0,
            "smoker": False,
        }
    )

    model = SpirometerModel(session=session, config=None)

    try:
        model.read_results(
            Path("data.xml")
        )
        print(json.dumps(model.to_response(), indent=4))
    except FileNotFoundError as e:
        print("error", e)
    except ValueError as e:
        print("error", e)
    except Exception as e:
        print("error", e)
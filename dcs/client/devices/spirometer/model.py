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


class SpirometerModel(Model):
    def __init__(self, session: SpirometerSession, config: SpirometerConfig):
        super().__init__(session=session, config=config)

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

        self.patient = Patient(patients_el[0])

        self.add_file(output_file, "data")
        self.add_file(Path(self.command.report_pdf), "report")

    @override
    def to_response(self):
        res = super().to_response()

        patient_dict = self.patient.to_dict()
        patient_dict["tests"] = []

        for test in self.patient.get_tests():
            test_dict = test.to_dict()
            test_dict["trials"] = []

            for trial in test.get_trials():
                trial_dict = trial.to_dict()

                for result in trial.get_results():
                    name = result.id
                    data_value = result.data_value
                    unit = result.unit
                    predicted_value = result.predicted_value
                    ll_normal_value = result.ll_normal_value
                    z_score = result.z_score

                    if unit:
                        trial_dict[name] = { "units": unit, "value": data_value }
                    else:
                        trial_dict[name] = data_value

                    if predicted_value:
                        if unit:
                            trial_dict[f"{name}_predicted"] = { "units": unit, "value": predicted_value }
                        else:
                            trial_dict[f"{name}_predicted"] = predicted_value

                    if ll_normal_value:
                        if unit:
                            trial_dict[f"{name}_ll_normal"] = { "units": unit, "value": ll_normal_value }
                        else:
                            trial_dict[f"{name}_ll_normal"] = ll_normal_value

                    if z_score:
                        trial_dict[f"{name}_z_score"] = z_score

                test_dict["trials"].append(trial_dict)

            patient_dict["tests"].append(test_dict)

        res["value"]["results"] = patient_dict

        return res


if __name__ == "__main__":
    session = SpirometerSession(
        **{
            "answer_id": 1,
            "barcode": "",
            "interviewer": "",
            "uid": "",
            "language": "en",
            "dob": "",
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
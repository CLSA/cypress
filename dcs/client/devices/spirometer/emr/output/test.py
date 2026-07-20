from typing import override

from devices.spirometer.emr.output.base import SpirometerXML
from devices.spirometer.emr.output.result_parameter import ResultParameter
from devices.spirometer.emr.output.trial import Trial


class Test(SpirometerXML):
    field_map = {
        "LungAge": {"key": "lung_age", "type": int},
        "SWVersion": {"key": "device_software_version", "type": str},
        "QualityGradeOriginal": {"key": "original_quality_grade", "type": str},
        "QualityGrade": {"key": "quality_grade", "type": str},
        "Test": {"key": "test_type", "type": str},
        "StageType": {"key": "stage_type", "type": str},
        "TestDate": {"key": "test_date", "type": str},
        "SerialNumber": {"key": "device_serial_number", "type": str},
        "DeviceType": {"key": "device_type", "type": str},
    }

    @override
    def _parse_xml(self, root):
        type_of_test = root.attrib.get("TypeOfTest", None)
        self.set_attribute("Test", type_of_test)

        stage_type = root.attrib.get("StageType", None)
        self.set_attribute("StageType", stage_type)

        for child in root:
            if child.tag in self.field_map:
                setattr(
                    self,
                    self.field_map[child.tag]["key"],
                    self.convert_type(child.text, self.field_map[child.tag]["type"]),
                )

            if child.tag == "Device":
                for sub_child in child:
                    if sub_child.tag in self.field_map:
                        self.set_attribute(sub_child.tag, sub_child.text)

            elif child.tag == "PatientDataAtTestTime":
                self.patient_data_at_test_time = {}
                for sub_child in child:
                    self.patient_data_at_test_time[sub_child.tag] = sub_child.text

            elif child.tag == "BestValues":
                self.best_values = []
                for sub_child in child:
                    if sub_child.tag == "ResultParameter":
                        self.best_values.append(ResultParameter(sub_child))

    def get_trials(self) -> list[Trial]:
        trial_els = self.root.find(".//Trials")

        res = []
        for trial_el in trial_els:
            trial = Trial(trial_el)
            res.append(trial)

        return res

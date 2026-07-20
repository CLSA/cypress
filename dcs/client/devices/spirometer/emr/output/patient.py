import logging
from typing import override

from devices.spirometer.settings import DEVICE_NAME
from devices.spirometer.emr.output.base import SpirometerXML
from devices.spirometer.emr.output.test import Test

logger = logging.getLogger(DEVICE_NAME)

class Patient(SpirometerXML):
    field_map = {
        "ID": {"key": "patient_id", "type": str},
        "OrderID": {"key": "order_id", "type": int },
        "TypeOfTest": {"key": "type_of_test", "type": str},
        "PftStandard": {"key": "pft_standard", "type": str},
        "ProtocolName": {"key": "protocol_name", "type": str},
        "Modified": {"key": "modified", "type": str},
        "LungAge": {"key": "lung_age", "type": int},
        "Height": {"key": "height", "type": float},
        "Weight": {"key": "weight", "type": float},
        "Ethnicity": {"key": "ethnicity", "type": str},
        "Smoker": {"key": "smoker", "type": bool},
        "Gender": {"key": "gender", "type": str},
        "DateOfBirth": {"key": "date_of_birth", "type": str},
        "Asthma": {"key": "asthma", "type": bool},
        "COPD": {"key": "copd", "type": bool},
    }

    @override
    def _parse_xml(self, root):
        self.patient_id = root.attrib.get("ID", None)

        patient_data_at_present = root.find("PatientDataAtPresent")
        if patient_data_at_present is None:
            raise ValueError("Patient: <PatientDataAtPresent/> not found")

        for child in patient_data_at_present:
            if child.tag in self.field_map:
                self.set_attribute(child.tag, child.text)

        interval_el = root.find(".//Interval")
        if interval_el is None:
            raise ValueError("Patient: <Interval/> not found")

        self.set_attribute("PftStandard", interval_el.attrib.get("PftStandard", None))
        self.set_attribute("Modified", interval_el.attrib.get("Modified", None))

        for child in interval_el:
            if child.tag in self.field_map:
                self.set_attribute(child.tag, child.text)

        type_of_test_el = interval_el.find(".//TypeOfTest")
        if type_of_test_el is None:
            raise ValueError("Patient: <TypeOfTest/> not found")

        self.set_attribute("TypeOfTest", type_of_test_el.text)

        protocol_el = root.find(".//Protocol")
        if protocol_el is None:
            raise ValueError("Patient: <Protocol/> not found")

        name_el = protocol_el.find("Name")
        if name_el is None:
            raise ValueError("Patient: <Protocol/><Name/> not found")

        self.set_attribute("ProtocolName", name_el.text)

    def get_tests(self) -> list[Test]:
        res = []
        test_els = self.root.findall(".//Test")

        for test_el in test_els:
            test = Test(test_el)
            res.append(test)
        return res

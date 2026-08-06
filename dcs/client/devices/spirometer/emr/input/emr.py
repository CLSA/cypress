import sys
import logging

from pathlib import Path

from PySide6.QtWidgets import QApplication

import xml.etree.ElementTree as ET

from devices.spirometer.config import DEVICE_NAME
from devices.spirometer.session import SpirometerSession, SpirometerSessionDialog

logger = logging.getLogger(DEVICE_NAME)


class EMRPlugin:
    @staticmethod
    def write(session: SpirometerSession, ethnicity: str, output_path: Path):

        try:
            root = ET.Element("ndd")

            # add command
            command = ET.SubElement(root, "Command")
            command.set("Type", "PerformTest")

            order_param = ET.SubElement(command, "Parameter")
            order_param.set("Name", "OrderID")
            order_param.text = "1"

            test_param = ET.SubElement(command, "Parameter")
            test_param.set("Name", "TestType")
            test_param.text = "FVL"

            patients = ET.SubElement(root, "Patients")
            patient = ET.SubElement(patients, "Patient")
            patient.set("ID", session.barcode)

            last_name = ET.SubElement(patient, "LastName")
            last_name.text = ""

            first_name = ET.SubElement(patient, "FirstName")
            first_name.text = ""

            is_bio_cal = ET.SubElement(patient, "IsBioCal")
            is_bio_cal.text = "false"

            patient_data = ET.SubElement(patient, "PatientDataAtPresent")

            gender = ET.SubElement(patient_data, "Gender")
            gender.text = session.sex.capitalize()

            dob = ET.SubElement(patient_data, "DateOfBirth")
            dob.text = str(session.dob)

            height = ET.SubElement(patient_data, "Height")
            height.text = str(session.height / 100.0)

            weight = ET.SubElement(patient_data, "Weight")
            weight.text = str(session.weight)

            ethnicity_el = ET.SubElement(patient_data, "Ethnicity")
            ethnicity_el.text = ethnicity

            smoker = ET.SubElement(patient_data, "Smoker")
            smoker.text = "Yes" if session.smoker else "No"

            asthma = ET.SubElement(patient_data, "Asthma")
            asthma.text = "No"

            copd = ET.SubElement(patient_data, "COPD")
            copd.text = "No"

            tree = ET.ElementTree(root)
            ET.indent(tree)
            tree.write(output_path, encoding="utf-8", xml_declaration=True)

        except Exception as e:
            logger.error(e)
            return False

        return True


if __name__ == "__main__":
    app = QApplication()

    session = SpirometerSession(answer_id=1, **SpirometerSessionDialog().prompt())
    EMRPlugin.write(
        session=session, ethnicity="Caucasian", output_path=Path("Cypress_In.xml")
    )

    sys.exit()

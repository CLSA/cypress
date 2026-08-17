import xml.etree.ElementTree as ET

from pathlib import Path

from model import Model


class ECGModel(Model):
    def __init__(self, session, config):
        super().__init__(session, config)

        self.pdf_path = None
        self.xml_path = None
        self.ecg_path = None

    def add_files(
        self, ecg_path: Path, xml_path: Path, pdf_path: Path
    ) -> tuple[bool, str | None]:
        self.reset()

        if not self.add_file(ecg_path, send_name="Ecg"):
            return False, "Could not add ecg file"

        if not self.add_file(xml_path, send_name="Ecg"):
            return False, "Could not add xml file"

        if not self.add_file(pdf_path, send_name="Ecg"):
            return False, "Could not add pdf file"

        self.ecg_path = ecg_path
        self.xml_path = xml_path
        self.pdf_path = pdf_path

        return True, None

    def read_results(self) -> tuple[bool, str | None]:
        if not self.xml_path:
            return False, "No XML file"

        parsed = self._parse_xml_file()
        if not parsed:
            return False, "Failed to parse XML file"

        return True, None

    def _parse_xml_file(self):
        if not self.xml_path:
            return False

        try:
            with open(self.xml_path) as xml_file:
                tree = ET.parse(xml_file)
                root = tree.getroot()
                for child in root:
                    self.metadata[child.tag] = child.attrib
        except (FileNotFoundError, PermissionError):
            return False
        except Exception as e:
            return False

        return True

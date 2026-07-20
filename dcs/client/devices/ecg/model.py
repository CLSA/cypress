import xml.etree.ElementTree as ET

from pathlib import Path

from model import Model


class ECGModel(Model):
    def __init__(self, session, config):
        super().__init__(session, config)

        self.pdf_path = None
        self.xml_path = None
        self.ecg_path = None

    def read_results(self, storage_path: Path) -> tuple[bool, str | None]:
        self.reset()

        if not storage_path.exists() or not storage_path.is_dir():
            return False, "Storage directory does not exist"

        file_count = sum(1 for path in storage_path.iterdir() if path.is_file())

        if file_count > 3:
            return False, "Too many files received"

        ecg_path = None
        pdf_path = None
        xml_path = None

        for file_path in storage_path.iterdir():
            if file_path.is_file() and file_path.exists():
                if file_path.suffix.lower() == '.ecg':
                    ecg_path = file_path
                elif file_path.suffix.lower() == '.pdf':
                    pdf_path = file_path
                elif file_path.suffix.lower() == '.xml':
                    xml_path = file_path
                else:
                    return False, "Unknown file"

        if not xml_path or not xml_path.exists() or not xml_path.is_file():
            return False, "XML file not found"

        if not pdf_path or not pdf_path.exists() or not pdf_path.is_file():
            return False, "PDF file not found"

        if not ecg_path or not ecg_path.exists() or not ecg_path.is_file():
            return False, "ECG file not found"

        self.add_file(file_path=pdf_path, send_name="Ecg")
        self.add_file(file_path=xml_path, send_name="Ecg")
        self.add_file(file_path=ecg_path, send_name="Ecg")

        return True, None

    def _parse_xml_file(self, xml_path: Path):
        with open(xml_path) as xml_file:
            tree = ET.parse(xml_file)
            root = tree.getroot()
            for child in root:
                self.metadata[child.tag] = child.attrib
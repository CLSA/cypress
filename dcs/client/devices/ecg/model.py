import logging

from typing import override
import xml.etree.ElementTree as ET

from pathlib import Path

from model import Model

logger = logging.getLogger("ecg")


class ECGModel(Model):
    def __init__(self, session, config):
        super().__init__(session, config)

        self.pdf_path = None
        self.xml_path = None
        self.ecg_path = None
        self.barcode = None

    def is_valid(self):
        if self.xml_path is None:
            logger.debug("xml path is none")
            return False

        if self.pdf_path is None:
            logger.debug("pdf path is none")
            return False

        if self.ecg_path is None:
            logger.debug("ecg path is none")
            return False

        if not self.xml_path.exists() or not self.xml_path.is_file():
            logger.debug("xml does not exist")
            return False

        if not self.ecg_path.exists() or not self.ecg_path.is_file():
            logger.debug("ecg does not exist")
            return False

        if not self.pdf_path.exists() or not self.pdf_path.is_file():
            logger.debug("pdf does not exist")
            return False

        return True

    def add_files(self, ecg_path: Path, xml_path: Path, pdf_path: Path) -> bool:
        self.reset()

        if not self.add_file(ecg_path, send_name="Ecg", ext=".ecg"):
            logger.debug("couldn't add ecg")
            return False

        if not self.add_file(xml_path, send_name="Ecg", ext=".xml"):
            logger.debug("couldn't add xml")
            return False

        if not self.add_file(pdf_path, send_name="Ecg", ext=".pdf"):
            logger.debug("couldn't add pdf")
            return False

        self.ecg_path = ecg_path
        self.xml_path = xml_path
        self.pdf_path = pdf_path

        return True

    def read_results(self):
        self._parse_xml_file(self.xml_path)

    @override
    def reset(self):
        super().reset()
        self.barcode = None
        self.xml_path = None
        self.pdf_path = None
        self.ecg_path = None

    def _parse_xml_file(self, xml_path: Path):
        with open(xml_path) as xml_file:
            tree = ET.parse(xml_file)
            root = tree.getroot()

            barcode_el = root.find(
                ".//{urn:ge:sapphire:sapphire_3}identifier/{urn:ge:sapphire:sapphire_3}id"
            )
            if barcode_el is None:
                logger.error("no identifier found")
                return False

            self.barcode = barcode_el.attrib.get("V")

            result = self._dfs(root)
            self.metadata = result

    def _dfs(self, el: ET.Element):
        excluded_nodes = ["wav"]
        excluded_units = ["{enum}", "{unitless}", "{bool}"]
        excluded_attr = ["BT", "INV"]

        res = {}

        tag_name = self._clean_tag(el.tag)

        if tag_name in excluded_nodes:
            return res

        if ("V" in el.attrib and "U" not in el.attrib) or el.attrib.get(
            "U"
        ) in excluded_units:
            if (
                el.attrib.get("V") == "false"
                or el.attrib.get("V") == "true"
            ):
                return el.attrib["V"] == True

            return el.attrib.get("V")

        attribute_data = {}
        for name, value in el.attrib.items():
            if name in excluded_attr:
                continue
            elif name == "V":
                attribute_data["value"] = value
            elif name == "U":
                attribute_data["unit"] = value
            else:
                attribute_data[name.lower()] = value

        if not len(el):
            return attribute_data

        for child in el:
            tag = self._clean_tag(child.tag)
            if tag in excluded_nodes:
                continue

            if len(el.findall(child.tag)) > 1:
                res[tag] = []
                for child in el.findall(child.tag):
                    child_json = self._dfs(child)
                    res[tag].append(child_json)
            else:
                child_json = self._dfs(child)
                res[tag] = child_json

        return res

    def _clean_tag(self, tag: str) -> str:
        return tag.replace("{urn:ge:sapphire:sapphire_3}", "")
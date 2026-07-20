from typing import override

from devices.spirometer.emr.output.base import SpirometerXML


class Command(SpirometerXML):
    field_map = {
        "OrderID": {"key": "order_id", "type": str},
        "Attachment": {"key": "report_pdf", "type": str},
    }

    def __init__(self, root):
        super().__init__(root)

    @override
    def _parse_xml(self, root):
        parameters = root.findall("Parameter")
        for parameter in parameters:
            name = parameter.attrib.get("Name", None)
            if name in self.field_map:
                self.set_attribute(name, parameter.text)

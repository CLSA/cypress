from typing import override
from abc import ABC, abstractmethod


class SpirometerXML(ABC):
    name = ""
    field_map = {}

    def __init__(self, root):
        if root is None:
            raise Exception("xml node is required")
        self.root = root
        self.set_defaults()
        self._parse_xml(self.root)

    @override
    def set_defaults(self):
        for attribute in self.field_map.values():
            setattr(self, attribute["key"], None)

    @abstractmethod
    def _parse_xml(self, root):
        pass

    def set_attribute(
        self,
        key,
        value
    ):
        setattr(
            self,
            self.field_map[key]["key"],
            self.convert_type(value, self.field_map[key]["type"]),
        )

    def convert_type(self, value: str, value_type: str | int | bool | float):
        if value is None:
            return None
        elif value_type == str:
            return str(value) if value != "None" else None
        elif value_type == bool:
            return True if (value in ["true", "Yes"]) else False if (value in ["false", "No"]) else str(value)
        elif value_type == float:
            return float(value) if value != "NaN" else None
        elif value_type == int:
            return int(value)
        else:
            return value

    def to_dict(self):
        return self.__dict__()

    @override
    def __dict__(self):
        res = {}
        for key, value in self.field_map.items():
            res[self.field_map[key]["key"]] = getattr(self, value["key"])
        return res

    @override
    def __str__(self):
        return f"{type(self).__name__}\n" + f"".join(
            f"{xml_name}: {getattr(self, self.field_map[xml_name]["key"])}\n"
            for xml_name in self.field_map.keys()
        )
